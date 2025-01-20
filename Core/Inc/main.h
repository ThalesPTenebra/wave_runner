/* USER CODE BEGIN Header */
/*
 * Equipe: Thales Tenebra, Silvio Porto, Antonio
 * Arquivo: main.c
 * Descrição: Arquivo principal responsável pela inicialização do sistema
 * e gerenciamento do FreeRTOS.
 */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define L293D_LATCH_Pin GPIO_PIN_6
#define L293D_LATCH_GPIO_Port GPIOA
#define L293D_PWM_Pin GPIO_PIN_7
#define L293D_PWM_GPIO_Port GPIOA
#define L293D_EN_Pin GPIO_PIN_8
#define L293D_EN_GPIO_Port GPIOA
#define L293D_SER_Pin GPIO_PIN_9
#define L293D_SER_GPIO_Port GPIOA
#define L293D_CLK_Pin GPIO_PIN_5
#define L293D_CLK_GPIO_Port GPIOB
#define SG90_PWM_Pin GPIO_PIN_6
#define SG90_PWM_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
