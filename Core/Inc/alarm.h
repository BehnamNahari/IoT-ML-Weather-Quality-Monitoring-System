#ifndef ALARM_H
#define ALARM_H

#include <stdint.h>

void Alarm_Init(void);
void Alarm_Update(void);
uint8_t Alarm_IsActive(void);

#endif