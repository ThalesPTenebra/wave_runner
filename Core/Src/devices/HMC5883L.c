/*
 * Equipe: Thales Tenebra, Silvio Porto, Antonio
 * Arquivo: HMC5883L.c
 * Descrição: Implementação do driver do magnetometro.
 */

#include "devices/HMC5883L.h"
#include <stdint.h>
#include <stdio.h>
#include <math.h>
#include "stm32f4xx_hal.h" // Ajuste para sua plataforma

/* Variáveis Estáticas para Configuração */
static HMC5883L_HandlerTypeDef magnetometer;
static HMC5883L_CalibrationParamsTypeDef calibration;

/* Declaração da Declinação Magnética (em radianos) */
#define MAGNETIC_DECLINATION 0.0f // Ajuste para sua localização

// Inicializa o HMC5883L
void HMC5883LDriver_Init(I2C_HandleTypeDef *hi2c) {
    magnetometer.hi2c = hi2c; // Atribui o handle I2C passado
    magnetometer.samples_avg = 3;
    magnetometer.output_rate = 4;
    magnetometer.measurement_mode = 0;
    magnetometer.gain = 1;
    magnetometer.operation_mode = 0;

    uint8_t config_reg_a = ((magnetometer.samples_avg & 0b11) << 5) |
                           ((magnetometer.output_rate & 0b111) << 2) |
                           (magnetometer.measurement_mode & 0b11);

    uint8_t config_reg_b = (magnetometer.gain & 0b111) << 5;
    uint8_t mode_reg = magnetometer.operation_mode & 0b11;

    if (HAL_I2C_Mem_Write(magnetometer.hi2c, HMC5883L_ADDRESS, HMC5883L_CONF_REG_A, 1, &config_reg_a, 1, HAL_MAX_DELAY) != HAL_OK ||
        HAL_I2C_Mem_Write(magnetometer.hi2c, HMC5883L_ADDRESS, HMC5883L_CONF_REG_B, 1, &config_reg_b, 1, HAL_MAX_DELAY) != HAL_OK ||
        HAL_I2C_Mem_Write(magnetometer.hi2c, HMC5883L_ADDRESS, HMC5883L_MODE_REG, 1, &mode_reg, 1, HAL_MAX_DELAY) != HAL_OK) {
        printf("Erro ao inicializar o HMC5883L.\n");
    }

    // Calibração
    HMC5883LDriver_Calibrate();
}

/* Leitura do Magnetômetro */
bool HMC5883LDriver_Read(HMC5883L_DataTypeDef *data) {
    uint8_t buffer[6];

    if (HAL_I2C_Mem_Read(magnetometer.hi2c, HMC5883L_ADDRESS, HMC5883L_DATAX_MSB_REG, 1, buffer, 6, HAL_MAX_DELAY) != HAL_OK) {
        return false;
    }

    data->x_axis = (buffer[0] << 8) | buffer[1];
    data->z_axis = (buffer[2] << 8) | buffer[3];
    data->y_axis = (buffer[4] << 8) | buffer[5];

    return true;
}

/* Calibração do Magnetômetro */
void HMC5883LDriver_Calibrate(void) {
    HMC5883L_DataTypeDef data;

    calibration.xmin = INT16_MAX;
    calibration.xmax = INT16_MIN;
    calibration.ymin = INT16_MAX;
    calibration.ymax = INT16_MIN;

    for (int i = 0; i < 100; i++) {
        if (HMC5883LDriver_Read(&data)) {
            calibration.xmin = (data.x_axis < calibration.xmin) ? data.x_axis : calibration.xmin;
            calibration.xmax = (data.x_axis > calibration.xmax) ? data.x_axis : calibration.xmax;
            calibration.ymin = (data.y_axis < calibration.ymin) ? data.y_axis : calibration.ymin;
            calibration.ymax = (data.y_axis > calibration.ymax) ? data.y_axis : calibration.ymax;

            HAL_Delay(10);
        }
    }

    calibration.x_offset = (calibration.xmax + calibration.xmin) / 2.0f;
    calibration.y_offset = (calibration.ymax + calibration.ymin) / 2.0f;

    float x_var = calibration.xmax - calibration.xmin;
    float y_var = calibration.ymax - calibration.ymin;

    calibration.x_scale = (x_var > y_var) ? 1.0f : y_var / x_var;
    calibration.y_scale = (y_var > x_var) ? 1.0f : x_var / y_var;
}

/* Cálculo do Ângulo */
float HMC5883LDriver_GetHeading(void) {
    HMC5883L_DataTypeDef data;

    if (HMC5883LDriver_Read(&data)) {
        float x_norm = (data.x_axis - calibration.x_offset) * calibration.x_scale;
        float y_norm = (data.y_axis - calibration.y_offset) * calibration.y_scale;

        float heading = atan2f(y_norm, x_norm);
        heading += MAGNETIC_DECLINATION;
        if (heading < 0) heading += 2 * M_PI;
        if (heading >= 2 * M_PI) heading -= 2 * M_PI;

        return heading * 180.0f / M_PI;
    }

    return 0; // Retorna 0 se a leitura falhar
}
