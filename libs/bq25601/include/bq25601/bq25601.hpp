#pragma once

#include <assert.h>
#include <bq25601/bq25601-reg.hpp>

class BQ25601
{
    public:
        using RW = bool(*)(uint8_t dir, uint8_t reg, uint8_t* buf, uint8_t size);
        using DelatCb = void(*)(uint32_t);
        using TimestampCb = uint32_t(*)();

        static constexpr uint32_t UPDATE_PERIOD_MS = 1000;
        static constexpr uint8_t BQ25601_MODEL_NUMBER = 0x02;
        static constexpr uint32_t CURRENT_STEP_uA = 60000;
        static constexpr uint32_t VOLTAGE_STEP_uA = 32000;

        enum class Notification : uint8_t
        {
            NONE = 0,
            CHARGER_SYSTEM_STATUS_CHANGE,           // good time to print sys_reg
            CHARGER_FAULT_CHANGE,                   // good time to print fault_reg
            HIGH_CELL_TEMPERATURE,                  // TODO
            ERROR_READING_SS_REGISTER,
            ERROR_CAN_NOT_ACCESS_ISC_REGISTER,
            ERROR_READING_F_REGISTER,
            ERROR_VALIDATING_MODEL_NUMBER,
            ERROR_READING_I2C,
            ERROR_WRITING_I2C,
            ERROR_RESETING_REGISTERS,
            ERROR_HARDWARE_INIT_FAIL,
            ERROR_SETING_MAX_ALERT_TEMPERATURE,
            ERROR_SETING_HOST_MODE,
            ERROR_SETING_ONLINE,
        };

        using NotificationCb = void(*)(Notification, uint32_t);

        enum class TempProtection : uint8_t
        {
            TEMP_PROTECTION_90C,
            TEMP_PROTECTION_110C,
        };

        struct Config
        {
            RW read_cb;
            RW write_cb;
            uint8_t device_address;
            DelatCb delay_ms_cb;
            TimestampCb get_timestamp_ms32_cb;
            NotificationCb notification_cb;
            TempProtection temp_protection;         // Shutdown 90C or 110C
            uint32_t charger_setpoint_ua;           // Max 3 Amps
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
        };

        BQ25601(const Config& config);

        void init();
        void update();
        void irq_handler();
        const Data& get_status();
        void set_system_shutdown();

    private:
        const Config& m_config;             // Driver configuration is stored here
        Data m_data;                        // Driver data is stored here
        uint8_t m_watchdog;
        bool m_new_irq;                     // If true, new irq was registed
        uint32_t m_update_timestamp {0};    // Update timestamp
        bool m_driver_enable {false};       // If true, init was success
        bool m_request_system_shutdown {false};

        bool read(uint8_t reg, uint8_t& data);
        bool write(uint8_t reg, uint8_t data);
        bool read_register_bits(uint8_t reg, uint8_t mask, uint8_t shift, uint8_t& value);
        bool write_register_bits(uint8_t reg, uint8_t mask, uint8_t shift, uint8_t value);

        uint32_t get_timestamp32();
        void notify(Notification notification, uint32_t value = 0);
        bool set_mode_host();
        bool register_reset();
        bool hardware_init();
        bool get_temp_alert_max(TempProtection& temp);
        bool set_temp_alert_max(TempProtection temp);
        bool get_charger_type(ChargerType& type);
        bool set_charger_type(ChargerType type);

        bool get_health(GetHealth& health);
        bool get_system_fault(uint8_t& f_reg);
        bool get_system_status(uint8_t& ss_reg);

        bool get_online(bool& battery_fet_enable);
        bool set_online(bool enable);

        /**
         * @brief provide a 2-way mapping for the value that goes in
         * the register field and the real-world value that it represents.
         * The index of the array is the value that goes in the register; the
         * number at that index in the array is the real-world value that it
         *
         * @param offset index of the register REG02[7:2]
         * @return uint32_t current in uA
         */
        uint32_t convert_reg02_to_current_uah(uint8_t offset);
        uint8_t convert_current_ua_to_reg02(uint32_t current_ua);

        /**
         * @brief Return the voltage offset in micro volts from controller
         * register
         *
         * @param offset index of the register REG04[7:2]
         * @return uint32_t voltage in uV
         */
        uint32_t convert_reg04_to_voltage_uv(uint8_t offset);
        uint8_t convert_voltage_ua_to_reg04(uint32_t voltage_uv);
};
