#include "alarm.h"
#include "system_data.h"
#include "app_config.h"

/*
 * AQI Thresholds
 */
#define AQI_WARNING_ON      100U
#define AQI_WARNING_OFF      90U

#define AQI_DANGER_ON       150U
#define AQI_DANGER_OFF      130U

#define AQI_CRITICAL_ON     250U
#define AQI_CRITICAL_OFF    220U


static uint8_t alarm_active = 0;
static uint32_t alarm_counter = 0;

static AlarmLevel_t current_level = ALARM_LEVEL_NONE;
static AlarmReason_t current_reason = ALARM_REASON_NONE;

/* Advanced Statistics */
static AlarmLevel_t max_level = ALARM_LEVEL_NONE;

static uint32_t alarm_active_ticks = 0;

static uint32_t warning_count = 0;
static uint32_t danger_count = 0;
static uint32_t critical_count = 0;

static uint16_t last_aqi = 0;
static float last_co2 = 0.0f;

/*----------------------------------------------------------*/

void Alarm_Init(void)
{
    alarm_active = 0;
    alarm_counter = 0;

    current_level = ALARM_LEVEL_NONE;
    current_reason = ALARM_REASON_NONE;

    max_level = ALARM_LEVEL_NONE;

    alarm_active_ticks = 0;

    warning_count = 0;
    danger_count = 0;
    critical_count = 0;

    last_aqi = 0;
    last_co2 = 0.0f;

    gWeather.alarm = 0;
}

/*----------------------------------------------------------*/

static AlarmLevel_t GetAQILevel(uint16_t aqi)
{
    if(aqi >= AQI_CRITICAL_ON)
    {
        return ALARM_LEVEL_CRITICAL;
    }

    if(aqi >= AQI_DANGER_ON)
    {
        return ALARM_LEVEL_DANGER;
    }

    if(aqi >= AQI_WARNING_ON)
    {
        return ALARM_LEVEL_WARNING;
    }

    return ALARM_LEVEL_NONE;
}

/*----------------------------------------------------------*/

static AlarmLevel_t GetCO2Level(float co2)
{
    if(co2 >= (CO2_DANGER_THRESHOLD + 400.0f))
    {
        return ALARM_LEVEL_CRITICAL;
    }

    if(co2 >= CO2_DANGER_THRESHOLD)
    {
        return ALARM_LEVEL_DANGER;
    }

    if(co2 >= CO2_WARNING_THRESHOLD)
    {
        return ALARM_LEVEL_WARNING;
    }

    return ALARM_LEVEL_NONE;
}

/*----------------------------------------------------------*/

void Alarm_Update(void)
{
    AlarmLevel_t aqiLevel;
    AlarmLevel_t co2Level;
    AlarmLevel_t newLevel;

    uint8_t previousState;
    AlarmLevel_t previousLevel;

    previousState = alarm_active;
    previousLevel = current_level;

    aqiLevel = GetAQILevel(gWeather.aqi);
    co2Level = GetCO2Level(gWeather.co2);

    newLevel = (aqiLevel > co2Level) ? aqiLevel : co2Level;

    /*
     * Alarm Reason
     */
    if((aqiLevel != ALARM_LEVEL_NONE) &&
       (co2Level != ALARM_LEVEL_NONE))
    {
        current_reason = ALARM_REASON_BOTH;
    }
    else if(aqiLevel != ALARM_LEVEL_NONE)
    {
        current_reason = ALARM_REASON_AQI;
    }
    else if(co2Level != ALARM_LEVEL_NONE)
    {
        current_reason = ALARM_REASON_CO2;
    }
    else
    {
        current_reason = ALARM_REASON_NONE;
    }

    /*
     * Data Validation
     */
    if(gWeather.aqi > 1000U)
    {
        newLevel = ALARM_LEVEL_CRITICAL;
    }

    if(gWeather.co2 > 10000.0f)
    {
        newLevel = ALARM_LEVEL_CRITICAL;
    }

    /*
     * Sudden AQI Change Detection
     */
    if((last_aqi != 0U) &&
       (gWeather.aqi > (last_aqi + 50U)))
    {
        if(newLevel < ALARM_LEVEL_DANGER)
        {
            newLevel = ALARM_LEVEL_DANGER;
        }
    }

    /*
     * Sudden CO2 Change Detection
     */
    if((last_co2 > 0.0f) &&
       ((gWeather.co2 - last_co2) > 300.0f))
    {
        if(newLevel < ALARM_LEVEL_DANGER)
        {
            newLevel = ALARM_LEVEL_DANGER;
        }
    }

    /*
     * Hysteresis
     */
    switch(current_level)
    {
        case ALARM_LEVEL_CRITICAL:

            if((gWeather.aqi < AQI_CRITICAL_OFF) &&
               (gWeather.co2 < (CO2_DANGER_THRESHOLD + 300.0f)))
            {
                current_level = newLevel;
            }
            break;

        case ALARM_LEVEL_DANGER:

            if((gWeather.aqi < AQI_DANGER_OFF) &&
               (gWeather.co2 < (CO2_DANGER_THRESHOLD - 100.0f)))
            {
                current_level = newLevel;
            }
            break;

        case ALARM_LEVEL_WARNING:

            if((gWeather.aqi < AQI_WARNING_OFF) &&
               (gWeather.co2 < (CO2_WARNING_THRESHOLD - 100.0f)))
            {
                current_level = newLevel;
            }
            break;

        default:
            current_level = newLevel;
            break;
    }

    /*
     * Maximum Alarm Level
     */
    if(current_level > max_level)
    {
        max_level = current_level;
    }

    /*
     * Statistics Per Level
     */
    if(previousLevel != current_level)
    {
        switch(current_level)
        {
            case ALARM_LEVEL_WARNING:
                warning_count++;
                break;

            case ALARM_LEVEL_DANGER:
                danger_count++;
                break;

            case ALARM_LEVEL_CRITICAL:
                critical_count++;
                break;

            default:
                break;
        }
    }

    alarm_active = (current_level != ALARM_LEVEL_NONE);

    /*
     * Active Time Counter
     */
    if(alarm_active)
    {
        alarm_active_ticks++;
    }

    /*
     * Alarm Counter
     */
    if((previousState == 0U) &&
       (alarm_active == 1U))
    {
        alarm_counter++;
    }

    /*
     * Shared System Data
     */
    gWeather.alarm = alarm_active;

    /*
     * Save Previous Samples
     */
    last_aqi = gWeather.aqi;
    last_co2 = gWeather.co2;
}

/*----------------------------------------------------------*/

uint8_t Alarm_IsActive(void)
{
    return alarm_active;
}

/*----------------------------------------------------------*/

AlarmLevel_t Alarm_GetLevel(void)
{
    return current_level;
}

/*----------------------------------------------------------*/

AlarmReason_t Alarm_GetReason(void)
{
    return current_reason;
}

/*----------------------------------------------------------*/

uint32_t Alarm_GetCounter(void)
{
    return alarm_counter;
}