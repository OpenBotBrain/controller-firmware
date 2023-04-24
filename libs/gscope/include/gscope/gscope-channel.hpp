// Copyright © 2021-2023 Santiago Goldaraz. All rights reserved.
#pragma once

#include <gscope/gscope-types.hpp>

struct GScopeChannelData
{
    constexpr GScopeChannelData(const char* name, uint8_t type, uint16_t size_byte,
        uint8_t type_size, uint16_t& channel_id) : m_ch_name(name), m_data_type(type),
        m_type_size(type_size), m_data_size_bytes(size_byte), m_channel_id(channel_id) {}

    const char* m_ch_name;                  // Channel name
    const uint8_t m_data_type;              // Channel data type
    const uint8_t m_type_size;              // Size of the datatypes in byte
    uint16_t m_data_size_bytes;             // Channel data size in bytes
    uint16_t& m_channel_id;                 // Unix channel id, it gets complete on registration
};

template <typename T>
struct GScopeChannelDescriptor : public GScopeChannelData
{
    constexpr GScopeChannelDescriptor(const char* channel_name, uint8_t size, uint16_t& channel_id) :
        GScopeChannelData(channel_name,
            GScopeTypes::arg_to_type(static_cast<T>(0)),
            size * sizeof(T), sizeof(T), channel_id) {}

    bool produce(const T* data) const;
    bool print(const char* p_string, ...) const;
};

class GScopeChannelRegister
{
    public:
        GScopeChannelRegister(const GScopeChannelData& channel_data);

        const GScopeChannelData& m_channel_data;
        GScopeChannelRegister* m_next_register_channel;
};

#define GScopeChannel(variable_name, channel_name, type, size) \
    static uint16_t s_gsvar_##variable_name##_id; \
    static constexpr GScopeChannelDescriptor<type> variable_name(channel_name, size, s_gsvar_##variable_name##_id); \
    static GScopeChannelRegister s_gsvar_##variable_name##_reg(variable_name);
