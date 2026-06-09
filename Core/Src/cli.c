#include "cli.h"
#include "aqi.h"
#include "alarm.h"
#include "history.h"
#include "predictor.h"
#include "system_data.h"
#include "main.h"
#include <stdio.h>
#include <string.h>

extern UART_HandleTypeDef huart2;

#define CLI_BUF_SIZE 64

static char    s_buf[CLI_BUF_SIZE];
static uint8_t s_len = 0;
static uint8_t s_rx_byte;

static void CLI_Send(const char *msg)
{
    HAL_UART_Transmit(&huart2, (uint8_t *)msg, (uint16_t)strlen(msg), HAL_MAX_DELAY);
}

static void CLI_Execute(char *cmd)
{
    char out[128];

    if (strcmp(cmd, "help") == 0)
    {
        CLI_Send("Commands: help status aqi alarm history forecast\r\n");
    }
    else if (strcmp(cmd, "status") == 0)
    {
        CLI_Send("System Running\r\n");
    }
    else if (strcmp(cmd, "aqi") == 0)
    {
        snprintf(out, sizeof(out),
                 "AQI=%u  Min=%u  Max=%u  Avg=%.1f\r\n",
                 gWeather.aqi,
                 AQI_GetMin(),
                 AQI_GetMax(),
                 AQI_GetAverage());
        CLI_Send(out);
    }
    else if (strcmp(cmd, "alarm") == 0)
    {
        snprintf(out, sizeof(out),
                 "Alarm=%s  Level=%d  Count=%lu\r\n",
                 Alarm_IsActive() ? "ON" : "OFF",
                 (int)Alarm_GetLevel(),
                 Alarm_GetCounter());
        CLI_Send(out);
    }
    else if (strcmp(cmd, "history") == 0)
    {
        snprintf(out, sizeof(out),
                 "Samples=%u  AvgCO2=%.1f  AvgPM25=%.1f\r\n",
                 History_GetSampleCount(),
                 History_GetAvgCO2(),
                 History_GetAvgPM25());
        CLI_Send(out);
    }
    else if (strcmp(cmd, "forecast") == 0)
    {
        snprintf(out, sizeof(out),
                 "Forecast AQI=%u\r\n",
                 Predictor_GetForecastAQI());
        CLI_Send(out);
    }
    else
    {
        CLI_Send("Unknown command\r\n");
    }
}

void CLI_Init(void)
{
    s_len = 0;
    memset(s_buf, 0, sizeof(s_buf));
    HAL_UART_Receive_IT(&huart2, &s_rx_byte, 1);
}

void CLI_Process(void)
{
    if (s_len == 0)
    {
        return;
    }

    s_buf[s_len] = '\0';
    CLI_Execute(s_buf);
    s_len = 0;
}

void CLI_RxCallback(void)
{
    uint8_t c = s_rx_byte;

    HAL_UART_Receive_IT(&huart2, &s_rx_byte, 1);

    if (c == '\r' || c == '\n')
    {
        if (s_len > 0)
        {
            s_buf[s_len] = '\0';
        }
        return;
    }

    if (s_len < (CLI_BUF_SIZE - 1))
    {
        s_buf[s_len++] = (char)c;
    }
}
