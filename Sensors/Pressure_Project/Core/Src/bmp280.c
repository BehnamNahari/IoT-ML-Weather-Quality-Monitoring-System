#include "bmp280.h"

static I2C_HandleTypeDef *bmp_i2c;

HAL_StatusTypeDef BMP280_Init(I2C_HandleTypeDef *hi2c)
{
    bmp_i2c = hi2c;
    return HAL_OK;
}

HAL_StatusTypeDef BMP280_ReadChipID(uint8_t *id)
{
    return HAL_I2C_Mem_Read(
            bmp_i2c,
            BMP280_ADDR,
            BMP280_CHIP_ID_REG,
            I2C_MEMADD_SIZE_8BIT,
            id,
            1,
            HAL_MAX_DELAY);
}