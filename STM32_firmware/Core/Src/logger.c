#include "logger.h"
#include "system_data.h"
#include "main.h"
#include "aqi.h"   // AQI_GetTrend()
#include <stdio.h>
#include <string.h>

extern UART_HandleTypeDef huart2;

static const char *StateToString(AirState_t state)
{
	switch (state)
	{
		case AIR_STATE_GOOD:		return "GOOD";
		case AIR_STATE_MODERATE:	return "MODERATE";
		case AIR_STATE_WARNING: 	return "WARNING";
		case AIR_STATE_DANGEROUS:	return "DANGEROUS";
		default:					return "UNKNOWN";
	}
}

/* ============================= */
/* AQI MODULE ADDITION*/
/* This section was added to display AQI trend information */
/* ============================= */

static const char *TrendToString(AQITrend_t t)
{
	switch (t)
	{
		case AQI_TREND_IMPROVING: return "IMPROVING";
		case AQI_TREND_WORSENING: return "WORSENING";
		default: return "STABLE";
	}
}

/* ============================= */

void Logger_Init(void)
{
}

void Logger_Report(void)
{
	char buf[160];

	int len = snprintf(
		buf,
		sizeof(buf),
		"T=%.1f H=%.1f P=%.1f PM25=%.1f AQI=%u STATE=%s ALARM=%u TREND=%s\r\n",
		gWeather.temperature,
		gWeather.humidity,
		gWeather.pressure,
		gWeather.filtered_pm25,
		gWeather.aqi,
		StateToString(gWeather.air_state),
		gWeather.alarm,
		TrendToString(AQI_GetTrend())   /* AQI MODULE ADDITION */
	);

	if(len > 0)
	{
		HAL_UART_Transmit(&huart2, (uint8_t*)buf, (uint16_t)len, HAL_MAX_DELAY);
	}
}