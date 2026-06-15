#include "logger.h"
#include "system_data.h"
#include "main.h"
#include <stdio.h>
#include <string.h>

extern UART_HandleTypeDef huart2;

static const char *StateToString(AirState_t state)
{
    switch(state)
    {
        case AIR_STATE_GOOD:      return "GOOD";
        case AIR_STATE_MODERATE:  return "MODERATE";
        case AIR_STATE_WARNING:   return "WARNING";
        case AIR_STATE_DANGEROUS: return "DANGEROUS";
        default:                  return "UNKNOWN";
    }
}

void Logger_Init(void)
{
    /* Reserved for future initialization */
}

void Logger_Report(void)
{
    char buf[256];

    uint32_t tick = HAL_GetTick();

    int len = snprintf(
        buf,
        sizeof(buf),
        "[%u ms] T=%.1f H=%.1f P=%.1f PM25=%.1f CO2=%.1f AQI=%u STATE=%s ALARM=%s\r\n",
        tick,
        gWeather.temperature,
        gWeather.humidity,
        gWeather.pressure,
        gWeather.filtered_pm25,
        gWeather.co2,
        gWeather.aqi,
        StateToString(gWeather.air_state),
        gWeather.alarm ? "ON" : "OFF"
    );

    if(len > 0)
    {
        HAL_UART_Transmit(
            &huart2,
            (uint8_t*)buf,
            (uint16_t)len,
            HAL_MAX_DELAY
        );
    }
}
