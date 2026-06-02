#ifndef FILTER_H
#define FILTER_H

#include <stdint.h>

/* Data Quality Status */
typedef enum
{
    FILTER_DATA_VALID = 0,
    FILTER_DATA_SPIKE
} FilterStatus_t;

/* Main Functions */
void Filter_Init(void);
void Filter_Update(void);

/* Statistics */
uint32_t Filter_GetRejectedSamples(void);

/* Status */
FilterStatus_t Filter_GetStatus(void);

/* Min / Max */
float Filter_GetMinPM25(void);
float Filter_GetMaxPM25(void);

/* Outputs */
float Filter_GetEMAOutput(void);
float Filter_GetMovingAverageOutput(void);

#endif