#include "filter.h"
#include "system_data.h"

static float p25_ema = 0.0f;
static uint8_t first_run = 1;

void Filter_Init(void)
{
	p25_ema = 0.0f;
	first_run = 1;
	}
	
	void Filter_Update(void)
	{
		if(first_run)
		{
			p25_ema = gWeather.p25;
			first_run = 0;
		}
		else
		{
			p25_ema =0.7f *p25_ema + 0.3f * gWeather.p25;
			}
		gWeather.filtered_pm25 = pm25_ema;
	}