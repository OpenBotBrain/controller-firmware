#include <actuator/lego-motor.hpp>

// ------------------------------------------------------------------------------------
//                                      PUBLIC API
// ------------------------------------------------------------------------------------

Lego_Motor_Type get_lego_motor_type(OutputPort* port)
{
    (void) port;

    return UNDEFINED_LEGO_MOTOR;
}
