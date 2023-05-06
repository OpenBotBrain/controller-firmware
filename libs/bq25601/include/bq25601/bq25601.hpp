#pragma once

#include <assert.h>
#include <bq25601/bq25601-reg.hpp>

class BQ25601
{
    public:
        using RW = bool(*)(uint8_t dir, uint8_t reg, uint8_t* buf, uint8_t size);
        using DelatCb = void(*)(uint32_t);
        using TimestampCb = uint32_t(*)();

        static constexpr uint32_t UPDATE_PERIOD_MS = 500;

        enum class Notification : uint8_t
        {
            NONE = 0,
            ERROR_READING_SS_REGISTER,
            ERROR_CAN_NOT_ACCESS_ISC_REGISTER,
            ERROR_READING_F_REGISTER,
            POWER_SUPPLY_BATTERY_AND_CHARGER_CHANGE,    // good time to print ss_reg and f reg
        };

        using NotificationCb = void(*)(Notification, uint32_t);

        struct Config
        {
            RW read_cb;
            RW write_cb;
            uint8_t device_address;
            DelatCb delay_ms_cb;
            TimestampCb get_timestamp_ms32_cb;
            NotificationCb notification_cb;
        };

        enum class TempProtection : uint8_t
        {
            DISABLE = 0,
            TEMP_PROTECTION_90C,
            TEMP_PROTECTION_110C,
        };

        struct Data
        {
            uint32_t charge_current_setpoint_ua;
            uint8_t system_status_register;
            uint8_t fault_register;
            bool charger_health_valid;
            bool battery_health_valid;
            bool battery_status_valid;
        };

        BQ25601(const Config& config);

        void init();
        void update();
        void irq_handler();
        const Data& get_status();

    private:
        const Config& m_config;
        uint8_t m_watchdog;
        uint32_t m_charge_current_setpoint_ua;  // Driver Charger setpoint!
        bool m_new_irq {false};
        uint32_t m_update_timestamp {0};
        Data m_data;

        uint32_t get_timestamp32();
        void notify(Notification notification, uint32_t value = 0);

        bool read(uint8_t reg, uint8_t& data);
        bool write(uint8_t reg, uint8_t data);
        bool read_register_bits(uint8_t reg, uint8_t mask, uint8_t shift, uint8_t& value);
        bool write_register_bits(uint8_t reg, uint8_t mask, uint8_t shift, uint8_t value);

        bool irq_get_status();
        /*
        * According to the "Host Mode and default Mode" section of the
        * manual, a write to any register causes the bq25601 to switch
        * from default mode to host mode.  It will switch back to default
        * mode after a WDT timeout unless the WDT is turned off as well.
        * So, by simply turning off the WDT, we accomplish both with the
        * same write.
        */
        bool set_mode_host();

        bool register_reset();

        // Charger power supply property routines
        enum class ChargerType : uint8_t
        {
            NONE = 0,       // DISABLE
            TYPE_TRICKLE,   // PRE-CHARGE
            TYPE_FAST,      // NORMAL
        };
        bool get_charger_type(ChargerType& type);
        bool set_charger_type(ChargerType type);

        enum class GetHealth : uint8_t
        {
            GOOD = 0,
            OVERVOLTAGE,
            UNSPEC_FAILURE,
            OVERHEAT,
            SAFETY_TIMER_EXPIRE,
            UNKNOWN,
        };
        bool m_health_is_valid = {false};
        uint8_t m_f_reg {0};
        bool get_health(GetHealth& health);

        bool get_online(bool& battery_fet_enable);
        bool set_online(bool enable);

        bool get_temp_alert_max(uint16_t& temp_x10);
        // if true, set to 110 else 90
        bool set_temp_alert_max(bool high);


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

        /**
         * @brief Convert the temperature register value to C degree.
         *
         * @param offset REG06[1:0] (TREG)
         * @return uint32_t in tenths of degrees Celcius
         */
        uint32_t convert_reg06_to_celcius_degrees(uint8_t offset);
};
