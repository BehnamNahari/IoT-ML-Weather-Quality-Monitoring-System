#include "pressure.h"

static float pressure = 1013.25f;

void Pressure_Init(void)
{
    pressure = 1013.25f;
}

float Pressure_Read(void)
{
    pressure += 0.10f;

    if(pressure > 1015.0f)
        pressure = 1013.25f;

    return pressure;
}