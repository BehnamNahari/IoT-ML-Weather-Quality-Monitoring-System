#ifndef __BMP280_H
#define __BMP280_H

#include "main.h"

#define BMP280_ADDR (0x76 << 1)
#define BMP280_CHIP_ID_REG 0xD0

HAL_StatusTypeDef BMP280_Init(I2C_HandleTypeDef *hi2c);
HAL_StatusTypeDef BMP280_ReadChipID(uint8_t *id);

#endif