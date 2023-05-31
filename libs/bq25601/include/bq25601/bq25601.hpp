#pragma once

#include <assert.h>
#include <bq25601/bq25601-reg.hpp>

class BQ25601
{
    public:
        using RW = bool(*)(uint8_t dir, uint8_t reg, uint8_t* buf, uint8_t size);
        using DelatCb = void(*)(uint32_t);
        using TimestampCb = uint32_t(*)();

        enum class Notification : uint8_t
        {
            // STATUS
            STATUS_NONE = 0,
            STATUS_INIT_SUCCESS,
            STATUS_CHARGER_SYSTEM_STATUS_CHANGED, // good time to print sys_reg
            STATUS_CHARGER_FAULT_CHANGED,         // good time to print fault_reg
            // HEALTH
            HEALTH_GOOD,
            HEALTH_UNSPEC_FAILURE,
            HEALTH_OVERVOLTAGE,
            HEALTH_OVERHEAT,
            HEALTH_SAFETY_TIMER,
            HEALTH_UNKNOWN,
            // ERRORS
            ERROR_READING_I2C,
            ERROR_WRITING_I2C,
            ERROR_READING_SS_REGISTER,
            ERROR_READING_F_REGISTER,
            ERROR_CAN_NOT_ACCESS_ISC_REGISTER,
            ERROR_VALIDATING_MODEL_NUMBER,
            ERROR_RESETING_REGISTERS,
            ERROR_HARDWARE_INIT_FAIL,
            ERROR_SETING_MAX_ALERT_TEMPERATURE,
            ERROR_SETING_HOST_MODE,
            ERROR_SETING_ONLINE,
            ERROR_SETTING_CHARGER_ENABLE,
            ERROR_SETING_CHARGER_VOLTAGE,
            ERROR_SETING_CHARGER_CURRENT,
            ERROR_SETING_AUTO_TERMINATION,
            ERROR_SETTING_INPUT_CURRENT_LIMIT,
        };

        using NotificationCb = void(*)(Notification);

        enum class TempProtection : uint8_t
        {
            TEMP_PROTECTION_90C,
            TEMP_PROTECTION_110C,
        };

        enum class BoostSetpoint : uint8_t
        {
            VOLTAGE_4_85V = 0,
            VOLTAGE_5_00V,
            VOLTAGE_5_15,
            VOLTAGE_5_30,
        };

        enum class BoostCurrentLimit : uint8_t
        {
            CURRENT_LIMIT_500_MA = 0,
            CURRENT_LIMIT_1200_MA
        };

        struct Config
        {
            RW read_cb;
            RW write_cb;
            DelatCb delay_ms_cb;
            TimestampCb get_timestamp_ms32_cb;
            NotificationCb notification_cb;
        };

        struct DriverConfig
        {
            TempProtection temp_protection;         // Shutdown 90C or 110C
            uint16_t charger_setpoint_ma;           // Max 3 Amps
            uint16_t charger_voltage_mv;            // 3.856V to 4.624V
            uint16_t input_current_limit_ma;        // Max 3.2 Amp
            BoostSetpoint boost_setpoint;           // Setpoint when is set to host mode
            BoostCurrentLimit boost_current_setpoint;
        };

        // Charger power supply property routines
        enum class ChargerType : uint8_t
        {
            NONE = 0,       // DISABLE
            TYPE_TRICKLE,   // PRE-CHARGE
            TYPE_FAST,      // NORMAL
        };

        enum class GetHealth : uint8_t
        {
            GOOD = 0,
            OVERVOLTAGE,
            UNSPEC_FAILURE,
            OVERHEAT,
            SAFETY_TIMER_EXPIRE,
            UNKNOWN,
        };

        struct Data
        {
            uint8_t system_status_register;         // Done
            uint8_t fault_register;                 // Done
            TempProtection temp_protection;         // Done
            ChargerType charger_type;               // Done
            GetHealth charger_health;               // Done
            uint32_t timestamp;
        };

        BQ25601(const Config& config, const DriverConfig& driver_config);

        void init();
        void update();
        void irq_handler();
        const Data& get_status();
        void set_system_shutdown();
        void set_max_input_current(uint16_t current_ma);
        void set_boost_power_supply_enabled(bool enable);

    private:
        static constexpr uint16_t INVALID_VALUE = 0xffff;

        const Config& m_config;             // Driver configuration is stored here
        const DriverConfig& m_driver_config;
        Data m_data;                        // Driver data is stored here
        uint8_t m_watchdog;
        bool m_new_irq;                     // If true, new irq was registed
        uint32_t m_update_timestamp {0};    // Update timestamp
        bool m_driver_enable {false};       // If true, init was success
        bool m_request_system_shutdown {false};
        uint8_t m_last_fault_register_helth {0};
        uint16_t m_new_input_current_setpoint_ma {INVALID_VALUE};
        uint16_t m_boost_request {INVALID_VALUE};

        bool read(uint8_t reg, uint8_t& data);
        bool write(uint8_t reg, uint8_t data);
        bool read_register_bits(uint8_t reg, uint8_t mask, uint8_t shift, uint8_t& value);
        bool write_register_bits(uint8_t reg, uint8_t mask, uint8_t shift, uint8_t value);

        uint32_t get_timestamp32();
        void notify(Notification notification);
        bool set_mode_host();
        bool register_reset();
        bool hardware_init();
        bool get_temp_alert_max(TempProtection& temp);
        bool set_temp_alert_max(TempProtection temp);
        bool get_charger_type(ChargerType& type);
        bool set_charger_type(ChargerType type);
        bool set_fet_reset_enable(bool enable);
        bool set_boost_setpoint(BoostSetpoint setpoint);
        bool set_shutdown_delay_time(bool immediately);
        bool set_boost_current_setpoint(BoostCurrentLimit);
        bool set_boost_power_enable(bool enable);

        bool get_health(GetHealth& health);
        bool get_system_fault(uint8_t& f_reg);
        bool get_system_status(uint8_t& ss_reg);

        bool get_online(bool& battery_fet_enable);
        bool set_online(bool enable);

        bool set_charging_voltage(uint32_t voltage_uv);
        bool set_charger_current(uint32_t current_ua);
        bool set_input_current_limit(uint32_t current_ua);

        uint32_t convert_reg02_to_current_mah(uint8_t offset);
        uint8_t convert_current_ua_to_reg02(uint32_t current_ua);
        uint32_t convert_reg04_to_voltage_mv(uint8_t offset);
        uint8_t convert_voltage_ua_to_reg04(uint32_t voltage_uv);
        uint8_t convert_current_ua_to_reg00(uint32_t current_ua);

        static constexpr uint32_t UPDATE_PERIOD_MS = 1000;
        static constexpr uint8_t BQ25601_MODEL_NUMBER = 0x02;
        static constexpr uint32_t CURRENT_STEP_mA = 60;
        static constexpr uint32_t VOLTAGE_STEP_mA = 32;
        static constexpr uint32_t CURRENT_LIMIT_STEP_mA = 100;
        static constexpr uint8_t BQ25601_ADDRESS = 0x6B << 1;
};
