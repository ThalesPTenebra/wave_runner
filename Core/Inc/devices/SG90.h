/*
 * Equipe: Thales Tenebra, Silvio Porto, Antonio
 * Arquivo: L293D.h
 * Descrição: Header para controle do módulo L293D.
 */

#ifndef __L293D_H
#define __L293D_H

#include <stdint.h>
#include "stm32f4xx.h"

/* Enum para controle de direção e estado dos motores */
typedef enum {
    M1_FORWARD = 0x04,
    M1_STOP = 0x00,
    M2_FORWARD = 0x02,
    M2_STOP = 0x00,
    M3_FORWARD = 0x20,
    M3_STOP = 0x00,
    M4_FORWARD = 0x01,
    M4_STOP = 0x00
} L293D_Control;

/* Estrutura para configuração do módulo L293D */
typedef struct {
    TIM_HandleTypeDef* htim;
    uint32_t channel;
    GPIO_TypeDef* EN_GPIO;
    uint16_t EN_Pin;
    GPIO_TypeDef* LATCH_GPIO;
    uint16_t LATCH_Pin;
    GPIO_TypeDef* CLK_GPIO;
    uint16_t CLK_Pin;
    GPIO_TypeDef* SER_GPIO;
    uint16_t SER_Pin;
} L293D_HandleTypeDef;

/**
 * @brief Inicializa o módulo L293D com parâmetros iniciais de controle.
 *
 * @param handler Ponteiro para o handler do módulo L293D.
 * @param period Período inicial do timer.
 */
void L293DDriver_Init(L293D_HandleTypeDef* handler, uint32_t period);

/**
 * @brief Define a velocidade do motor.
 *
 * @param handler Ponteiro para o handler do módulo L293D.
 * @param speed Velocidade relativa do motor (0.0 = mínimo, 1.0 = máximo).
 */
void L293DDriver_SetSpeed(L293D_HandleTypeDef* handler, float speed);

/**
 * @brief Envia comandos de controle para o módulo.
 *
 * @param handler Ponteiro para o handler do módulo L293D.
 * @param control Comando de controle (direção e estado).
 */
void L293DDriver_SendControl(L293D_HandleTypeDef* handler, L293D_Control control);

#endif /* __L293D_H */
