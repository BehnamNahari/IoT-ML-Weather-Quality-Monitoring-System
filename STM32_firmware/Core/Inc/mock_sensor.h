#ifndef MOCK_SENSOR_H
#define MOCK_SENSOR_H

#include <stdint.h>

typedef enum
{
    MOCK_SCENARIO_NORMAL   = 0,
    MOCK_SCENARIO_POLLUTED = 1,
    MOCK_SCENARIO_HOT      = 2,
} MockScenario_t;

void MockSensor_Init(MockScenario_t scenario);
void MockSensor_Update(void);

#endif