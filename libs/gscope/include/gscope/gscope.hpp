// Copyright © 2021-2023 Santiago Goldaraz. All rights reserved.

#pragma once

#include <gscope/gscope-button.hpp>
#include <gscope/gscope-channel.hpp>
#include <gscope/gscope-command.hpp>
#include <gscope/gscope-debug.hpp>

class GScope
{
    public:
        using handler = void(*)(void);
        using lock_handler = bool(*)(void);
        using send_handler = bool(*)(const uint8_t*, uint32_t);
        struct SerialConfig
        {
            lock_handler buffer_lock;           // Callback to lock the mutex
            handler buffer_unlock;              // Callback to unlock the mutex
            send_handler send_data;             // Callback to send data
            uint8_t* tx_buf;                    // Pointer to main buffer
            const uint32_t ring_buffer_size;    // size of the buffer
        };

        GScope(const SerialConfig& serial_config, handler on_connect_cb);

        // Public API
        bool update(void);
        bool incoming(const uint8_t* data, uint32_t size);
        bool incoming_fragmented(const uint8_t* data, int size, uint8_t* buf, int buf_size);
        void enable_transmission(bool enable);
        bool is_enabled();

        // Only used by Command and Channels - DO NOT USE EXTERNALLY
        bool serialization_channel(uint16_t idx, const uint8_t* data, uint32_t size);

    private:
        const SerialConfig& m_serial_config;
        const handler m_on_connect_cb;
        bool m_enabled {false};
        uint32_t m_fragmented_offset {0};
        uint16_t m_send_manifest_channel_offset {0xffff};
        uint16_t m_send_manifest_button_offset {0xffff};

        bool incoming_control_command(const uint8_t* data, uint32_t size);
        bool send_manifest_general_channel_info();
        bool send_manifest_channel_info(uint16_t channel_id);
        bool send_manifest_general_button_info();
        bool send_manifest_button_info(uint16_t button_id);
        bool send(uint8_t fun, const uint8_t* data, uint16_t size,
            const uint8_t* extra_data = nullptr, uint16_t extra_size = 0);

        // Local buffer control
        uint32_t m_head {0};
        uint32_t m_tail {0};
        uint32_t m_intermediate_tail {0};

        bool buffer_lock(uint32_t size);
        void buffer_unlock(void);
        void buffer_write(const uint8_t* data, uint32_t size);
        uint32_t buffer_update();
        int32_t buffer_get_byte_to_send();
        void buffer_clean();
};
