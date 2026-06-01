#include "alarm.h"
#include "system_data.h"
#include "app_config.h"
#include "mian.h"

void Alarm_Init(void)
{
	gWeather.alarm = 0;
	HAL_GPIO_WritePin(ALARM_LED_GPIO_Port, ALARM_LED_Pin,GPIO_PIN_SET);
}

void Alarm_Update(void)
{
	if(gWeather.aqi >= 175 || gWeather.co2 >= CO2_DANGER_THRESHOLD)
	{
		gWeather.alarm = 1;
		HAL_GPIO_WritePin(ALARM_LED_GPIO_Port, ALARM_LED_Pin,GPIO_PIN_RESET);
	} else
	{
		gWeather.alarm = 0;
		HAL_GPIO_WritePin(ALARM_LED_GPIO_Port, ALARM_LED_Pin,GPIO_PIN_SET);
	}
}

uint8_t Alarm_IsActive(void)
{
	return gWeather.alarm;
}
