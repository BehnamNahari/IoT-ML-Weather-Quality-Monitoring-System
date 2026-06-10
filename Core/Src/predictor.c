#include "predictor.h"
#include "system_data.h"

#define PREDICTOR_WINDOW 10

static uint16_t s_aqi_buf[PREDICTOR_WINDOW];
static uint8_t  s_head  = 0;
static uint8_t  s_count = 0;
static uint16_t s_forecast = 0;

void Predictor_Init(void)
{
    s_head     = 0;
    s_count    = 0;
    s_forecast = 0;
}

void Predictor_Update(void)
{
    int32_t  sum_x  = 0;
    int32_t  sum_y  = 0;
    int32_t  sum_xy = 0;
    int32_t  sum_x2 = 0;
    int32_t  n;
    int32_t  slope_num;
    int32_t  slope_den;
    int32_t  intercept;
    int32_t  predicted;
    uint8_t  i;
    uint8_t  idx;

    s_aqi_buf[s_head] = gWeather.aqi;
    s_head = (s_head + 1U) % PREDICTOR_WINDOW;

    if (s_count < PREDICTOR_WINDOW)
    {
        s_count++;
    }

    if (s_count < 2U)
    {
        s_forecast = gWeather.aqi;
        return;
    }

    n = (int32_t)s_count;

    for (i = 0; i < s_count; i++)
    {
        idx = (uint8_t)((s_head + PREDICTOR_WINDOW - s_count + i) % PREDICTOR_WINDOW);
        sum_x  += i;
        sum_y  += (int32_t)s_aqi_buf[idx];
        sum_xy += i * (int32_t)s_aqi_buf[idx];
        sum_x2 += i * i;
    }

    slope_den = n * sum_x2 - sum_x * sum_x;

    if (slope_den == 0)
    {
        s_forecast = gWeather.aqi;
        return;
    }

    slope_num = n * sum_xy - sum_x * sum_y;
    intercept = (sum_y - (slope_num / slope_den) * sum_x) / n;
    predicted = (slope_num / slope_den) * n + intercept;

    if (predicted < 0)
    {
        predicted = 0;
    }
    if (predicted > 500)
    {
        predicted = 500;
    }

    s_forecast = (uint16_t)predicted;
}

uint16_t Predictor_GetForecastAQI(void)
{
    return s_forecast;
}
