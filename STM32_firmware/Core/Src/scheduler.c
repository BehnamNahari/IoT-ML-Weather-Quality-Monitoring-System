#include "scheduler.h"
#include "app_config.h"
#include "mock_sensor.h"
#include "filter.h"
#include "aqi.h"
#include "alarm.h"
#include "logger.h"
#include "stm32f1xx_hal.h"
#include "history.h"
#include "predictor.h"
#include "cli.h"
#include "fault.h"

void Scheduler_Init(void)
{
	History_Init();
	Predictor_Init();
	Fault_Init();
	CLI_Init();
}

void Scheduler_Run(void)
{
	
	
	
	Fault_Update();
	CLI_Process();
	
	
	
	static uint32_t last_sensor_time = 0;
	static uint32_t last_log_time = 0;
	
	uint32_t now = HAL_GetTick();
	gWeather.timestamp_ms = now;
	
	if((now - last_sensor_time) >= SENSOR_PERIOD_MS)
	{
		MockSensor_Update();
		gWeather.sample_id++;
		Filter_Update();
		AQI_Update();
		History_PushSample();
		Predictor_Update();
		Alarm_Update();
		last_sensor_time = now;
	}
	if((now - last_log_time) >= LOG_PERIOD_MS)
	{
		Logger_Report();
		last_log_time = now;
	}
}