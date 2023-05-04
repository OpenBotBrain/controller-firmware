#include <assert.h>
#include <config/appconfig.h>
#include <stm32l4xx_hal.h>
#include <stm-hal/hal-i2c.hpp>
#include <cstring>

static constexpr uint16_t I2C_BUFFER_SIZE = 64;

enum class I2CState : uint8_t
{
    NONE = 0,
    RX_MODE_CMD,
    RX_MODE_DATA,
    TX_MODE,
    ERROR,
};

struct I2CChannelConfig
{
    I2C_TypeDef* i2c_instance;
    uint32_t timing;
    IRQn_Type irq_type;
    uint8_t irq_priority;
};

struct I2CData
{
    I2C_HandleTypeDef handler;
    FinishI2CCb finish_tx_cb;
    void* param;
    uint8_t address;
    uint8_t buf[I2C_BUFFER_SIZE];
    uint16_t buf_size;
    I2CState state;
};

static constexpr I2CChannelConfig s_i2c_config[I2C_TYPE_TOTAL] =
{
    { I2C4, 0x10909CEC, I2C1_EV_IRQn, PRI_HARD_PORT_I2C }, // I2C_TYPE_PORT_INPUT_1
    { I2C1, 0x10909CEC, I2C2_EV_IRQn, PRI_HARD_PORT_I2C }, // I2C_TYPE_PORT_INPUT_2
    { I2C2, 0x10909CEC, I2C3_EV_IRQn, PRI_HARD_PORT_I2C }, // I2C_TYPE_PORT_INPUT_3
    { I2C3, 0x10909CEC, I2C4_EV_IRQn, PRI_HARD_SYS_I2C  }, // I2C_TYPE_SYSTEM
};

static I2CData s_i2c_data[I2C_TYPE_TOTAL];

void hal_i2c_init_default(uint8_t board_rev)
{
    (void) board_rev;

    __HAL_RCC_I2C1_CLK_ENABLE();
    __HAL_RCC_I2C2_CLK_ENABLE();
    __HAL_RCC_I2C3_CLK_ENABLE();
    __HAL_RCC_I2C4_CLK_ENABLE();
}

void hal_i2c_init(uint8_t type)
{
    assert(type < I2C_TYPE_TOTAL);

    I2CData* i2c = &s_i2c_data[type];
    const I2CChannelConfig* config = &s_i2c_config[type];

    i2c->state = I2CState::NONE;

    i2c->handler.Instance = config->i2c_instance;
    i2c->handler.Init.Timing = config->timing;
    i2c->handler.Init.OwnAddress1 = 0;
    i2c->handler.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
    i2c->handler.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
    i2c->handler.Init.OwnAddress2 = 0;
    i2c->handler.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
    i2c->handler.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
    i2c->handler.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
    assert (HAL_I2C_Init(&i2c->handler) != HAL_OK);

    assert (HAL_I2CEx_ConfigAnalogFilter(&i2c->handler, I2C_ANALOGFILTER_ENABLE) == HAL_OK);

    assert (HAL_I2CEx_ConfigDigitalFilter(&i2c->handler, 0) == HAL_OK);

    __HAL_I2C_ENABLE_IT(&i2c->handler, I2C_IT_TCI);
    __HAL_I2C_ENABLE_IT(&i2c->handler, I2C_IT_RXI);

    HAL_NVIC_SetPriority(config->irq_type, config->irq_priority, 0);
    HAL_NVIC_EnableIRQ(config->irq_type);
}

bool hal_i2c_read(uint8_t type, uint8_t address, uint8_t reg, uint16_t data_size,
    FinishI2CCb finish_tx_cb, void* param)
{
    assert(type < I2C_TYPE_TOTAL);
    assert(data_size < I2C_BUFFER_SIZE);

    I2CData* data = &s_i2c_data[type];
    I2C_HandleTypeDef* handler = &data->handler;

    if (data->state != I2CState::NONE)
    {
        data->buf_size = data_size;
        data->address = address;
        data->finish_tx_cb = finish_tx_cb;
        data->param = param;

        // send the command first
        if (HAL_I2C_Master_Transmit_IT(handler, address, &reg, 1) == HAL_OK)
        {
            data->state = I2CState::RX_MODE_CMD;
            return true;
        }
        else
        {
            data->state = I2CState::NONE;
        }
    }

    return false;
}

bool hal_i2c_write(uint8_t type, uint8_t address, uint8_t reg, uint8_t* buf, uint16_t data_size,
    FinishI2CCb finish_tx_cb, void* param)
{
    assert(type < I2C_TYPE_TOTAL);
    assert(data_size < (I2C_BUFFER_SIZE - 1));

    I2CData* data = &s_i2c_data[type];
    I2C_HandleTypeDef* handler = &data->handler;

    if (data->state != I2CState::NONE)
    {
        data->buf[0] = reg;
        data->buf_size = 0;
        data->address = address;
        data->finish_tx_cb = finish_tx_cb;
        data->param = param;

        std::memcpy(data->buf + 1, buf, data_size);

        if (HAL_I2C_Master_Transmit_IT(handler, address, data->buf, data_size + 1) == HAL_OK)
        {
            data->state = I2CState::TX_MODE;
            return true;
        }
        else
        {
            data->state = I2CState::NONE;
        }
    }

    return false;
}

static void i2c_tx_rx_complete(I2C_HandleTypeDef* hi2c, bool error)
{
    for (int i = 0; i < I2C_TYPE_TOTAL; i++)
    {
        I2CData* data = &s_i2c_data[i];

        if (&data->handler == hi2c)
        {
            if (error)
            {
                data->state = I2CState::ERROR;
            }

            switch (data->state)
            {
                case I2CState::RX_MODE_CMD:
                    data->state = I2CState::RX_MODE_DATA;
                    HAL_I2C_Master_Receive_IT(hi2c, (data->address | 0x01), data->buf, data->buf_size);
                    break;

                case I2CState::RX_MODE_DATA:
                case I2CState::TX_MODE:
                case I2CState::ERROR:
                    data->state = I2CState::NONE;
                    if (data->finish_tx_cb)
                    {
                        data->finish_tx_cb(error, data->param, data->buf, data->buf_size);
                    }
                    break;

                default:
                    assert(0);  // error
                    break;
            }
            return;
        }
    }
}

static void i2c_ev_irq(I2C_TypeDef* hi2c)
{
    for (int i = 0; i < I2C_TYPE_TOTAL; i++)
    {
        if (s_i2c_config[i].i2c_instance == hi2c)
        {
            HAL_I2C_EV_IRQHandler(&s_i2c_data[i].handler);
            return;
        }
    }
}

static void i2c_er_irq(I2C_TypeDef* hi2c)
{
    for (int i = 0; i < I2C_TYPE_TOTAL; i++)
    {
        if (s_i2c_config[i].i2c_instance == hi2c)
        {
            HAL_I2C_ER_IRQHandler(&s_i2c_data[i].handler);
            return;
        }
    }
}

extern "C"
{

void HAL_I2C_MasterTxCpltCallback(I2C_HandleTypeDef* hi2c)
{
    i2c_tx_rx_complete(hi2c, false);
}

void HAL_I2C_MasterRxCpltCallback(I2C_HandleTypeDef* hi2c)
{
    i2c_tx_rx_complete(hi2c, false);
}

void HAL_I2C_ErrorCallback(I2C_HandleTypeDef* hi2c)
{
    i2c_tx_rx_complete(hi2c, true);
}

void I2C1_EV_IRQHandler(void)
{
    i2c_ev_irq(I2C1);
}

void I2C2_EV_IRQHandler(void)
{
    i2c_ev_irq(I2C2);
}

void I2C3_EV_IRQHandler(void)
{
    i2c_ev_irq(I2C3);
}

void I2C4_EV_IRQHandler(void)
{
    i2c_ev_irq(I2C4);
}

void I2C1_ER_IRQHandler(void)
{
    i2c_er_irq(I2C1);
}

void I2C2_ER_IRQHandler(void)
{
    i2c_er_irq(I2C2);
}

void I2C3_ER_IRQHandler(void)
{
    i2c_er_irq(I2C3);
}

void I2C4_ER_IRQHandler(void)
{
    i2c_er_irq(I2C4);
}

}
