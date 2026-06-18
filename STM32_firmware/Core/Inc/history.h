#ifndef HISTORY_H
#define HISTORY_H

#include <stdint.h>

void History_Init(void);

void History_PushSample(void);

float History_GetAvgPM25(void);
float History_GetAvgCO2(void);

float History_GetMinPM25(void);
float History_GetMaxPM25(void);

uint16_t History_GetSampleCount(void);

#endif