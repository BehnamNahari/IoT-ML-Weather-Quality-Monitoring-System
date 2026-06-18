#ifndef AQI_H
#define AQI_H

#include <stdint.h>
#include "system_data.h"

/* Core */
void AQI_Init(void);
void AQI_Update(void);

/* Statistics */
uint16_t AQI_GetMin(void);
uint16_t AQI_GetMax(void);
float AQI_GetAverage(void);

/* Trend */
typedef enum
{
    AQI_TREND_STABLE = 0,
    AQI_TREND_IMPROVING,
    AQI_TREND_WORSENING
} AQITrend_t;

AQITrend_t AQI_GetTrend(void);

#endif