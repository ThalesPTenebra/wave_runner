/*
 * Equipe: Thales Tenebra, Silvio Porto, Antonio
 * Arquivo: HMC5883L.h
 * Descrição: Implementação do driver do magnetometro.
 */


#ifndef HMC5883L_H
#define HMC5883L_H

#include "stm32f4xx_hal.h" // Substitua pelo HAL correspondente ao seu microcontrolador
#include <stdint.h>
#include <stdbool.h>


/* Endereço do HMC5883L */
#define HMC5883L_ADDRESS         (0x1E << 1) // Shift para leitura/escrita no barramento I2C

/* Registradores do HMC5883L */
#define HMC5883L_CONF_REG_A      0x00
#define HMC5883L_CONF_REG_B      0x01
#define HMC5883L_MODE_REG        0x02
#define HMC5883L_DATAX_MSB_REG   0x03
#define HMC5883L_STATUS_REG      0x09

/* Estrutura para os Dados do Magnetômetro */
typedef struct {
    int16_t x_axis;
    int16_t y_axis;
    int16_t z_axis;
} HMC5883L_DataTypeDef;

/* Estrutura para Configuração do Magnetômetro */
typedef struct {
    I2C_HandleTypeDef *hi2c; // Handle do I2C
    uint8_t samples_avg;     // Amostras para média (1, 2, 4 ou 8)
    uint8_t output_rate;     // Taxa de saída (0.75Hz a 75Hz)
    uint8_t measurement_mode;// Modo de medição (Normal, Positive, Negative)
    uint8_t gain;            // Ganho (±0.88 Ga a ±8.1 Ga)
    uint8_t operation_mode;  // Modo de operação (Continuous, Single, Idle)
} HMC5883L_HandlerTypeDef;

/* Estrutura para Calibração */
typedef struct {
    float xmin, xmax, ymin, ymax;
    float x_offset, y_offset;
    float x_scale, y_scale;
} HMC5883L_CalibrationParamsTypeDef;

/* Prototipação das Funções */
void HMC5883LDriver_Init(I2C_HandleTypeDef *hi2c);
bool HMC5883LDriver_Read(HMC5883L_DataTypeDef *data);
void HMC5883LDriver_Calibrate(void);
float HMC5883LDriver_GetHeading(void);

#endif /* HMC5883L_H */
