// Copyright © 2021-2023 Santiago Goldaraz. All rights reserved.
#pragma once

#include <gscope/gscope-types.hpp>

struct GScopeCommandDescriptor
{
    constexpr GScopeCommandDescriptor(const char* name, const GScopeTypes::Callback& cb) :
        m_cmd_name(name), m_name_size(GScopeTypes::strlen(name)), m_cb(cb) {}

    const char* m_cmd_name;
    const uint8_t m_name_size;
    const GScopeTypes::Callback m_cb;
};

struct GScopeCommandRegister
{
    public:
        GScopeCommandRegister(const GScopeCommandDescriptor& reg);
        const GScopeCommandDescriptor* m_cmd;
        const GScopeCommandRegister* m_next_registration;
};

#define GScopeCommand(command_name, callback) \
    static constexpr GScopeCommandDescriptor s_gsvar_##callback##_descriptor(command_name, callback); \
    static GScopeCommandRegister s_gsvar_##callback(s_gsvar_##callback##_descriptor);
