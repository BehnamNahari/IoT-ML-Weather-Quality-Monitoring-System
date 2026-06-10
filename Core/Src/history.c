#include "history.h"
#include "system_data.h"

#define HISTORY_MAX_SAMPLES 50

typedef struct
{
    float    co2;
    float    pm25;
} HistorySample_t;

static HistorySample_t s_samples[HISTORY_MAX_SAMPLES];
static uint16_t        s_head  = 0;
static uint16_t        s_count = 0;

void History_Init(void)
{
    s_head  = 0;
    s_count = 0;
}

void History_PushSample(void)
{
    s_samples[s_head].co2  = gWeather.co2;
    s_samples[s_head].pm25 = gWeather.pm25;

    s_head = (s_head + 1U) % HISTORY_MAX_SAMPLES;

    if (s_count < HISTORY_MAX_SAMPLES)
    {
        s_count++;
    }
}

float History_GetAvgCO2(void)
{
    float sum = 0.0f;
    uint16_t i;

    if (s_count == 0U)
    {
        return 0.0f;
    }

    for (i = 0; i < s_count; i++)
    {
        sum += s_samples[i].co2;
    }

    return sum / (float)s_count;
}

float History_GetAvgPM25(void)
{
    float sum = 0.0f;
    uint16_t i;

    if (s_count == 0U)
    {
        return 0.0f;
    }

    for (i = 0; i < s_count; i++)
    {
        sum += s_samples[i].pm25;
    }

    return sum / (float)s_count;
}

float History_GetMinPM25(void)
{
    float min_val;
    uint16_t i;

    if (s_count == 0U)
    {
        return 0.0f;
    }

    min_val = s_samples[0].pm25;

    for (i = 1; i < s_count; i++)
    {
        if (s_samples[i].pm25 < min_val)
        {
            min_val = s_samples[i].pm25;
        }
    }

    return min_val;
}

float History_GetMaxPM25(void)
{
    float max_val;
    uint16_t i;

    if (s_count == 0U)
    {
        return 0.0f;
    }

    max_val = s_samples[0].pm25;

    for (i = 1; i < s_count; i++)
    {
        if (s_samples[i].pm25 > max_val)
        {
            max_val = s_samples[i].pm25;
        }
    }

    return max_val;
}

uint16_t History_GetSampleCount(void)
{
    return s_count;
}
