#include <bq25601/bq25601.hpp>

BQ25601::BQ25601(const Config& config) :
    m_config(config)
{

}

bool BQ25601::read(uint8_t reg, uint8_t& data)
{
    uint8_t read_data;
    if (m_config.read_cb(m_config.device_address, reg, &read_data, 1))
    {
        data = read_data;
        return true;
    }
    notify(Notification::ERROR_READING_I2C);
    return false;
}

bool BQ25601::write(uint8_t reg, uint8_t data)
{
    if (m_config.write_cb(m_config.device_address, reg, &data, 1))
    {
        return true;
    }
    notify(Notification::ERROR_WRITING_I2C);
    return false;
}

bool BQ25601::read_register_bits(uint8_t reg, uint8_t mask, uint8_t shift, uint8_t& value)
{
    uint8_t read_reg;
    if (read(reg, read_reg))
    {
        read_reg &= mask;
        read_reg >>= shift;
        value = read_reg;
        return true;
    }
    return false;
}

bool BQ25601::write_register_bits(uint8_t reg, uint8_t mask, uint8_t shift, uint8_t value)
{
    uint8_t read_reg;
    if (read(reg, read_reg))
    {
        read_reg &= ~mask;
        read_reg |= (value << shift) & mask;
        read_reg >>= shift;
        return write(reg, read_reg);
    }
    return false;
}

uint32_t BQ25601::get_timestamp32()
{
    if (m_config.get_timestamp_ms32_cb)
    {
        return m_config.get_timestamp_ms32_cb();
    }
    return 0;
}

void BQ25601::notify(Notification notification, uint32_t value)
{
    if (m_config.notification_cb)
    {
        return m_config.notification_cb(notification, value);
    }
}

uint32_t BQ25601::convert_reg02_to_current_uah(uint8_t offset)
{
    assert(offset <= 50);
    return static_cast<uint32_t>(offset) * CURRENT_STEP_uA;
}

uint8_t BQ25601::convert_current_ua_to_reg02(uint32_t current_ua)
{
    return current_ua / CURRENT_STEP_uA;
}

uint32_t BQ25601::convert_reg04_to_voltage_uv(uint8_t offset)
{
    assert(offset <= 24);
    return 3856000 + static_cast<uint32_t>(offset) * 32000;
}

uint8_t BQ25601::convert_voltage_ua_to_reg04(uint32_t voltage_uv)
{
    return voltage_uv / VOLTAGE_STEP_uA;
}

/*
* According to the "Host Mode and default Mode" section of the
* manual, a write to any register causes the bq25601 to switch
* from default mode to host mode.  It will switch back to default
* mode after a WDT timeout unless the WDT is turned off as well.
* So, by simply turning off the WDT, we accomplish both with the
* same write.
*/
bool BQ25601::set_mode_host()
{
    uint8_t read_value;
    if (read(BQ25601_REG_CTTC, read_value))
    {
        m_watchdog = ((read_value & BQ25601_REG_CTTC_WATCHDOG_MASK) >>
            BQ25601_REG_CTTC_WATCHDOG_SHIFT);
        read_value &= ~BQ25601_REG_CTTC_WATCHDOG_MASK;

        return write(BQ25601_REG_CTTC, read_value);
    }
    return false;
}

bool BQ25601::register_reset()
{
    uint8_t tries = 5;
    uint8_t value;

    // Reset the registers
    if (write_register_bits(BQ25601_REG_VPRS, BQ25601_REG_VPRS_REG_RESET_MASK,
        BQ25601_REG_VPRS_REG_RESET_SHIFT, 0x01) == false)
    {
        return false;
    }

    // Reset bit will be cleared by hardware so poll until it is
    do {
        if (read_register_bits(BQ25601_REG_VPRS, BQ25601_REG_VPRS_REG_RESET_MASK,
            BQ25601_REG_VPRS_REG_RESET_SHIFT, value) == false)
        {
            return false;
        }

        if (value == 0)
        {
            return true;
        }

        m_config.delay_ms_cb(2);

    } while (tries--);

    return false;
}

bool BQ25601::hardware_init()
{
    uint8_t value;
    // First check that the device really is what its supposed to be
    if (read_register_bits(BQ25601_REG_VPRS, BQ25601_REG_VPRS_PN_MASK,
        BQ25601_REG_VPRS_PN_SHIFT, value) == false)
    {
        return false;
    }

    if (value != BQ25601_MODEL_NUMBER)
    {
        notify(Notification::ERROR_VALIDATING_MODEL_NUMBER);
        return false;
    }

    // Reset all register to default
    if (!register_reset())
    {
        notify(Notification::ERROR_RESETING_REGISTERS);
        return false;
    }

    // Configure Host mode
    if (!set_mode_host())
    {
        notify(Notification::ERROR_SETING_HOST_MODE);
        return false;
    }

    return true;
}

bool BQ25601::get_temp_alert_max(TempProtection& temp)
{
    uint8_t value;
    if (read_register_bits(BQ25601_REG_CTTC, BQ25601_REG_CTTC_TREG_MASK,
        BQ25601_REG_CTTC_TREG_SHIFT, value) == false)
    {
        return false;
    }

    temp = value == 0 ? TempProtection::TEMP_PROTECTION_90C : TempProtection::TEMP_PROTECTION_110C;
    return true;
}

bool BQ25601::set_temp_alert_max(TempProtection temp)
{
    uint8_t value = temp == TempProtection::TEMP_PROTECTION_110C ? 1 : 0;
    return write_register_bits(BQ25601_REG_CTTC, BQ25601_REG_CTTC_TREG_MASK,
        BQ25601_REG_CTTC_TREG_SHIFT, value);
}

bool BQ25601::get_charger_type(ChargerType& type)
{
    uint8_t value;
    if (read_register_bits(BQ25601_REG_POC, BQ25601_REG_POC_CHG_CONFIG_MASK,
        BQ25601_REG_POC_CHG_CONFIG_SHIFT, value) == false)
    {
        return false;
    }

    // If POC[CHG_CONFIG] (REG01[5:4]) == 0, charge is disabled
    if (value == 0)
    {
        type = ChargerType::NONE;
    }
    else
    {
        // read the current setpoint
        if (read_register_bits(BQ25601_REG_CCC, BQ25601_REG_CCC_ICHG_MASK,
            BQ25601_REG_CCC_ICHG_SHIFT, value) == false)
        {
            return false;
        }

        uint8_t bits_20_percentage = 2 * convert_current_ua_to_reg02(m_config.charger_setpoint_ua) / 10;
        type = value <= bits_20_percentage ? ChargerType::TYPE_TRICKLE : ChargerType::TYPE_FAST;
    }
    return true;
}

bool BQ25601::set_charger_type(ChargerType type)
{
    uint8_t charger_config = 0;
    uint8_t enable_termination = 0;

    // According to the "Termination when REG02[0] = 1" section of
    // the bq25601 manual, the trickle charge could be less than the
    // termination current so it recommends turning off the termination
    // function.
    // Note: AFAICT from the datasheet, the user will have to manually
    // turn off the charging when in 20% mode.  If its not turned off,
    // there could be battery damage.  So, use this mode at your own risk.

    switch (type)
    {
        default:
            break;
        case ChargerType::TYPE_TRICKLE:
            charger_config = 0x01;
            break;
        case ChargerType::TYPE_FAST:
            charger_config = 0x01;
            enable_termination = 0x01;
            break;
    }

    if (charger_config)
    {
        // configure the charge type
        if (write_register_bits(BQ25601_REG_CTTC, BQ25601_REG_CTTC_EN_TERM_MASK,
            BQ25601_REG_CTTC_EN_TERM_SHIFT, enable_termination) == false)
        {
            return false;
        }
    }

    // enable or disable the charger
    return write_register_bits(BQ25601_REG_POC, BQ25601_REG_POC_CHG_CONFIG_MASK,
        BQ25601_REG_POC_CHG_CONFIG_SHIFT, charger_config);
}

bool BQ25601::get_health(GetHealth& health)
{
    if (m_data.fault_register & BQ25601_REG_F_BOOST_FAULT_MASK)
    {
        // This could be over-current or over-voltage but there's
        // no way to tell which.  Return 'OVERVOLTAGE' since there
        // isn't an 'OVERCURRENT' value defined that we can return
        // even if it was over-current.
        health = GetHealth::OVERVOLTAGE;
    }
    else
    {
        uint8_t tmp = m_data.fault_register & BQ25601_REG_F_CHRG_FAULT_MASK;
        tmp = tmp >> BQ25601_REG_F_CHRG_FAULT_SHIFT;

        switch (tmp)
        {
            case 0: // normal
                health = GetHealth::GOOD;
                break;
            case 1: // Input Fault (VBUS OVP or VBAT<VBUS<3.8V)
                // This could be over-voltage or under-voltage
                // and there's no way to tell which.  Instead
                // of looking foolish and returning 'OVERVOLTAGE'
                // when its really under-voltage, just return
                // 'UNSPEC_FAILURE'.
                health = GetHealth::UNSPEC_FAILURE;
                break;
            case 2: // Thermal Shutdown
                health = GetHealth::OVERHEAT;
                break;
            case 3: // Charge Safety Timer Expiration
                health = GetHealth::SAFETY_TIMER_EXPIRE;
                break;
            default:
                health = GetHealth::UNKNOWN;
                break;
        }
    }

    return true;
}

bool BQ25601::get_system_fault(uint8_t& f_reg)
{
    uint8_t f_reg_temp;
    if (read(BQ25601_REG_F, f_reg_temp) == false)
    {
        notify(Notification::ERROR_READING_F_REGISTER);
        return false;
    }

    if (m_data.fault_register != f_reg_temp)
    {
        f_reg = f_reg_temp;
        notify(Notification::CHARGER_FAULT_CHANGE);
    }

    return true;
}

bool BQ25601::get_system_status(uint8_t& ss_reg)
{
    uint8_t ss_reg_temp;
    if (read(BQ25601_REG_SS, ss_reg_temp) == false)
    {
        notify(Notification::ERROR_READING_SS_REGISTER);
        return false;
    }

    if (ss_reg_temp != m_data.system_status_register)
    {
        // The device is in host mode so when PG_STAT goes from 1->0
        // (i.e., power removed) HIZ needs to be disabled.
        if ((ss_reg_temp & BQ25601_REG_SS_PG_STAT_MASK) &&
            !(ss_reg_temp & BQ25601_REG_SS_PG_STAT_MASK))
        {
            if (write_register_bits(BQ25601_REG_ISC, BQ25601_REG_ISC_EN_HIZ_MASK,
                BQ25601_REG_ISC_EN_HIZ_SHIFT, 0x00) == false)
            {
                notify(Notification::ERROR_CAN_NOT_ACCESS_ISC_REGISTER);
                return false;
            }
        }
        ss_reg = ss_reg_temp;
        notify(Notification::CHARGER_SYSTEM_STATUS_CHANGE);
    }
    return true;
}

bool BQ25601::get_online(bool& battery_fet_enable)
{
    uint8_t bat_fet_disable;
    if (read_register_bits(BQ25601_REG_MOC, BQ25601_REG_MOC_BATFET_DISABLE_MASK,
        BQ25601_REG_MOC_BATFET_DISABLE_SHIFT, bat_fet_disable) == false)
    {
        return false;
    }
    battery_fet_enable = bat_fet_disable == 0;
    return true;
}

bool BQ25601::set_online(bool enable)
{
    uint8_t bat_fet_disable = enable ? 0 : 1;
    bool ret = write_register_bits(BQ25601_REG_CTTC, BQ25601_REG_MOC_BATFET_DISABLE_MASK,
        BQ25601_REG_MOC_BATFET_DISABLE_SHIFT, bat_fet_disable);

    if (ret)
    {
        notify(Notification::ERROR_SETING_ONLINE);
    }

    return ret;
}

// ------------------------------------------------------------------------
//                              PUBLIC API
// ------------------------------------------------------------------------
void BQ25601::init()
{
    assert(m_config.read_cb);
    assert(m_config.write_cb);
    assert(m_config.delay_ms_cb);

    m_data = {0};
    m_new_irq = true;

    // Initialize IC, set all to default
    if (!hardware_init())
    {
        notify(Notification::ERROR_HARDWARE_INIT_FAIL);
        return;
    }

    // Configure temperature protection
    if (!set_temp_alert_max(m_config.temp_protection))
    {
        notify(Notification::ERROR_SETING_MAX_ALERT_TEMPERATURE);
        return;
    }

    // TODO: CONFIGURE WORKING VALUES!!!

    m_driver_enable = true;
}

void BQ25601::update()
{
    if (!m_driver_enable)
    {
        return;
    }

    uint32_t now = get_timestamp32();

    if (m_new_irq || (now - m_update_timestamp) >= UPDATE_PERIOD_MS)
    {
        m_new_irq = false;

        get_system_fault(m_data.fault_register);
        get_system_status(m_data.system_status_register);
        get_health(m_data.charger_health);
        get_temp_alert_max(m_data.temp_protection);
        get_charger_type(m_data.charger_type);

        m_update_timestamp = now;
    }

    if (m_request_system_shutdown)
    {
        m_request_system_shutdown = false;
        set_online(false);
    }
}

void BQ25601::irq_handler()
{
    m_new_irq = true;
}

const BQ25601::Data& BQ25601::get_status()
{
    return m_data;
}

void BQ25601::set_system_shutdown()
{
    m_request_system_shutdown = true;
}
