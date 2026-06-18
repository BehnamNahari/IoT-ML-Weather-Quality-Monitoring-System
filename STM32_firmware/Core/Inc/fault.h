#ifndef FAULT_H
#define FAULT_H

#include <stdint.h>

#define FAULT_NONE					(0U)
#define FAULT_PM25_SPIKE		(1U << 0)
#define FAULT_CO2_INVALID		(1U << 1)
#define FAULT_SENSOR_ERROR	(1U << 2)

void Fault_Init(void);
void Fault_Update(void);

uint32_t Fault_GetFlags(void);
uint8_t Fault_HasFault(void);

#endif