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
    return false;
}

bool BQ25601::write(uint8_t reg, uint8_t data)
{
    return m_config.write_cb(m_config.device_address, reg, &data, 1);
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

static constexpr uint32_t CURRENT_STEP_uA = 60000;
uint32_t BQ25601::convert_reg02_to_current_uah(uint8_t offset)
{
    assert(offset <= 50);
    return static_cast<uint32_t>(offset) * CURRENT_STEP_uA;
}

uint8_t BQ25601::convert_current_ua_to_reg02(uint32_t current_ua)
{
    return current_ua / CURRENT_STEP_uA;
}

static constexpr uint32_t VOLTAGE_STEP_uA = 32000;
uint32_t BQ25601::convert_reg04_to_voltage_uv(uint8_t offset)
{
    assert(offset <= 24);
    return 3856000 + static_cast<uint32_t>(offset) * 32000;
}

uint8_t BQ25601::convert_voltage_ua_to_reg04(uint32_t voltage_uv)
{
    return voltage_uv / VOLTAGE_STEP_uA;
}

uint32_t BQ25601::convert_reg06_to_celcius_degrees(uint8_t offset)
{
    return offset == 0 ? 900 : 1100;
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

bool BQ25601::irq_get_status()
{
    uint8_t ss_reg;
    bool report = false;

    if (read(BQ25601_REG_SS, ss_reg) == false)
    {
        notify(Notification::ERROR_READING_SS_REGISTER);
        return false;
    }

    if (ss_reg != m_data.system_status_register)
    {
        // The device is in host mode so when PG_STAT goes from 1->0
        // (i.e., power removed) HIZ needs to be disabled.
        if ((ss_reg & BQ25601_REG_SS_PG_STAT_MASK) &&
            !(ss_reg & BQ25601_REG_SS_PG_STAT_MASK))
        {
            if (write_register_bits(BQ25601_REG_ISC, BQ25601_REG_ISC_EN_HIZ_MASK,
                BQ25601_REG_ISC_EN_HIZ_SHIFT, 0x00) == false)
            {
                notify(Notification::ERROR_CAN_NOT_ACCESS_ISC_REGISTER);
                return false;
            }
        }
        report = true;
        m_data.system_status_register = ss_reg;
    }

    uint8_t f_reg;
    if (read(BQ25601_REG_F, f_reg) == false)
    {
        notify(Notification::ERROR_READING_F_REGISTER);
        return false;
    }

    if (m_data.fault_register != f_reg)
    {
        m_data.fault_register = f_reg;
        m_data.charger_health_valid = true;
        m_data.battery_health_valid = true;
        m_data.battery_status_valid = true;
        report = true;
    }

    // Sometimes bq25601 gives a steady trickle of interrupts even
    // though the watchdog timer is turned off and neither the STATUS
    // nor FAULT registers have changed.  Weed out these sprurious
    // interrupts so userspace isn't alerted for no reason.
    // In addition, the chip always generates an interrupt after
    // register reset so we should ignore that one (the very first
    // interrupt received).

    if (report)
    {
        notify(Notification::POWER_SUPPLY_BATTERY_AND_CHARGER_CHANGE);
    }

    return true;
}

void BQ25601::init()
{
    assert(m_config.read_cb);
    assert(m_config.write_cb);
    assert(m_config.delay_ms_cb);

    m_data = {0};
}

void BQ25601::update()
{
    // TODO: PERIODIC GET
    uint32_t now = get_timestamp32();

    if (m_new_irq)
    {
        irq_get_status();
        m_new_irq = false;
    }
    else if ((now - m_update_timestamp) >= UPDATE_PERIOD_MS)
    {

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

        uint8_t bits_20_percentage = 2 * convert_current_ua_to_reg02(m_charge_current_setpoint_ua) / 10;
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
    uint8_t value;
    if (m_health_is_valid)
    {
        value = m_f_reg;
        m_health_is_valid = false;
    }
    else
    {
        if (read(BQ25601_REG_F, value) == false)
        {
            return false;
        }
    }

    if (value & BQ25601_REG_F_BOOST_FAULT_MASK)
    {
        // This could be over-current or over-voltage but there's
        // no way to tell which.  Return 'OVERVOLTAGE' since there
        // isn't an 'OVERCURRENT' value defined that we can return
        // even if it was over-current.
        health = GetHealth::OVERVOLTAGE;
    }
    else
    {
        value &= BQ25601_REG_F_CHRG_FAULT_MASK;
        value >>= BQ25601_REG_F_CHRG_FAULT_SHIFT;

        switch (value)
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
    return write_register_bits(BQ25601_REG_CTTC, BQ25601_REG_MOC_BATFET_DISABLE_MASK,
        BQ25601_REG_MOC_BATFET_DISABLE_SHIFT, bat_fet_disable);
}

bool BQ25601::get_temp_alert_max(uint16_t& temp_x10)
{
    uint8_t value;
    if (read_register_bits(BQ25601_REG_CTTC, BQ25601_REG_CTTC_TREG_MASK,
        BQ25601_REG_CTTC_TREG_SHIFT, value) == false)
    {
        return false;
    }
    temp_x10 = convert_reg06_to_celcius_degrees(value);
    return true;
}

bool BQ25601::set_temp_alert_max(bool high)
{
    uint8_t value = high ? 1 : 0;
    return write_register_bits(BQ25601_REG_CTTC, BQ25601_REG_CTTC_TREG_MASK,
        BQ25601_REG_CTTC_TREG_SHIFT, value);
}


// static int bq25601_battery_get_property(struct power_supply *psy,
// 		enum power_supply_property psp, union power_supply_propval *val)
// {
// 	struct bq25601_dev_info *bdi =
// 			container_of(psy, struct bq25601_dev_info, battery);
// 	int ret;

// 	dev_dbg(bdi->dev, "prop: %d\n", psp);

// 	pm_runtime_get_sync(bdi->dev);

// 	switch (psp) {
// 	case POWER_SUPPLY_PROP_STATUS:
// 		ret = bq25601_battery_get_status(bdi, val);
// 		break;
// 	case POWER_SUPPLY_PROP_HEALTH:
// 		ret = bq25601_battery_get_health(bdi, val);
// 		break;
// 	case POWER_SUPPLY_PROP_ONLINE:
// 		ret = bq25601_battery_get_online(bdi, val);
// 		break;
// 	case POWER_SUPPLY_PROP_TECHNOLOGY:
// 		/* Could be Li-on or Li-polymer but no way to tell which */
// 		val->intval = POWER_SUPPLY_TECHNOLOGY_UNKNOWN;
// 		ret = 0;
// 		break;
// 	case POWER_SUPPLY_PROP_TEMP_ALERT_MAX:
// 		ret = bq25601_battery_get_temp_alert_max(bdi, val);
// 		break;
// 	case POWER_SUPPLY_PROP_SCOPE:
// 		val->intval = POWER_SUPPLY_SCOPE_SYSTEM;
// 		ret = 0;
// 		break;
// 	default:
// 		ret = -ENODATA;
// 	}

// 	pm_runtime_put_sync(bdi->dev);
// 	return ret;
// }