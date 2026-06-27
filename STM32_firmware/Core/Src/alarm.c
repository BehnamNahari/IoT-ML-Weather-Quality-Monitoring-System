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


/*----------------------------------------------------------*/

void Alarm_Init(void)
{
    alarm_active = 0;
    alarm_counter = 0;

    current_level = ALARM_LEVEL_NONE;
    current_reason = ALARM_REASON_NONE;

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

    previousState = alarm_active;

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

    alarm_active = (current_level != ALARM_LEVEL_NONE);

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

const char* AlarmLevelToString(AlarmLevel_t level)
{
    switch(level)
    {
        case ALARM_LEVEL_WARNING:
            return "WARNING";

        case ALARM_LEVEL_DANGER:
            return "DANGER";

        case ALARM_LEVEL_CRITICAL:
            return "CRITICAL";

        case ALARM_LEVEL_NONE:
        default:
            return "NONE";
    }
}