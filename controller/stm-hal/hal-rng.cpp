#include <stm-hal/hal-rng.hpp>

// ------------------------------------------------------------------------
//                              PUBLIC API
// ------------------------------------------------------------------------

/**
 * Start RNG.
*/
void hal_rng_start()
{

}

/**
 * Stop RNG.
*/
void hal_rng_stop()
{

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

}