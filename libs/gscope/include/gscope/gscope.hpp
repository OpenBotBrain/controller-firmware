// Copyright © 2021-2023 Santiago Goldaraz. All rights reserved.
#pragma once

#include <gscope/gscope-buffer.hpp>
#include <gscope/gscope-button.hpp>
#include <gscope/gscope-channel.hpp>
#include <gscope/gscope-command.hpp>
#include <gscope/gscope-debug.hpp>

class GScope
{
    public:
        using handler = void(*)(void);

        GScope(const GScopeSerialBuffer::SerialConfig& serial_config, handler on_connect_cb);

        // Public API
        bool update(void);
        bool incoming(const uint8_t* data, uint32_t size);
        bool incoming_fragmented(const uint8_t* data, int size, uint8_t* buf, int buf_size);
        void enable_transmission(bool enable);
        bool is_enabled();

        // Only used by Command and Channels - DO NOT USE ON API
        bool serialization_console(const uint8_t* data, uint32_t size, GSColor color);
        bool serialization_channel(uint16_t idx, const uint8_t* data, uint32_t size);

    private:
        GScopeSerialBuffer m_buffer;
        const handler m_on_connect_cb;
        bool m_enabled;
        uint32_t m_fragmented_offset {0};

        bool incoming_control_command(const uint8_t* data, uint32_t size);
        bool send_manifest_version();
        bool send_manifest_general_channel_info();
        bool send_manifest_channel_info(const uint8_t* data);
        bool send_manifest_general_button_info();
        bool send_manifest_button_info(const uint8_t* data);
};
