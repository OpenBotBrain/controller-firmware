#include <stm-hal/hal-rng.hpp>
#include <stm32l4xx_hal.h>
#include <stm32l4xx_hal_rng.h>
#include <gscope/gscope.hpp>

static RNG_HandleTypeDef s_hrng;
static RNG_Status s_rng_status = RNG_Status::DISABLED;

// ------------------------------------------------------------------------
//                              PUBLIC API
// ------------------------------------------------------------------------

/**
 * Start RNG.
 * Enables RNG CLK and sets hrng Instance.
 * Inits the HAL RNG.
 * Sets RNG_Status to ENABLED.
*/
void hal_rng_start()
{
    __HAL_RCC_RNG_CLK_ENABLE();
    s_hrng.Instance = RNG;

    HAL_StatusTypeDef status = HAL_RNG_Init(&s_hrng);
    if (status != HAL_StatusTypeDef::HAL_OK)
    {
        GSDebug("init status: %d", status);
    }

    s_rng_status = RNG_Status::ENABLED;
}

/**
 * Stop RNG.
 * Disables RNG CLK.
 * DeInits the HAL RNG.
 * Sets RNG_Status to DISABLED.
*/
void hal_rng_stop()
{
    __HAL_RCC_RNG_CLK_DISABLE();

    HAL_StatusTypeDef status = HAL_RNG_DeInit(&s_hrng);
    if (status != HAL_StatusTypeDef::HAL_OK)
    {
        GSDebug("deinit status: %d", status);
    }

    s_rng_status = RNG_Status::DISABLED;
}

/**
 * Once RNG has been started use this function to get a random number.
 * Make sure to use the stop method once done to prevent useless power draw.
 * This is a true random number.
 *
 * @return random number
*/
uint32_t hal_rng_get()
{
    if (s_rng_status == RNG_Status::DISABLED)
    {
        return 0;
    }

    uint32_t random_number = HAL_RNG_GetRandomNumber(&s_hrng);

    return random_number;
}

/**
 * Starts and stops RNG whilst returning a single random number.
 * Can be used as seed for std::srand.
 * This is a true random number.
 *
 * @return random number
*/
uint32_t hal_rng_get_once()
{
    hal_rng_start();

    uint32_t random_number = HAL_RNG_GetRandomNumber(&s_hrng);

    hal_rng_stop();

    return random_number;
}
