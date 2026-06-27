#include "logger.h"
#include "system_data.h"
#include "alarm.h"
#include "history.h"
#include "fault.h"
#include "predictor.h"
#include "main.h"

#include <stdio.h>
#include <stdarg.h>
#include <string.h>

extern UART_HandleTypeDef huart2;

#define LOGGER_BUFFER_SIZE   768U

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


static const char* TrendToString(TrendState_t trend)
{
	switch (trend)
	{
	case TREND_IMPROVING: return "IMPROVING";
	case TREND_WORSENING: return "WORSENING";
	case TREND_STABLE:
	default:              return "STABLE";
	}
}


static const char* AlarmReasonToString(AlarmReason_t reason)
{
	switch (reason)
	{
	case ALARM_REASON_AQI:  return "AQI";
	case ALARM_REASON_CO2:  return "CO2";
	case ALARM_REASON_BOTH: return "BOTH";
	case ALARM_REASON_NONE:
	default:                return "NONE";
	}
}

static int Logger_Append(char* buf, size_t buf_size, size_t* offset, const char* fmt, ...)
{
    int written;
    va_list args;

    if ((buf == NULL) || (offset == NULL) || (fmt == NULL) || (*offset >= buf_size))
    {
        return -1;
    }

    va_start(args, fmt);
    written = vsnprintf(&buf[*offset], buf_size - *offset, fmt, args);
    va_end(args);

    if (written < 0)
    {
        return -1;
    }

    if ((size_t)written >= (buf_size - *offset))
    {
        *offset = buf_size - 1U;
        buf[*offset] = '\0';
        return written;
    }

    *offset += (size_t)written;
    return written;
}

static void Logger_AppendHeader(char* buf, size_t buf_size, size_t* offset)
{
    Logger_Append(buf, buf_size, offset,
        "================================\r\n");
    Logger_Append(buf, buf_size, offset,
        "AIR QUALITY REPORT\r\n");
    Logger_Append(buf, buf_size, offset,
        "================================\r\n");
}

static void Logger_AppendSensorSection(char* buf, size_t buf_size, size_t* offset)
{
    Logger_Append(buf, buf_size, offset, "\r\n");
    Logger_Append(buf, buf_size, offset, "TEMP          : %.1f C\r\n", gWeather.temperature);
    Logger_Append(buf, buf_size, offset, "HUMIDITY      : %.1f %%\r\n", gWeather.humidity);
    Logger_Append(buf, buf_size, offset, "PRESSURE      : %.1f hPa\r\n", gWeather.pressure);
    Logger_Append(buf, buf_size, offset, "PM2.5 RAW     : %.1f\r\n", gWeather.pm25);
    Logger_Append(buf, buf_size, offset, "PM2.5 FILTER  : %.1f\r\n", gWeather.filtered_pm25);
    Logger_Append(buf, buf_size, offset, "CO2           : %.1f\r\n", gWeather.co2);
    Logger_Append(buf, buf_size, offset, "AQI           : %u\r\n"  , gWeather.aqi);
		Logger_Append(buf, buf_size, offset,"ML PREDICT     : %.1f\r\n",gWeather.ml_prediction);
		Logger_Append(buf, buf_size, offset,"FORECAST AQI   : %u\r\n",gWeather.forecast_aqi);
    Logger_Append(buf, buf_size, offset, "FORECAST AQI  : %u\r\n", Predictor_GetForecastAQI());
    Logger_Append(buf, buf_size, offset, "TREND         : %s\r\n", TrendToString(gWeather.trend_state));
    Logger_Append(buf, buf_size, offset, "STATE         : %s\r\n", StateToString(gWeather.air_state));
}

static void Logger_AppendAlarmSection(char* buf, size_t buf_size, size_t* offset)
{
    Logger_Append(buf, buf_size, offset, "ALARM         : %s\r\n", gWeather.alarm ? "ON" : "OFF");
    Logger_Append(buf, buf_size, offset, "ALARM LEVEL   : %s\r\n", AlarmLevelToString(Alarm_GetLevel()));
    Logger_Append(buf, buf_size, offset, "ALARM REASON  : %s\r\n", AlarmReasonToString(Alarm_GetReason()));
    Logger_Append(buf, buf_size, offset, "ALARM COUNT   : %lu\r\n", (unsigned long)Alarm_GetCounter());
}

static void Logger_AppendFaultSection(char* buf, size_t buf_size, size_t* offset)
{
    Logger_Append(buf, buf_size, offset, "FAULT         : %s\r\n", Fault_HasFault() ? "YES" : "NO");
    Logger_Append(buf, buf_size, offset, "FAULT FLAGS   : 0x%08lX\r\n", (unsigned long)Fault_GetFlags());
}

static void Logger_AppendHistorySection(char* buf, size_t buf_size, size_t* offset)
{
    Logger_Append(buf, buf_size, offset, "SAMPLES       : %u\r\n", History_GetSampleCount());
    Logger_Append(buf, buf_size, offset, "AVG PM2.5     : %.1f\r\n", History_GetAvgPM25());
    Logger_Append(buf, buf_size, offset, "AVG CO2       : %.1f\r\n", History_GetAvgCO2());
    Logger_Append(buf, buf_size, offset, "MIN PM2.5     : %.1f\r\n", History_GetMinPM25());
    Logger_Append(buf, buf_size, offset, "MAX PM2.5     : %.1f\r\n", History_GetMaxPM25());
}

static void Logger_AppendFooter(char* buf, size_t buf_size, size_t* offset)
{
    Logger_Append(buf, buf_size, offset,
        "================================\r\n");
}

void Logger_Init(void)
{
}

void Logger_Report(void)
{
	char buf[LOGGER_BUFFER_SIZE];
	size_t offset = 0U;
	uint32_t tick = HAL_GetTick();

	memset(buf, 0, sizeof(buf));

	Logger_AppendHeader(buf, sizeof(buf), &offset);

	Logger_Append(buf, sizeof(buf), &offset, "TIME          : %lu ms\r\n", (unsigned long)tick);
	Logger_Append(buf, sizeof(buf), &offset, "SAMPLE ID     : %lu\r\n", (unsigned long)gWeather.sample_id);
	Logger_Append(buf,sizeof(buf),&offset,"TIMESTAMP     : %lu ms\r\n",(unsigned long)gWeather.timestamp_ms);
	Logger_AppendSensorSection(buf, sizeof(buf), &offset);
	Logger_AppendAlarmSection(buf, sizeof(buf), &offset);
	Logger_AppendFaultSection(buf, sizeof(buf), &offset);
	Logger_AppendHistorySection(buf, sizeof(buf), &offset);
	Logger_AppendFooter(buf, sizeof(buf), &offset);

	if (offset > 0U)
	{
		HAL_UART_Transmit(&huart2, (uint8_t*)buf, (uint16_t)strlen(buf), HAL_MAX_DELAY);
	}
}