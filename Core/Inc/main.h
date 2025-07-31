/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32g0xx_hal.h"

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

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define D1V2_EN_Pin GPIO_PIN_11
#define D1V2_EN_GPIO_Port GPIOC
#define AX_3V3_Pin GPIO_PIN_12
#define AX_3V3_GPIO_Port GPIOC
#define AX_5V_Pin GPIO_PIN_13
#define AX_5V_GPIO_Port GPIOC
#define D5V2_EN_Pin GPIO_PIN_14
#define D5V2_EN_GPIO_Port GPIOC
#define AX_Mute_N_Pin GPIO_PIN_0
#define AX_Mute_N_GPIO_Port GPIOC
#define AX_Reset_N_Pin GPIO_PIN_0
#define AX_Reset_N_GPIO_Port GPIOA
#define AX_State_Pin GPIO_PIN_1
#define AX_State_GPIO_Port GPIOA
#define PVDD_EN_Pin GPIO_PIN_2
#define PVDD_EN_GPIO_Port GPIOA
#define LED_Display_CS_Pin GPIO_PIN_4
#define LED_Display_CS_GPIO_Port GPIOA
#define LED_Display_CLK_Pin GPIO_PIN_5
#define LED_Display_CLK_GPIO_Port GPIOA
#define LED_Display_Data_Pin GPIO_PIN_7
#define LED_Display_Data_GPIO_Port GPIOA
#define MCU_Pstart_Pin GPIO_PIN_8
#define MCU_Pstart_GPIO_Port GPIOA
#define LED_ON_OFF_Pin GPIO_PIN_8
#define LED_ON_OFF_GPIO_Port GPIOC
#define Mode_Select_Pin GPIO_PIN_9
#define Mode_Select_GPIO_Port GPIOC

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
