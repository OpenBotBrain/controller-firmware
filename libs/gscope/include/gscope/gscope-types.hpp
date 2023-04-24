// Copyright © 2021-2023 Santiago Goldaraz. All rights reserved.
#pragma once

#include <cstdint>

enum class GSColor : uint8_t
{
    BLACK = 0,
    MAROON,
    BROWN,
    OLIVE,
    TEAL,
    NAVY,
    RED,
    ORANGE,
    YELLOW,
    LIME,
    GREEN,
    CYAN,
    BLUE,
    PURPLE,
    MAGENTA,
    GRAY,
    PINK,
    APRICOT,
    MINT,
    LAVENDER
};

namespace GScopeTypes
{
    static constexpr uint16_t CHANNEL_ENABLE = (1 << 15);

    using CB_None = void(*)(void);
    using CB_Raw = void(*)(uint8_t*, int);
    using CB_1_Int = void(*)(int);
    using CB_2_Int = void(*)(int, int);
    using CB_3_Int = void(*)(int, int, int);
    using CB_1_Float = void(*)(float);
    using CB_2_Float = void(*)(float, float);
    using CB_3_Float = void(*)(float, float, float);
    using CB_1_Int_1_Float = void(*)(int, float);
    using CB_Bool = void(*)(bool);

    enum ArgumentType : uint8_t
    {
        ARG_NONE_DATA = 0,
        ARG_RAW_DATA,
        ARG_1_INT,
        ARG_2_INT,
        ARG_3_INT,
        ARG_1_FLOAT,
        ARG_2_FLOAT,
        ARG_3_FLOAT,
        ARG_1_INT_1_FLOAT,
        ARG_1_BOOL,
    };

    static constexpr ArgumentType type_to_arg(CB_None) { return ArgumentType::ARG_NONE_DATA; }
    static constexpr ArgumentType type_to_arg(CB_Raw) { return ArgumentType::ARG_RAW_DATA; }
    static constexpr ArgumentType type_to_arg(CB_1_Int) { return ArgumentType::ARG_1_INT; }
    static constexpr ArgumentType type_to_arg(CB_2_Int) { return ArgumentType::ARG_2_INT; }
    static constexpr ArgumentType type_to_arg(CB_3_Int) { return ArgumentType::ARG_3_INT; }
    static constexpr ArgumentType type_to_arg(CB_1_Float) { return ArgumentType::ARG_1_FLOAT; }
    static constexpr ArgumentType type_to_arg(CB_2_Float) { return ArgumentType::ARG_2_FLOAT; }
    static constexpr ArgumentType type_to_arg(CB_3_Float) { return ArgumentType::ARG_3_FLOAT; }
    static constexpr ArgumentType type_to_arg(CB_1_Int_1_Float) { return ArgumentType::ARG_1_INT_1_FLOAT; }
    static constexpr ArgumentType type_to_arg(CB_Bool) { return ArgumentType::ARG_1_BOOL; }

    static constexpr uint8_t strlen(const char* str) { return *str ? 1 + strlen(str + 1) : 0; }

    class Callback
    {
        public:
            union CallbackTypes
            {
                constexpr CallbackTypes(CB_None cb) : m_none(cb) {}
                constexpr CallbackTypes(CB_Raw cb) : m_raw(cb) {}
                constexpr CallbackTypes(CB_1_Int cb) : m_one_int(cb) {}
                constexpr CallbackTypes(CB_2_Int cb) : m_two_int(cb) {}
                constexpr CallbackTypes(CB_3_Int cb) : m_three_int(cb) {}
                constexpr CallbackTypes(CB_1_Float cb) : m_one_float(cb) {}
                constexpr CallbackTypes(CB_2_Float cb) : m_two_float(cb) {}
                constexpr CallbackTypes(CB_3_Float cb) : m_three_float(cb) {}
                constexpr CallbackTypes(CB_1_Int_1_Float cb) : m_one_int_one_flaot(cb) {}
                constexpr CallbackTypes(CB_Bool cb) : m_bool(cb) {}

                const CB_None m_none;
                const CB_Raw m_raw;
                const CB_1_Int m_one_int;
                const CB_2_Int m_two_int;
                const CB_3_Int m_three_int;
                const CB_1_Float m_one_float;
                const CB_2_Float m_two_float;
                const CB_3_Float m_three_float;
                const CB_1_Int_1_Float m_one_int_one_flaot;
                const CB_Bool m_bool;
            };

            template<class T>
            constexpr Callback(const T& cb) :
                m_type(GScopeTypes::type_to_arg(cb)), m_callback(cb) {}

            const char* get_type_description() const
            {
                switch (m_type)
                {
                    case ARG_NONE_DATA: return "";
                    case ARG_RAW_DATA: return "R";
                    case ARG_1_INT: return "I";
                    case ARG_2_INT: return "II";
                    case ARG_3_INT: return "III";
                    case ARG_1_FLOAT: return "F";
                    case ARG_2_FLOAT: return "FF";
                    case ARG_3_FLOAT: return "FFF";
                    case ARG_1_INT_1_FLOAT: return "IF";
                    case ARG_1_BOOL: return "B";
                }
                return "?";
            }

            const uint8_t m_type;
            const CallbackTypes m_callback;
    };

    enum DataType : uint8_t
    {
        CHARS = 0,
        UINT8,
        INT8,
        UINT16,
        INT16,
        UINT32,
        INT32,
        UINT64,
        INT64,
        FLOAT,
        DOUBLE,
        STRING
    };

    static constexpr DataType arg_to_type(char) { return DataType::STRING; }
    static constexpr DataType arg_to_type(uint8_t) { return DataType::UINT8; }
    static constexpr DataType arg_to_type(int8_t) { return DataType::INT8; }
    static constexpr DataType arg_to_type(uint16_t) { return DataType::UINT16; }
    static constexpr DataType arg_to_type(int16_t) { return DataType::INT16; }
    static constexpr DataType arg_to_type(uint32_t) { return DataType::UINT32; }
    static constexpr DataType arg_to_type(int32_t) { return DataType::INT32; }
    static constexpr DataType arg_to_type(uint64_t) { return DataType::UINT64; }
    static constexpr DataType arg_to_type(int64_t) { return DataType::INT64; }
    static constexpr DataType arg_to_type(float) { return DataType::FLOAT; }
    static constexpr DataType arg_to_type(double) { return DataType::DOUBLE; }
}
