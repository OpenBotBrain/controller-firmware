#include <sensor/lego-sensor.hpp>

// ------------------------------------------------------------------------------------
//                                      PUBLIC API
// ------------------------------------------------------------------------------------

Lego_Sensor_Type get_lego_sensor_type(InputPort *port)
{
    (void) port;

    return UNDEFINED_LEGO_SENSOR;
}
