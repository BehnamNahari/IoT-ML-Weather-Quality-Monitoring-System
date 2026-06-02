#include "filter.h"
#include "system_data.h"

static float pm25_ema = 0.0f;
static uint8_t first_run = 1;

void Filter_Init(void)
{
	pm25_ema = 0.0f;
	first_run = 1;
	}
	
	void Filter_Update(void)
	{
		if(first_run)
		{
			pm25_ema = gWeather.pm25;
			first_run = 0;
		}
		else
		{
			pm25_ema =0.7f *pm25_ema + 0.3f * gWeather.pm25;
			}
		gWeather.filtered_pm25 = pm25_ema;
	}