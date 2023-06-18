#include <assert.h>
#include <config/appconfig.h>
#include <cstring>
#include <gscope/gscope.hpp>
#include <lsm6dsl/lsm6dsl.hpp>
#include <stm-hal/hal-gpio.hpp>
#include <stm-hal/hal-tim.hpp>
#include <stm-hal/hal-spi.hpp>
#include <system/system-freertos.hpp>

static TaskHandle_t s_task_handler;
static SemaphoreHandle_t s_wait_tx_finish;

// static bool s_enabled = false;
static uint8_t s_tx_buffer[16];
static uint8_t s_rx_buffer[16];

GScopeChannel(s_accelerometer, "imu_accel", float, LSM6DS::TOTAL_AXIS)
GScopeChannel(s_gyro, "imu_gyro", float, LSM6DS::TOTAL_AXIS)

static void s_set_chip_select(bool enable)
{
    enable ? hal_gpio_reset_pin(SYS_IMU_CS_IO) : hal_gpio_set_pin(SYS_IMU_CS_IO);
}

static bool s_spi_write(uint8_t, uint8_t reg, uint8_t* buf, uint16_t size)
{
    assert (size < (sizeof(s_tx_buffer) - 1));

    s_tx_buffer[0] = reg;
    std::memcpy(s_tx_buffer + 1, buf, size);

    hal_spi_transmit_receive(SPI_TYPE_IMU_FLASH, s_tx_buffer, s_rx_buffer, size + 1);

    // Wait until transmition finish
    return system_freertos_semaphore_take(s_wait_tx_finish, 200);
}

static bool s_spi_read(uint8_t, uint8_t reg, uint8_t* buf, uint16_t size)
{
    assert (size < sizeof(s_rx_buffer));

    std::memset(s_tx_buffer, 0, sizeof(s_tx_buffer));
    s_tx_buffer[0] = reg | 0x80;

    hal_spi_transmit_receive(SPI_TYPE_IMU_FLASH, s_tx_buffer, s_rx_buffer, size + 1);

    // Wait until transmition finish
    bool success = system_freertos_semaphore_take(s_wait_tx_finish, 200);
    std::memcpy(buf, s_rx_buffer + 1, size);

    return success;
}

static void s_spi_end_callback(void*)
{
    system_freertos_semaphore_give(s_wait_tx_finish);
}

static constexpr LSM6DS::Config s_imu_config =
{
    .read_cb = s_spi_read,
    .write_cb = s_spi_write,
    .chip_select_cb = s_set_chip_select,
    .get_timestamp_ms32_cb = hal_timer_32_ms,
    .address = 0, // SPI is un use
};

static LSM6DS s_imu(s_imu_config);

static void as_imu_debug_update()
{
    float data[LSM6DS::TOTAL_AXIS];
    s_imu.get_acceleration(data);
    s_accelerometer.produce(data);
    s_imu.get_gyro(data);
    s_gyro.produce(data);
}

static void s_imu_thread(void*)
{
    s_imu.init();

    while(1)
    {
        s_imu.update();

        as_imu_debug_update();

        vTaskDelay(5);
    }
}

// ------------------------------------------------------------------------------------
//                                      PUBLIC API
// ------------------------------------------------------------------------------------
void task_imu_init()
{
    static StaticTask_t s_task_buffer;
    static StackType_t s_stack[SIZE_IMU];

    s_task_handler = xTaskCreateStatic(s_imu_thread, "Imu", SIZE_IMU,
        0, PRI_IMU, s_stack, &s_task_buffer);

    // Mutex to wait for SPI transaction to finish
    static StaticSemaphore_t s_wait_ts_sem;
    s_wait_tx_finish = xSemaphoreCreateBinaryStatic(&s_wait_ts_sem);

    hal_spi_init(SPI_TYPE_IMU_FLASH, s_spi_end_callback, nullptr);
}