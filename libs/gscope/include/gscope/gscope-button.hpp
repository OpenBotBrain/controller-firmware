// Copyright © 2023 Santiago Goldaraz. All rights reserved.
#pragma once

#include <gscope/gscope-types.hpp>

struct GScopeButtonDescriptor
{
    constexpr GScopeButtonDescriptor(const char* name, const GScopeTypes::CB_None cb) :
        m_button_name(name), m_name_size(GScopeTypes::strlen(name)), m_cb(cb) {}

    const char* m_button_name;
    const uint8_t m_name_size;
    const GScopeTypes::CB_None m_cb;
};

struct GScopeButtonRegister
{
    public:
        GScopeButtonRegister(const GScopeButtonDescriptor& reg);
        const GScopeButtonDescriptor* m_button;
        const GScopeButtonRegister* m_next_registration;
};

#define GScopeButton(button_name, callback) \
    static constexpr GScopeButtonDescriptor s_gsvar_but_##callback##_descriptor(button_name, callback); \
    static GScopeButtonRegister s_gsvar_but_##callback(s_gsvar_but_##callback##_descriptor);
