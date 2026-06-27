#include "history.h"
#include "system_data.h"

static HistorySample_t history_buffer[HISTORY_SIZE];

static uint16_t write_index = 0;
static uint16_t sample_count = 0;

void History_Init(void)
{
	write_index=0;
	sample_count=0;
}
void History_PushSample(void)
{
    history_buffer[write_index].pm25 = gWeather.filtered_pm25;
    history_buffer[write_index].co2 = gWeather.co2;
    history_buffer[write_index].aqi = gWeather.aqi;

    write_index++;

    if(write_index >= HISTORY_SIZE)
    {
        write_index = 0;
    }

    if(sample_count < HISTORY_SIZE)
    {
        sample_count++;
    }
}

float History_GetAvgCO2(void)
{
if(sample_count == 0)
    {
        return 0.0f;
    }

    float sum = 0.0f;

    for(uint16_t i = 0; i < sample_count; i++)
    {
        sum += history_buffer[i].co2;
    }

    return sum / sample_count;
}

float History_GetMinPM25(void)
{
	if(sample_count == 0)
    {
        return 0.0f;
    }

    float min = history_buffer[0].pm25;

    for(uint16_t i = 1; i < sample_count; i++)
    {
        if(history_buffer[i].pm25 < min)
        {
            min = history_buffer[i].pm25;
        }
    }

    return min;
}

float History_GetMaxPM25(void)
{
	 if(sample_count == 0)
    {
        return 0.0f;
    }

    float max = history_buffer[0].pm25;

    for(uint16_t i = 1; i < sample_count; i++)
    {
        if(history_buffer[i].pm25 > max)
        {
            max = history_buffer[i].pm25;
        }
    }

    return max;
}
float History_GetAvgPM25(void)
{
 if(sample_count == 0)
    {
        return 0.0f;
    }

    float sum = 0.0f;

    for(uint16_t i = 0; i < sample_count; i++)
    {
        sum += history_buffer[i].pm25;
    }

    return sum / sample_count;
}

uint16_t History_GetSampleCount(void)
{
	return sample_count;
}