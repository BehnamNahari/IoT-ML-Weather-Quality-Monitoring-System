#include "aqi.h"
#include "system_data.h"
#include "app_config.h"

/* AQI Statistics */
static uint16_t min_aqi = 65535;
static uint16_t max_aqi = 0;

static uint32_t aqi_sum = 0;
static uint32_t aqi_count = 0;


static AQITrend_t aqi_trend = AQI_TREND_STABLE;
static uint16_t previous_aqi = 0;

/* Linear Interpolation */
static uint16_t InterpolateAQI(float value,
                               float c_low,
                               float c_high,
                               uint16_t i_low,
                               uint16_t i_high)
{
    return (uint16_t)
    (
        i_low +
        ((value - c_low) * (i_high - i_low)) /
        (c_high - c_low)
    );
}

/* AQI Calculation */
static uint16_t ComputeAQI(float pm25, AirState_t *state)
{
    if(pm25 <= 12.0f)
    {
        *state = AIR_STATE_GOOD;

        return InterpolateAQI(
            pm25,
            0.0f,
            12.0f,
            0,
            50);
    }
    else if(pm25 <= 35.4f)
    {
        *state = AIR_STATE_MODERATE;

        return InterpolateAQI(
            pm25,
            12.0f,
            35.4f,
            50,
            100);
    }
    else if(pm25 <= 55.4f)
    {
        *state = AIR_STATE_WARNING;

        return InterpolateAQI(
            pm25,
            35.4f,
            55.4f,
            100,
            150);
    }
    else
    {
        *state = AIR_STATE_DANGEROUS;

        if(pm25 > 150.0f)
        {
            pm25 = 150.0f;
        }

        return InterpolateAQI(
            pm25,
            55.4f,
            150.0f,
            150,
            300);
    }
}

void AQI_Init(void)
{
    gWeather.aqi = 0;
    gWeather.air_state = AIR_STATE_GOOD;

    min_aqi = 65535;
    max_aqi = 0;

    aqi_sum = 0;
    aqi_count = 0;

    aqi_trend = AQI_TREND_STABLE;
    previous_aqi = 0;
}

void AQI_Update(void)
{
    gWeather.aqi =
        ComputeAQI(
            gWeather.filtered_pm25,
            &gWeather.air_state);

    if(gWeather.co2 >= CO2_DANGER_THRESHOLD)
    {
        gWeather.air_state = AIR_STATE_DANGEROUS;

        if(gWeather.aqi < 200)
        {
            gWeather.aqi = 200;
        }
    }
    else if(
        gWeather.co2 >= CO2_WARNING_THRESHOLD &&
        gWeather.air_state < AIR_STATE_WARNING)
    {
        gWeather.air_state = AIR_STATE_WARNING;

        if(gWeather.aqi < 120)
        {
            gWeather.aqi = 120;
        }
    }

    /* Statistics */

    if(gWeather.aqi < min_aqi)
    {
        min_aqi = gWeather.aqi;
    }

    if(gWeather.aqi > max_aqi)
    {
        max_aqi = gWeather.aqi;
    }

    aqi_sum += gWeather.aqi;
    aqi_count++;

    /* Trend Detection */

    if(previous_aqi != 0)
    {
        if(gWeather.aqi > previous_aqi + 5)
        {
            aqi_trend = AQI_TREND_WORSENING;
        }
        else if(gWeather.aqi + 5 < previous_aqi)
        {
            aqi_trend = AQI_TREND_IMPROVING;
        }
        else
        {
            aqi_trend = AQI_TREND_STABLE;
        }
    }

    previous_aqi = gWeather.aqi;
		
}

/* Statistics API */

uint16_t AQI_GetMin(void)
{
    return min_aqi;
}

uint16_t AQI_GetMax(void)
{
    return max_aqi;
}

float AQI_GetAverage(void)
{
    if(aqi_count == 0)
    {
        return 0.0f;
    }

    return (float)aqi_sum / (float)aqi_count;
}

/* Trend API */

AQITrend_t AQI_GetTrend(void)
{
    return aqi_trend;
}