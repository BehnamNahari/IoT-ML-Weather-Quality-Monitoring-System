#include "aqi.h"
#include "system_data.h"
#include "app_config.h"

static uint16_t ComputeAQI(float pm25, AirState_t *state)
{
	if(pm25 <= 12.0f)
	{
		*state = AIR_STATE_GOOD;
		return 25;
	} 
	else if(pm25 <= 35.4f)
	{
		*state = AIR_STATE_MODERATE;
		return 75;
	}
	else if ( pm25 <= 55.4f)
	{
		*state = AIR_STATE_WARNING;
		return 125;
	} 
	else 
	{
		*state = AIR_STATE_DANGEROUS;
		return 250;
	}
}

void AQI_Init(void)
{
	gWeather.aqi = 0;
	gWeather.air_state = AIR_STATE_GOOD;
}

void AQI_Update(void)
{
	gWeather.aqi = ComputeAQI(gWeather.filtered_pm25, &gWeather.air_state);
	if(gWeather.co2 >= CO2_DANGER_THRESHOLD)
	{
		gWeather.air_state = AIR_STATE_DANGEROUS;
		if(gWeather.aqi < 200)
		{
			gWeather.aqi = 200;
		}
	} 
	else if(gWeather.co2 >= CO2_WARNING_THRESHOLD && gWeather.air_state < AIR_STATE_WARNING) 
	{
		gWeather.air_state = AIR_STATE_WARNING;
		if(gWeather.aqi < 120)
		{
			gWeather.aqi = 120;
		}
	}
}
	
