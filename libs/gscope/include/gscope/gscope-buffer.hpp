// Copyright © 2021-2023 Santiago Goldaraz. All rights reserved.
#pragma once

#include <gscope/gscope-types.hpp>

class GScopeSerialBuffer
{
    public:
        using lock_handler = bool(*)(void);
        using send_handler = bool(*)(const uint8_t*, uint32_t);
        using handler = void(*)(void);
        typedef struct
        {
            lock_handler buffer_lock;           // Callback to lock the mutex
            handler buffer_unlock;              // Callback to unlock the mutex
            send_handler send_data;             // Callback to send data
            uint8_t* tx_buf;                    // Pointer to main buffer
            const uint32_t ring_buffer_size;    // size of the buffer
        } SerialConfig;

        GScopeSerialBuffer(const SerialConfig& serial_config);

        bool lock_buffer(uint32_t size);
        void unlock_buffer(void);
        void buffer_write(const uint8_t* data, uint32_t size);
        uint32_t update(void);
        int32_t get_byte_to_send(void);

    private:
        const SerialConfig& m_serial_config;

        uint32_t m_head {0};
        uint32_t m_tail {0};
        uint32_t m_intermediate_tail {0};
};
