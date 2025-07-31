/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : AX5689 Audio Amplifier Control with GPIO Mode Switching
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

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include <stdio.h>
#include "SEGGER_RTT.h"
#include "ax5689_control.h"

/* Private defines -----------------------------------------------------------*/
// RTT printf macro definition for debug output
#define RTT_printf(fmt, ...) SEGGER_RTT_printf(0, fmt, ##__VA_ARGS__)

// Initial configuration mode: 1=CloseLoop, 0=OpenLoop
#define USE_CLOSELOOP_MODE 0

// GPIO debounce configuration
#define DEBOUNCE_TIME_MS    10    // 10ms debounce time
#define MAIN_LOOP_DELAY_MS  1     // 1ms main loop interval

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c2;
SPI_HandleTypeDef hspi2;
UART_HandleTypeDef huart1;

/* Private variables for GPIO mode switching --------------------------------*/
static GPIO_PinState mode_pin_prev_state = GPIO_PIN_SET;  // External pull-up, initial state is HIGH
static GPIO_PinState mode_pin_current_state;
static uint32_t mode_switch_debounce_counter = 0;

/* Global variables ----------------------------------------------------------*/
volatile HAL_StatusTypeDef i2c_status = HAL_OK;

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C2_Init(void);
static void MX_SPI2_Init(void);
static void MX_USART1_UART_Init(void);

/* Private functions for GPIO mode switching --------------------------------*/
static void GPIO_ModeSwitch_Init(void);
static void GPIO_ModeSwitch_Process(void);
static void AX5689_InitialSetup(void);

#ifdef __GNUC__
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif 
/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_I2C2_Init();
  MX_SPI2_Init();
  MX_USART1_UART_Init();

  /* USER CODE BEGIN 2 */
  
  // Initialize AX5689 and RTT debug system
  AX5689_InitialSetup();
  
  // Initialize GPIO mode switching
  GPIO_ModeSwitch_Init();

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  
  SEGGER_RTT_WriteString(0, "=== AX5689 GPIO Mode Switching System Started ===\r\n");
  SEGGER_RTT_WriteString(0, "Mode_Select Pin (PC9): LOW pulse = switch mode\r\n");
  SEGGER_RTT_WriteString(0, "System ready for operation.\r\n\r\n");
  
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
    
    // Check AX5689 status register
    checkStatusRegister();
    
    // Process GPIO mode switching
    GPIO_ModeSwitch_Process();
    
    // Main loop delay
    HAL_Delay(MAIN_LOOP_DELAY_MS);
    
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV1;
  RCC_OscInitStruct.PLL.PLLN = 16;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief I2C2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C2_Init(void)
{

  /* USER CODE BEGIN I2C2_Init 0 */

  /* USER CODE END I2C2_Init 0 */

  /* USER CODE BEGIN I2C2_Init 1 */

  /* USER CODE END I2C2_Init 1 */
  hi2c2.Instance = I2C2;
  hi2c2.Init.Timing = 0x00503D58;
  hi2c2.Init.OwnAddress1 = 0;
  hi2c2.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c2.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c2.Init.OwnAddress2 = 0;
  hi2c2.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c2.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c2.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c2) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Analogue filter
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c2, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Digital filter
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c2, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C2_Init 2 */

  /* USER CODE END I2C2_Init 2 */

}

/**
  * @brief SPI2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI2_Init(void)
{

  /* USER CODE BEGIN SPI2_Init 0 */

  /* USER CODE END SPI2_Init 0 */

  /* USER CODE BEGIN SPI2_Init 1 */

  /* USER CODE END SPI2_Init 1 */
  /* SPI2 parameter configuration*/
  hspi2.Instance = SPI2;
  hspi2.Init.Mode = SPI_MODE_MASTER;
  hspi2.Init.Direction = SPI_DIRECTION_2LINES;
  hspi2.Init.DataSize = SPI_DATASIZE_4BIT;
  hspi2.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi2.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi2.Init.NSS = SPI_NSS_HARD_OUTPUT;
  hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
  hspi2.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi2.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi2.Init.CRCPolynomial = 7;
  hspi2.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
  hspi2.Init.NSSPMode = SPI_NSS_PULSE_ENABLE;
  if (HAL_SPI_Init(&hspi2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI2_Init 2 */

  /* USER CODE END SPI2_Init 2 */

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart1.Init.ClockPrescaler = UART_PRESCALER_DIV1;
  huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetTxFifoThreshold(&huart1, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetRxFifoThreshold(&huart1, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_DisableFifoMode(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, D1V2_EN_Pin|AX_3V3_Pin|AX_5V_Pin|D5V2_EN_Pin
                          |AX_Mute_N_Pin|LED_ON_OFF_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, AX_Reset_N_Pin|AX_State_Pin|PVDD_EN_Pin|LED_Display_CS_Pin
                          |LED_Display_Data_Pin|MCU_Pstart_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : D1V2_EN_Pin AX_3V3_Pin AX_5V_Pin D5V2_EN_Pin
                           AX_Mute_N_Pin LED_ON_OFF_Pin */
  GPIO_InitStruct.Pin = D1V2_EN_Pin|AX_3V3_Pin|AX_5V_Pin|D5V2_EN_Pin
                          |AX_Mute_N_Pin|LED_ON_OFF_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : AX_Reset_N_Pin AX_State_Pin */
  GPIO_InitStruct.Pin = AX_Reset_N_Pin|AX_State_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PVDD_EN_Pin LED_Display_CS_Pin LED_Display_Data_Pin MCU_Pstart_Pin */
  GPIO_InitStruct.Pin = PVDD_EN_Pin|LED_Display_CS_Pin|LED_Display_Data_Pin|MCU_Pstart_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : LED_Display_CLK_Pin */
  GPIO_InitStruct.Pin = LED_Display_CLK_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(LED_Display_CLK_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : Mode_Select_Pin */
  GPIO_InitStruct.Pin = Mode_Select_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(Mode_Select_GPIO_Port, &GPIO_InitStruct);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/**
  * @brief  Initialize AX5689 amplifier with initial configuration
  * @param  None
  * @retval None
  */
static void AX5689_InitialSetup(void)
{
  // Initialize AX5689 related I/O and RTT
  IO_Init();
  
  // Set initial configuration mode
#if USE_CLOSELOOP_MODE
  AX5689_SetConfigMode(AX5689_CLOSELOOP_MODE);
  SEGGER_RTT_WriteString(0, "Initial mode: CloseLoop\r\n");
#else
  AX5689_SetConfigMode(AX5689_OPENLOOP_MODE);
  SEGGER_RTT_WriteString(0, "Initial mode: OpenLoop\r\n");
#endif
  
  // Setup and start AX5689
  AX5689_Setup();
  StartControlLoop();
  
  RTT_printf("AX5689 initialization completed.\r\n");
}

/**
  * @brief  Initialize GPIO mode switching variables and state
  * @param  None
  * @retval None
  */
static void GPIO_ModeSwitch_Init(void)
{
  // Initialize GPIO state variables
  mode_pin_prev_state = GPIO_PIN_SET;  // External pull-up, initial state is HIGH
  mode_pin_current_state = GPIO_PIN_SET;
  mode_switch_debounce_counter = 0;
  
  RTT_printf("GPIO mode switching initialized.\r\n");
}

/**
  * @brief  Process GPIO mode switching logic with debouncing
  * @param  None
  * @retval None
  */
static void GPIO_ModeSwitch_Process(void)
{
  // Read current GPIO state
  mode_pin_current_state = HAL_GPIO_ReadPin(Mode_Select_GPIO_Port, Mode_Select_Pin);
  
  // Detect falling edge (HIGH to LOW transition)
  if (mode_pin_prev_state == GPIO_PIN_SET && mode_pin_current_state == GPIO_PIN_RESET) {
    mode_switch_debounce_counter = DEBOUNCE_TIME_MS;
    RTT_printf("Mode switch signal detected, debouncing...\r\n");
  }
  
  // Debounce processing
  if (mode_switch_debounce_counter > 0) {
    mode_switch_debounce_counter--;
    
    if (mode_switch_debounce_counter == 0) {
      // Confirm signal is stable, execute mode switch
      if (HAL_GPIO_ReadPin(Mode_Select_GPIO_Port, Mode_Select_Pin) == GPIO_PIN_RESET) {
        RTT_printf("Confirmed mode switch request!\r\n");
        
        // Execute mode switching
        if (AX5689_GetConfigMode() == AX5689_OPENLOOP_MODE) {
          AX5689_SwitchToCloseLoop();
        } else {
          AX5689_SwitchToOpenLoop();
        }
        
        RTT_printf("Mode switched successfully!\r\n");
      }
    }
  }
  
  // Update previous state
  mode_pin_prev_state = mode_pin_current_state;
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: RTT_printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
