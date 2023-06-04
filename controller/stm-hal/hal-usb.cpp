#include <assert.h>
#include <system/system-freertos.hpp>
#include <stm32l4xx_hal.h>
#include <usbd_cdc.h>
#include <stm-hal/hal-usb.hpp>

/* Define size for the receive and transmit buffer over CDC */
/* It's up to user to redefine and/or remove those define */
#define APP_RX_DATA_SIZE  2048
#define APP_TX_DATA_SIZE  2048

#define PRI_USB_TASK            14      // USB app
#define SIZE_USB_TASK           256
#define USB_RX_TASK_DELAY_MS    100

static uint8_t s_user_rx_buffer[APP_RX_DATA_SIZE];
static uint8_t s_user_tx_buffer[APP_TX_DATA_SIZE];
static USBD_HandleTypeDef s_usb_device_fs_handler;
static SemaphoreHandle_t s_wait_complete_transaction = nullptr;
static SemaphoreHandle_t s_new_usb_data = nullptr;
static uint32_t s_rx_data_length = 0;
static TaskHandle_t s_task_handler;

struct USBCofig
{
    USBDataCb cb;
    void* param;
    bool enable;
};
static USBCofig s_config[USB_CALLBACK_TYPES] = {0};
static FinishTransmiteCallback s_finish_tx = nullptr;
static void* s_tx_param;
static bool s_busy = false;

extern USBD_DescriptorsTypeDef FS_Desc;

static void s_reset_usb_rx()
{
    s_rx_data_length = 0;
    USBD_CDC_SetRxBuffer(&s_usb_device_fs_handler, s_user_rx_buffer);
    USBD_CDC_ReceivePacket(&s_usb_device_fs_handler);
}

static int8_t CDC_Init_FS(void)
{
    s_rx_data_length = 0;
    USBD_CDC_SetTxBuffer(&s_usb_device_fs_handler, s_user_tx_buffer, 0);
    USBD_CDC_SetRxBuffer(&s_usb_device_fs_handler, s_user_rx_buffer);
    return USBD_OK;
}

static int8_t CDC_DeInit_FS(void)
{
    return USBD_OK;
}

static int8_t CDC_Control_FS(uint8_t cmd, uint8_t* pbuf, uint16_t length)
{
    (void) length;
    static uint8_t tempbuf[7];

    switch(cmd)
    {
        case CDC_SEND_ENCAPSULATED_COMMAND: break;
        case CDC_GET_ENCAPSULATED_RESPONSE: break;
        case CDC_SET_COMM_FEATURE: break;
        case CDC_GET_COMM_FEATURE: break;

        case CDC_CLEAR_COMM_FEATURE: break;
        /*******************************************************************************/
        /* Line Coding Structure                                                       */
        /*-----------------------------------------------------------------------------*/
        /* Offset | Field       | Size | Value  | Description                          */
        /* 0      | dwDTERate   |   4  | Number |Data terminal rate, in bits per second*/
        /* 4      | bCharFormat |   1  | Number | Stop bits                            */
        /*                                        0 - 1 Stop bit                       */
        /*                                        1 - 1.5 Stop bits                    */
        /*                                        2 - 2 Stop bits                      */
        /* 5      | bParityType |  1   | Number | Parity                               */
        /*                                        0 - None                             */
        /*                                        1 - Odd                              */
        /*                                        2 - Even                             */
        /*                                        3 - Mark                             */
        /*                                        4 - Space                            */
        /* 6      | bDataBits  |   1   | Number Data bits (5, 6, 7, 8 or 16).          */
        /*******************************************************************************/
        case CDC_SET_LINE_CODING:
            tempbuf[0] = pbuf[0];
            tempbuf[1] = pbuf[1];
            tempbuf[2] = pbuf[2];
            tempbuf[3] = pbuf[3];
            tempbuf[4] = pbuf[4];
            tempbuf[5] = pbuf[5];
            tempbuf[6] = pbuf[6];
            // uint32_t speed = (uint32_t)(pbuf[0] | (pbuf[1] << 8) | (pbuf[2] << 16) | (pbuf[3] << 24));
            break;

        case CDC_GET_LINE_CODING:
            pbuf[0] = tempbuf[0];
            pbuf[1] = tempbuf[1];
            pbuf[2] = tempbuf[2];
            pbuf[3] = tempbuf[3];
            pbuf[4] = tempbuf[4];
            pbuf[5] = tempbuf[5];
            pbuf[6] = tempbuf[6];
            break;

        case CDC_SET_CONTROL_LINE_STATE: break;
        case CDC_SEND_BREAK:break;
        default:  break;
    }

    return (USBD_OK);
}

static int8_t CDC_Receive_FS(uint8_t* Buf, uint32_t *Len)
{
    (void) Buf;

    s_rx_data_length += *Len;

    USBD_CDC_SetRxBuffer(&s_usb_device_fs_handler, &s_user_rx_buffer[s_rx_data_length]);
    USBD_CDC_ReceivePacket(&s_usb_device_fs_handler);

    system_freertos_semaphore_give(s_new_usb_data);

    return USBD_OK;
}

uint8_t CDC_Transmit_FS(uint8_t* Buf, uint16_t Len)
{
    uint8_t result = USBD_OK;
    USBD_CDC_HandleTypeDef *hcdc = (USBD_CDC_HandleTypeDef*)s_usb_device_fs_handler.pClassData;

    if (hcdc->TxState == 0)
    {
        USBD_CDC_SetTxBuffer(&s_usb_device_fs_handler, Buf, Len);
        result = USBD_CDC_TransmitPacket(&s_usb_device_fs_handler);

        if (system_freertos_semaphore_take(s_wait_complete_transaction, 100))
        {
            return result;
        }
        system_freertos_semaphore_give(s_wait_complete_transaction);

        return USBD_FAIL;
    }

    return USBD_BUSY;
}

static int8_t CDC_TransmitCplt_FS(uint8_t* Buf, uint32_t *Len, uint8_t epnum)
{
    (void) Buf;
    (void) Len;
    (void) epnum;

    uint8_t result = USBD_OK;
    system_freertos_semaphore_give(s_wait_complete_transaction);

    if (s_finish_tx != nullptr)
    {
        s_finish_tx(s_tx_param);
    }
    return result;
}

static USBD_CDC_ItfTypeDef USBD_Interface_fops_FS =
{
    CDC_Init_FS,
    CDC_DeInit_FS,
    CDC_Control_FS,
    CDC_Receive_FS,
    CDC_TransmitCplt_FS
};

static void s_process_rx_data(void)
{
    for (int i = 0; i < USB_CALLBACK_TYPES; i++)
    {
        USBCofig& config = s_config[i];
        if (config.enable && config.cb != nullptr)
        {
            if (config.cb(s_user_rx_buffer, s_rx_data_length, config.param))
            {
                return;
            }
        }
    }

    s_reset_usb_rx();  // restart RX
}


static void s_usb_app_rx_task(void*)
{
    uint32_t delay_time = USB_RX_TASK_DELAY_MS;

    assert(USBD_Init(&s_usb_device_fs_handler, &FS_Desc, DEVICE_FS) == USBD_OK);
    assert(USBD_RegisterClass(&s_usb_device_fs_handler, &USBD_CDC) == USBD_OK);
    assert(USBD_CDC_RegisterInterface(&s_usb_device_fs_handler, &USBD_Interface_fops_FS) == USBD_OK);
    assert(USBD_Start(&s_usb_device_fs_handler) == USBD_OK);

    while (true)
    {
        // Wait for data from USB
        if (xSemaphoreTake(s_new_usb_data, (TickType_t) delay_time) == pdTRUE)
        {
            if (s_rx_data_length < 64)
            {
                s_process_rx_data();
            }
            else
            {
                delay_time = 2;
            }
        }
        else
        {
            if (s_rx_data_length >= 64)
            {
                delay_time = USB_RX_TASK_DELAY_MS;
                s_process_rx_data();
            }
        }
    }
}

void hal_usb_init()
{
    static StaticTask_t s_task_buffer;
    static StackType_t s_stack[SIZE_USB_TASK];

    s_task_handler = xTaskCreateStatic(s_usb_app_rx_task, "USB Rx", SIZE_USB_TASK,
        0, PRI_USB_TASK, s_stack, &s_task_buffer);

    static StaticSemaphore_t s_semaphore_tx;
    s_wait_complete_transaction = xSemaphoreCreateBinaryStatic(&s_semaphore_tx);
    static StaticSemaphore_t s_semaphore_rx;
    s_new_usb_data = xSemaphoreCreateBinaryStatic(&s_semaphore_rx);
}

bool hal_usb_transmitte(const uint8_t* data, uint16_t size, FinishTransmiteCallback cb, void* param)
{
    bool ret = false;
    if (s_busy == false)
    {
        s_busy = true;
        s_finish_tx = cb;
        s_tx_param = param;
        ret = CDC_Transmit_FS((uint8_t*)data, size) == USBD_OK;
        s_busy = false;
    }
    return ret;
}

bool hal_usb_reception_add_handler(uint8_t id, USBDataCb cb, void* param)
{
    assert(id < USB_CALLBACK_TYPES);
    USBCofig& config = s_config[id];

    config.cb = cb;
    config.param = param;
    config.enable = true;

    return true;
}