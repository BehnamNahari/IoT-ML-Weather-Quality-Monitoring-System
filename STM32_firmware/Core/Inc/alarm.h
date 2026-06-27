#ifndef ALARM_H
#define ALARM_H

#include <stdint.h>

typedef enum
{
    ALARM_LEVEL_NONE = 0,
    ALARM_LEVEL_WARNING,
    ALARM_LEVEL_DANGER,
    ALARM_LEVEL_CRITICAL

} AlarmLevel_t;

typedef enum
{
    ALARM_REASON_NONE = 0,
    ALARM_REASON_AQI,
    ALARM_REASON_CO2,
    ALARM_REASON_BOTH

} AlarmReason_t;

void Alarm_Init(void);
void Alarm_Update(void);
const char* AlarmLevelToString(AlarmLevel_t level);
uint8_t Alarm_IsActive(void);

AlarmLevel_t Alarm_GetLevel(void);
AlarmReason_t Alarm_GetReason(void);

uint32_t Alarm_GetCounter(void);

#endif