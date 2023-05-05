#include <assert.h>
#include <stm-hal/hal-gpio.hpp>

static const GPIOInitConfig* s_board_gpio[TOTAL_GPIO];
static const GPIOInitConfig* s_gpio_config = nullptr;

#define GPIO_PIN(io) (s_board_gpio[io]->pin)
#define GPIO_PORT(io) (s_board_gpio[io]->port)

// This is used only when multiple configuration of file is used
static void hal_gpio_init_config(const GPIOInitConfig* gpios, int num_gpios)
{
    for (int i = 0; i < num_gpios; i++)
    {
        s_board_gpio[gpios[i].id] = &gpios[i];
    }
}

void hal_gpio_init_initialize(const GPIOInitConfig* gpios_arr, int num_gpios)
{
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
    __HAL_RCC_GPIOE_CLK_ENABLE();
    __HAL_RCC_GPIOF_CLK_ENABLE();

    for (int i = 0; i < num_gpios; i++)
    {
        const GPIOInitConfig* config = &gpios_arr[i];

        if (config->pin != 0)
        {
            GPIO_InitTypeDef gpio_config;
            gpio_config.Pin = config->pin;
            gpio_config.Mode = config->mode;
            gpio_config.Pull = config->pull;
            gpio_config.Speed = config->speed;
            gpio_config.Alternate = config->alternate;

            HAL_GPIO_Init(config->port, &gpio_config);

            if (config->mode== GPIO_MODE_OUTPUT_PP || config->mode == GPIO_MODE_OUTPUT_OD)
            {
                if (config->state == SET_HIGH)
                {
                    hal_gpio_set_pin(i);
                }
                else
                {
                    hal_gpio_reset_pin(i);
                }
            }
        }
    }
}

void hal_gpio_init_default(const BoardSpecificConfig* board_config)
{
    s_gpio_config = board_config->gpio_config;
    assert(s_gpio_config);

    // Initialize ID table
    hal_gpio_init_config(s_gpio_config, TOTAL_GPIO);

    // Initialize GPIOS
    hal_gpio_init_initialize(s_gpio_config, TOTAL_GPIO);
}

bool hal_gpio_read_pin_default(GPIO_TypeDef* port, uint16_t pin)
{
    return HAL_GPIO_ReadPin(port, pin);
}

bool hal_gpio_read_pin(uint16_t pin)
{
    return HAL_GPIO_ReadPin(GPIO_PORT(pin), GPIO_PIN(pin));
}

void hal_gpio_reset_pin(uint16_t pin)
{
    GPIO_PORT(pin)->BRR = static_cast<uint32_t>(GPIO_PIN(pin));
}

void hal_gpio_set_pin(uint16_t pin)
{
    GPIO_PORT(pin)->BSRR = static_cast<uint32_t>(GPIO_PIN(pin));
}
