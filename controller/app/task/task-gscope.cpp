#include <config/appconfig.h>
#include <system/system-freertos.hpp>
#include <system/system-version.hpp>
#include <gscope/gscope.hpp>

static TaskHandle_t s_task_handler;
static SemaphoreHandle_t s_add_buffer_mutex = NULL;
static SemaphoreHandle_t s_wait_tx_finish;
static bool s_rtos_on = false;
static uint8_t s_local_rx_buffer[128];

static bool s_buffer_lock(void)
{
    return s_rtos_on ? system_freertos_semaphore_take(s_add_buffer_mutex, 100) : true;
}

static void s_buffer_unlock(void)
{
    if (s_rtos_on)
    {
        system_freertos_semaphore_give(s_add_buffer_mutex);
    }
}

static bool s_send_data(const uint8_t* data, uint32_t size)
{
    (void) data;
    (void) size;
    // hal_debug_uart_transmitte(data, size);
    return system_freertos_semaphore_take(s_wait_tx_finish, 200);
}

static constexpr uint32_t RING_BUFFER_SIZE = 4096;
static uint8_t s_ring_buffer[RING_BUFFER_SIZE];
static constexpr GScopeSerialBuffer::SerialConfig s_serial_config =
{
    .buffer_lock = s_buffer_lock,
    .buffer_unlock = s_buffer_unlock,
    .send_data = s_send_data,
    .tx_buf = s_ring_buffer,
    .ring_buffer_size = RING_BUFFER_SIZE
};

static GScope s_gscope(s_serial_config, system_version_get_version);

// static void s_finish_sending_data()
// {
//     system_freertos_semaphore_give(s_wait_tx_finish);
// }

static void s_gscope_thread(void*)
{
    s_rtos_on = true;

    while(1)
    {
        // We do the serial decoding inside the thread
        uint32_t read_size = 0;// hal_debug_uart_read(s_local_rx_buffer, sizeof(s_local_rx_buffer));

        if (read_size != 0)
        {
            s_gscope.incoming(s_local_rx_buffer, read_size);
        }

        // Update main routine, if return true, we should keep updating information
        if (!s_gscope.update())
        {
            vTaskDelay(5);
        }
    }
}

// ------------------------------------------------------------------------------------
//                                      PUBLIC API
// ------------------------------------------------------------------------------------
void task_gscope_init()
{
    // Create task for memory hole
    static StaticTask_t s_task_buffer;
    static StackType_t s_stack[SIZE_GSCOPE];

    s_task_handler = xTaskCreateStatic(s_gscope_thread, "GScope", SIZE_GSCOPE,
        0, PRI_GSCOPE, s_stack, &s_task_buffer);

    // Init semaphore and mutex
    static StaticSemaphore_t s_buf_sem;
    s_add_buffer_mutex = xSemaphoreCreateMutexStatic(&s_buf_sem);
    static StaticSemaphore_t s_wait_ts_sem;
    s_wait_tx_finish = xSemaphoreCreateBinaryStatic(&s_wait_ts_sem);

    // hal_debug_uart_init(s_finish_sending_data);

    s_gscope.enable_transmission(true);
}