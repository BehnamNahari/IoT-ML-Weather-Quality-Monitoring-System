#include "mock_sensor.h"
#include "system_data.h"
#include "stm32f1xx_hal.h"

static uint32_t seed = 0x12345678u;

static uint32_t NextRand(void)
{
	seed = 1664525u * seed + 1013904223u;
	return seed;
}

void MockSensor_Init(void){
	seed ^= HAL_GetTick();
	}
	
void MockSensor_Update(void)
{
	gWeather.temperature = 20.0f  + (NextRand() % 1000u)   / 100.0f;
	gWeather.humidity   = 45.0f  + (NextRand() % 3000u)   / 100.0f;
	gWeather.pressure   = 990.0f + (NextRand() % 4000u)   / 100.0f;
	gWeather.pm25       = 5.0f   + (NextRand() % 20000u)  / 100.0f;
	gWeather.co2        = 400.0f + (NextRand() % 150000u) / 100.0f;
	}