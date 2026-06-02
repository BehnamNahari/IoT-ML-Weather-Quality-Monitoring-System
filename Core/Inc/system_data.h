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
	} WeatherData_t;
	
	extern WeatherData_t gWeather;
	
#endif