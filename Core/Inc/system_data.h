#ifndef SYSTEM_DATA_H
#define SYSTEM_DATA_H

#include <stdint.h>

typedef enum
{
	AIR_STATE_GOOD = 0,
	AIR_STATE_MODERATE,
	AIR_STATE_WARNING,
	AIR_STATE_DANGEROUS
	} AirState_t;
	
	typedef enum{
	SYSTEM_MODE_AUTO = 0,
	SYSTEM_MODE_MANUAL
}SystemMOde_t;

	typedef enum
	{
	TREND_STABLE = 0,
	TREND_IMPROVING,
	TREND_WORSENING
}TrendState_t;
	
typedef struct
{
	float temperature;
	float humidity;
	float pressure;
	
	float pm25;
	float co2;
	
	float filtered_pm25;
	
	uint16_t aqi;
	AirState_t air_state;
	
	uint8_t alarm;
	
	uint32_t sample_id;
	uint32_t timestamp_ms;
	uint16_t forecast_aqi;
	
	TrendState_t trend_state;
	uint32_t fault_flags;
	
	SystemMOde_t system_mode;
	
	} WeatherData_t;


	extern WeatherData_t gWeather;
	
#endif