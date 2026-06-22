#include "fault.h"
#include "system_data.h"

static uint32_t faultFlags = FAULT_NONE;

void Fault_Init(void)
{
    faultFlags = FAULT_NONE;
    gWeather.fault_flags = FAULT_NONE;
}

void Fault_Update(void)
{
    faultFlags = FAULT_NONE;

    /* PM2.5 Spike Detection */
    if(gWeather.pm25 > 250.0f)
    {
        faultFlags |= FAULT_PM25_SPIKE;
    }

    /* Invalid CO2 Value */
    if((gWeather.co2 < 0.0f) || (gWeather.co2 > 5000.0f))
    {
        faultFlags |= FAULT_CO2_INVALID;
    }

    /* General Sensor Error */
    if((gWeather.pm25 < 0.0f) || (gWeather.co2 < 0.0f))
    {
        faultFlags |= FAULT_SENSOR_ERROR;
    }

    gWeather.fault_flags = faultFlags;
}

uint32_t Fault_GetFlags(void)
{
    return faultFlags;
}

uint8_t Fault_HasFault(void)
{
    return (faultFlags != FAULT_NONE);
}