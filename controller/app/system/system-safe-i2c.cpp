#include <assert.h>
#include <system/system-freertos.hpp>
#include <stm-hal/hal-i2c.hpp>
#include <cstring>

static SemaphoreHandle_t s_mutex = nullptr;
static SemaphoreHandle_t s_wait_complete_transaction = nullptr;
static constexpr TickType_t SEMAPHORE_TIMEOUT_MS = 250;
static constexpr uint16_t I2C_RX_SIZE = 64;
static uint8_t* s_rx_data;

static void s_i2c_finish_cb(bool error, void*, uint8_t* data, uint16_t size)
{
    if (error == false)
    {
        if (size != 0 && size < I2C_RX_SIZE)
        {
            assert(s_rx_data);
            std::memcpy(s_rx_data, data, size);
        }
    }

    system_freertos_semaphore_give(s_wait_complete_transaction);
}

void system_safe_i2c_init()
{
    static StaticSemaphore_t s_semaphore;
    s_wait_complete_transaction = xSemaphoreCreateBinaryStatic(&s_semaphore);

    static StaticSemaphore_t s_mutex_semaphore;
    s_mutex = xSemaphoreCreateMutexStatic(&s_mutex_semaphore);

    hal_i2c_init(I2C_TYPE_SYSTEM);
}

bool system_safe_i2c_write(uint8_t address, uint8_t reg, uint8_t* buf, uint8_t buf_size)
{
    bool ret = false;

    if (system_freertos_semaphore_take(s_mutex, SEMAPHORE_TIMEOUT_MS))
    {
        ret = hal_i2c_write(I2C_TYPE_SYSTEM, address, reg, buf, buf_size, s_i2c_finish_cb, nullptr);

        if (ret)
        {
            ret = system_freertos_semaphore_take(s_wait_complete_transaction, SEMAPHORE_TIMEOUT_MS);
        }

        system_freertos_semaphore_give(s_mutex);
    }

    return ret;
}

bool system_safe_i2c_read(uint8_t address, uint8_t reg, uint8_t* buf, uint8_t buf_size)
{
    bool ret = false;

    if (system_freertos_semaphore_take(s_mutex, SEMAPHORE_TIMEOUT_MS))
    {
        s_rx_data = buf;
        ret =  hal_i2c_read(I2C_TYPE_SYSTEM, address, reg, buf_size, s_i2c_finish_cb, nullptr);

        if (ret)
        {
            ret = system_freertos_semaphore_take(s_wait_complete_transaction, SEMAPHORE_TIMEOUT_MS);
        }

        s_rx_data = nullptr;

        system_freertos_semaphore_give(s_mutex);
    }

    return ret;
}