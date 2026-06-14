#include "mock_sensor.h"
#include "system_data.h"
#include "stm32f1xx_hal.h"

#define TEMP_BASE_MIN    20.0f
#define TEMP_BASE_MAX    26.0f
#define TEMP_NOISE       0.3f

#define HUM_BASE_MIN     40.0f
#define HUM_BASE_MAX     65.0f
#define HUM_NOISE        0.5f

#define PRES_BASE_MIN   1000.0f
#define PRES_BASE_MAX   1015.0f
#define PRES_NOISE       0.2f

#define PM25_BASE_MIN    5.0f
#define PM25_BASE_MAX   30.0f
#define PM25_NOISE       1.0f

#define CO2_BASE_MIN   400.0f
#define CO2_BASE_MAX   700.0f
#define CO2_NOISE       5.0f

static uint32_t       s_seed;
static MockScenario_t s_scenario;

static float s_temp_base;
static float s_hum_base;
static float s_pres_base;
static float s_pm25_base;
static float s_co2_base;

static uint32_t NextRand(void)
{
    s_seed = 1664525u * s_seed + 1013904223u;
    return s_seed;
}

static float RandFloat(float lo, float hi)
{
    float t = (float)(NextRand() & 0xFFFFu) / 65536.0f;
    return lo + t * (hi - lo);
}

static float DriftToward(float current, float target, float rate)
{
    float delta = target - current;
    if (delta > rate)  { return current + rate; }
    if (delta < -rate) { return current - rate; }
    return target;
}

static float Clamp(float v, float min_val, float max_val)
{
    if (v < min_val) { return min_val; }
    if (v > max_val) { return max_val; }
    return v;
}

void MockSensor_Init(MockScenario_t scenario)
{
    s_seed     = 0x12345678u ^ HAL_GetTick();
    s_scenario = scenario;

    s_temp_base  = RandFloat(TEMP_BASE_MIN,  TEMP_BASE_MAX);
    s_hum_base   = RandFloat(HUM_BASE_MIN,   HUM_BASE_MAX);
    s_pres_base  = RandFloat(PRES_BASE_MIN,  PRES_BASE_MAX);
    s_pm25_base  = RandFloat(PM25_BASE_MIN,  PM25_BASE_MAX);
    s_co2_base   = RandFloat(CO2_BASE_MIN,   CO2_BASE_MAX);
}

void MockSensor_Update(void)
{
    float target_pm25;
    float target_co2;
    float target_temp;

    switch (s_scenario)
    {
        case MOCK_SCENARIO_POLLUTED:
            target_pm25 = 180.0f;
            target_co2  = 1600.0f;
            target_temp = RandFloat(TEMP_BASE_MIN, TEMP_BASE_MAX);
            break;

        case MOCK_SCENARIO_HOT:
            target_temp = 33.0f;
            target_pm25 = RandFloat(PM25_BASE_MIN, PM25_BASE_MAX);
            target_co2  = RandFloat(CO2_BASE_MIN,  CO2_BASE_MAX);
            break;

        case MOCK_SCENARIO_NORMAL:
        default:
            target_temp = RandFloat(TEMP_BASE_MIN, TEMP_BASE_MAX);
            target_pm25 = RandFloat(PM25_BASE_MIN, PM25_BASE_MAX);
            target_co2  = RandFloat(CO2_BASE_MIN,  CO2_BASE_MAX);
            break;
    }

    s_temp_base  = DriftToward(s_temp_base,  target_temp, 0.1f);
    s_hum_base   = DriftToward(s_hum_base,   RandFloat(HUM_BASE_MIN,  HUM_BASE_MAX), 0.3f);
    s_pres_base  = DriftToward(s_pres_base,  RandFloat(PRES_BASE_MIN, PRES_BASE_MAX), 0.05f);
    s_pm25_base  = DriftToward(s_pm25_base,  target_pm25, 0.5f);
    s_co2_base   = DriftToward(s_co2_base,   target_co2,  2.0f);

    gWeather.temperature = Clamp(s_temp_base + RandFloat(-TEMP_NOISE, TEMP_NOISE), 15.0f, 45.0f);
    gWeather.humidity    = Clamp(s_hum_base  + RandFloat(-HUM_NOISE,  HUM_NOISE),  0.0f, 100.0f);
static float bmp280_pressure = 1013.25f;

/* BMP280 Pressure Sensor Simulation */
bmp280_pressure += RandFloat(-0.2f, 0.2f);

gWeather.pressure =
    Clamp(bmp280_pressure,
          950.0f,
          1060.0f);
    gWeather.pm25        = Clamp(s_pm25_base + RandFloat(-PM25_NOISE, PM25_NOISE), 0.0f, 500.0f);
    gWeather.co2         = Clamp(s_co2_base  + RandFloat(-CO2_NOISE,  CO2_NOISE),  300.0f, 5000.0f);
}