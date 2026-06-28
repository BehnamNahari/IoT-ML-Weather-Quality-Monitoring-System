#include "dht11.h"

float temperature = 25.0f;
float humidity = 55.0f;

void DHT11_Read(void)
{
    temperature += 0.2f;

    if(temperature > 35.0f)
    {
        temperature = 25.0f;
    }

    humidity += 1.0f;

    if(humidity > 80.0f)
    {
        humidity = 55.0f;
    }
}