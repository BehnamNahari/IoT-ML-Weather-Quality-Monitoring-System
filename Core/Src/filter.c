#include "filter.h"
#include "system_data.h"
#include <math.h>

/* Configuration */
#define PM25_SPIKE_THRESHOLD     150.0f
#define MOVING_AVG_WINDOW_SIZE   5

/* EMA Variables */
static float pm25_ema = 0.0f;
static uint8_t first_run = 1;

/* Moving Average Variables */
static float samples[MOVING_AVG_WINDOW_SIZE];
static uint8_t sample_index = 0;
static uint8_t sample_count = 0;
static float moving_average = 0.0f;

/* Statistics */
static uint32_t rejected_samples = 0;
static FilterStatus_t filter_status = FILTER_DATA_VALID;

/* Min / Max Tracking */
static float min_pm25 = 0.0f;
static float max_pm25 = 0.0f;

void Filter_Init(void)
{
    uint8_t i;

    pm25_ema = 0.0f;
    first_run = 1;

    sample_index = 0;
    sample_count = 0;
    moving_average = 0.0f;

    rejected_samples = 0;
    filter_status = FILTER_DATA_VALID;

    min_pm25 = 0.0f;
    max_pm25 = 0.0f;

    for(i = 0; i < MOVING_AVG_WINDOW_SIZE; i++)
    {
        samples[i] = 0.0f;
    }
}

void Filter_Update(void)
{
    float new_sample = gWeather.pm25;
    float alpha;
    float sum = 0.0f;
    uint8_t i;

    if(first_run)
    {
        pm25_ema = new_sample;

        min_pm25 = new_sample;
        max_pm25 = new_sample;

        first_run = 0;
    }
    else
    {
        /* Min / Max Update */
        if(new_sample < min_pm25)
        {
            min_pm25 = new_sample;
        }

        if(new_sample > max_pm25)
        {
            max_pm25 = new_sample;
        }

        /* Spike Detection */
        if(fabsf(new_sample - pm25_ema) > PM25_SPIKE_THRESHOLD)
        {
            rejected_samples++;
            filter_status = FILTER_DATA_SPIKE;

            gWeather.filtered_pm25 = pm25_ema;
            return;
        }

        filter_status = FILTER_DATA_VALID;

        /* Dynamic Alpha */
        if(fabsf(new_sample - pm25_ema) > 30.0f)
        {
            alpha = 0.5f;
        }
        else
        {
            alpha = 0.2f;
        }

        /* EMA */
        pm25_ema =
            (1.0f - alpha) * pm25_ema +
            alpha * new_sample;
    }

    /* Moving Average */
    samples[sample_index] = new_sample;

    sample_index++;
    if(sample_index >= MOVING_AVG_WINDOW_SIZE)
    {
        sample_index = 0;
    }

    if(sample_count < MOVING_AVG_WINDOW_SIZE)
    {
        sample_count++;
    }

    for(i = 0; i < sample_count; i++)
    {
        sum += samples[i];
    }

    moving_average = sum / sample_count;

    /* Main Output */
    gWeather.filtered_pm25 = pm25_ema;
}

/* Statistics */
uint32_t Filter_GetRejectedSamples(void)
{
    return rejected_samples;
}

/* Status */
FilterStatus_t Filter_GetStatus(void)
{
    return filter_status;
}

/* Min / Max */
float Filter_GetMinPM25(void)
{
    return min_pm25;
}

float Filter_GetMaxPM25(void)
{
    return max_pm25;
}

/* Outputs */
float Filter_GetEMAOutput(void)
{
    return pm25_ema;
}

float Filter_GetMovingAverageOutput(void)
{
    return moving_average;
}