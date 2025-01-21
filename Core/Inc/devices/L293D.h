#ifndef __L293D_H
#define __L293D_H

#include <stdint.h>

#include "stm32f4xx.h"

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
 * @brief Initialize the L293D module with the initial motor control parameters.
 *
 * @param handler L293D_HandleTypeDef pointer to module handler.
 * @param period Initial timer period.
 */
void L293DDriver_Init(L293D_HandleTypeDef* handler, uint32_t period);

/**
 * @brief Set motor speed value.
 *
 * @param handler L293D_HandleTypeDef pointer to module handler.
 * @param speed Relative speed of the motor, where 0.0 is the minimum speed and 1.0 the maximum speed.
 */
void L293DDriver_SetSpeed(L293D_HandleTypeDef* handler, float speed);

/**
 * @brief Send control to module.
 *
 * @param handler L293D_HandleTypeDef pointer to module handler.
 * @param control L293D_Control indicating control command.
 */
void L293DDriver_SendControl(L293D_HandleTypeDef* handler, L293D_Control control);

#endif /* __L293D_DRIVER_H */
