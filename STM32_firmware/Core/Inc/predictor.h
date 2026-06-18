#ifndef PREDICTOR_H
#define PREDICTOR_H

#include <stdint.h>

void Predictor_Init(void);

void Predictor_Update(void);

uint16_t Predictor_GetForecastAQI(void);
	
#endif