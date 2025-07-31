/*
 * ax5689_control.c
 *
 *  Created on: Jul 9, 2025
 *      Author: ncyu
 *  
 *  @brief AX5689 Audio Amplifier Control Implementation
 *         Provides OpenLoop and CloseLoop mode switching with GPIO control
 */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "ax5689_control.h"
#include "OpenLoop.h"
#include "CloseLoop.h"
#include "SEGGER_RTT.h"

/* Private defines -----------------------------------------------------------*/
#define RTT_printf(fmt, ...) SEGGER_RTT_printf(0, fmt, ##__VA_ARGS__)

/* External variables --------------------------------------------------------*/
extern I2C_HandleTypeDef hi2c2;

/* Global variables ----------------------------------------------------------*/
AX5689_ConfigMode_t current_config_mode = AX5689_OPENLOOP_MODE;

/* Private function prototypes -----------------------------------------------*/
static void AX5689_PowerSequence_Init(void);
static void AX5689_EnableController(void);
static void AX5689_DisableController(void);

/* Public Functions ----------------------------------------------------------*/

/**
 * @brief  Initialize GPIO pins for AX5689 and power stage control
 * @note   Based on official Step 2: Initialize I/O
 */
void IO_Init(void) 
{
    // Initialize RTT for debug output
    SEGGER_RTT_Init();
    
    RTT_printf("=== Step 2: Initialize I/O ===\r\n");
    
    // Following official Axign documentation sequence:
    // SetPin(AX_MUTE_N, LOW); // 将放大器置于静音状态（音量为 0）
    HAL_GPIO_WritePin(AX_Mute_N_GPIO_Port, AX_Mute_N_Pin, GPIO_PIN_RESET);
    RTT_printf("AX_MUTE_N set to LOW (amplifier muted)\r\n");
    
    // SetPin(PS_RESET_N, LOW); // 将功率级置于高阻态 (Hi-Z)
    // Note: PS_RESET_N corresponds to power stage reset, using available power control pins
    HAL_GPIO_WritePin(D5V2_EN_GPIO_Port, D5V2_EN_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(PVDD_EN_GPIO_Port, PVDD_EN_Pin, GPIO_PIN_RESET);
    RTT_printf("Power stage set to Hi-Z state\r\n");
    
    // delay(20); // 等待信号稳定 (ms)
    HAL_Delay(20);
    RTT_printf("Signal stabilization delay: 20ms\r\n");
    
    // SetPin(AX_RESET_N, LOW); // 禁用 Axign 控制器
    HAL_GPIO_WritePin(AX_Reset_N_GPIO_Port, AX_Reset_N_Pin, GPIO_PIN_RESET);
    RTT_printf("AX_RESET_N set to LOW (Axign controller disabled)\r\n");
    
    RTT_printf("I/O initialization completed according to official documentation.\r\n\r\n");
}

/**
 * @brief  Read 16-bit data from AX5689 register
 * @param  regAddr: Register address to read from
 * @param  data: Pointer to store the read 16-bit data
 * @retval HAL status
 */
HAL_StatusTypeDef AX5689_Read(uint16_t regAddr, uint16_t *data) 
{
    HAL_StatusTypeDef status = HAL_ERROR;
    uint8_t buffer[2];

    // Convert 16-bit register address to two 8-bit bytes
    buffer[0] = (regAddr >> 8) & 0xFF;
    buffer[1] = regAddr & 0xFF;

    // Send register address
    status = HAL_I2C_Master_Transmit(&hi2c2, AX5689_I2C_ADDR, buffer, 2, HAL_MAX_DELAY);
    if (status == HAL_OK) {
        // Receive data
        status = HAL_I2C_Master_Receive(&hi2c2, AX5689_I2C_ADDR, buffer, 2, HAL_MAX_DELAY);
        if (status == HAL_OK) {
            *data = ((uint16_t)buffer[0] << 8) | (uint16_t)buffer[1];
        }
    }
    
    return status;
}

/**
 * @brief  Write 16-bit data to AX5689 register
 * @param  regAddr: Register address to write to
 * @param  regVal: 16-bit value to write
 * @retval HAL status
 */
HAL_StatusTypeDef AX5689_Write(uint16_t regAddr, uint16_t regVal) 
{
    HAL_StatusTypeDef status;
    uint8_t data[4];

    // Format data: [RegAddr_MSB, RegAddr_LSB, Data_MSB, Data_LSB]
    data[0] = (regAddr >> 8) & 0xFF;
    data[1] = regAddr & 0xFF;
    data[2] = (regVal >> 8) & 0xFF;
    data[3] = regVal & 0xFF;

    // Debug output - show what we're writing
    RTT_printf("Writing reg 0x%04X = 0x%04X\r\n", regAddr, regVal);

    // Send complete transaction
    status = HAL_I2C_Master_Transmit(&hi2c2, AX5689_I2C_ADDR, data, 4, 1000);
    
    return status;
}

/**
 * @brief  Write a complete register configuration set to AX5689
 * @param  ptrConfigArray: Pointer to configuration array
 * @note   Based on official AX568X_WriteRegisterSet implementation
 */
void AX5689_WriteRegisterSet(const uint16_t *ptrConfigArray) 
{
    RTT_printf("Starting register configuration according to official documentation...\r\n");

    // 循环直到数组命令结束
    // Loop until end of array commands
    while (*ptrConfigArray != 0xFFFF) {
        uint16_t regAddr = ptrConfigArray[0];
        uint16_t regVal = ptrConfigArray[1];

        // AX568X_Write(axAddrI2C, ptrConfigArray[0], ptrConfigArray[1]);
        AX5689_Write(regAddr, regVal);
        HAL_Delay(1); // Small delay between register writes
        
        // ptrConfigArray += 2; // 转到数组中的下一个地址
        // Move to next address in array
        ptrConfigArray += 2;
    }

    RTT_printf("Register configuration completed according to official documentation.\r\n");
}

/**
 * @brief  Setup AX5689 with current configuration mode
 * @note   Based on official Step 4: Set up AX5688/AX5689
 */
void AX5689_Setup(void) 
{
    RTT_printf("=== Step 4: Set up AX5688/AX5689 ===\r\n");
    
    // SetPin(AX_RESET_N, HIGH); // 启用 AX5688 或 AX5689
    HAL_GPIO_WritePin(AX_Reset_N_GPIO_Port, AX_Reset_N_Pin, GPIO_PIN_SET);
    RTT_printf("AX_RESET_N set to HIGH (Enable AX5689)\r\n");
    
    // AX568X_WriteRegisterSet(axAddrI2C, &amplifierCommands[0]); 
    // 从支持文件中写入"寄存器设置"
    if (current_config_mode == AX5689_CLOSELOOP_MODE) {
        RTT_printf("Writing CloseLoop register configuration...\r\n");
        AX5689_WriteRegisterSet(amplifierCommands_CloseLoop);
    } else {
        RTT_printf("Writing OpenLoop register configuration...\r\n");
        AX5689_WriteRegisterSet(amplifierCommands);
    }
    
    // delay(500); // 系统稳定时间 (ms)
    HAL_Delay(500);
    RTT_printf("System stabilization delay: 500ms\r\n");
    RTT_printf("AX5689 setup completed according to official documentation.\r\n\r\n");
}

/**
 * @brief  Start the AX5689 control loop
 * @note   Follows official Axign startup sequence to minimize pop noise
 */
void StartControlLoop(void) 
{
    // Get current mode values
    uint16_t reg01_value, reg03_value, reg07_value, reg07_default;
    if (current_config_mode == AX5689_CLOSELOOP_MODE) {
        reg01_value = 0xFFFF;  // REG01_VALUE from CloseLoop
        reg03_value = 0x1155;  // REG03_VALUE from CloseLoop  
        reg07_value = 0x0001;  // REG07_VALUE from CloseLoop
        reg07_default = 0x0064; // REG07_VALUE_DEFAULT
    } else {
        reg01_value = 0xFFFF;  // REG01_VALUE from OpenLoop
        reg03_value = 0x5500;  // REG03_VALUE from OpenLoop
        reg07_value = 0x0001;  // REG07_VALUE from OpenLoop
        reg07_default = 0x0064; // REG07_VALUE_DEFAULT
    }

    RTT_printf("Starting control loop in %s mode...\r\n", 
               (current_config_mode == AX5689_CLOSELOOP_MODE) ? "CloseLoop" : "OpenLoop");

    // Step 1: Disable all loop filters and PWM modulators
    AX5689_Write(0x0001, 0xFF00); // REG01_VALUE_DEFAULT
    
    // Step 2: Set volume ramp time to fast
    AX5689_Write(0x0007, reg07_value);
    
    // Step 3: Set Pstart to 'high' to enable power stage
    AX5689_Write(0x0002, 0x1FFF); // REG02_PSTART_ENABLE
    
    // Step 4: Enable all loop filters and PWM modulators
    AX5689_Write(0x0001, reg01_value);
    
    // Step 5: Enable external ADC feedback (mode-specific)
    AX5689_Write(0x0003, reg03_value);
    
    // Step 6: Wait 1ms
    HAL_Delay(1);
    
    // Step 7: Unmute amplifier
    HAL_GPIO_WritePin(AX_Mute_N_GPIO_Port, AX_Mute_N_Pin, GPIO_PIN_SET);
    
    // Step 8: Wait for volume ramp up (20ms)
    HAL_Delay(20);
    
    // Step 9: Set volume ramp time to default value
    AX5689_Write(0x0007, reg07_default);
    
    RTT_printf("Control loop started successfully.\r\n");
}

/**
 * @brief  Stop the AX5689 control loop
 * @note   Follows official Axign shutdown sequence to minimize pop noise
 */
void StopControlLoop(void) 
{
    RTT_printf("Stopping control loop...\r\n");
    
    // Step 1: Set volume ramp time to fast
    AX5689_Write(0x0007, 0x0001); // REG07_VALUE (fast ramp)
    
    // Step 2: Mute amplifier (volume to 0)
    HAL_GPIO_WritePin(AX_Mute_N_GPIO_Port, AX_Mute_N_Pin, GPIO_PIN_RESET);
    
    // Step 3: Wait for volume ramp down (20ms)
    HAL_Delay(20);
    
    // Step 4: Set Pstart to 'low' to disable power stage
    AX5689_Write(0x0002, 0x17FF); // REG02_PSTART_DISABLE
    
    // Step 5: Wait for tristate to go low (20ms)
    HAL_Delay(20);
    
    // Step 6: Disable/reset all loop filters and PWM modulators
    AX5689_Write(0x0001, 0xFF00); // REG01_VALUE_DEFAULT
    
    // Step 7: Set volume ramp time to default value
    AX5689_Write(0x0007, 0x0064); // REG07_VALUE_DEFAULT
    
    // Step 8: Restore all ramps to default values
    AX5689_Write(0x0003, 0x0000); // REG03_VALUE_DEFAULT
    
    // Step 9: Reset status registers
    clearStatusRegisters();
    
    RTT_printf("Control loop stopped successfully.\r\n");
}

/**
 * @brief  Clear AX5689 status registers
 * @note   Read all status registers to clear status pin
 */
void clearStatusRegisters(void) 
{
    uint16_t temp;
    
    // Read all status registers to clear them
    AX5689_Read(0x003C, &temp); // Read register 60 (0x3C)
    AX5689_Read(0x003D, &temp); // Read register 61 (0x3D) 
    AX5689_Read(0x003E, &temp); // Read register 62 (0x3E)
    
    RTT_printf("Status registers cleared.\r\n");
}

/**
 * @brief  Check AX5689 status register for errors
 */
void checkStatusRegister(void) 
{
    uint16_t statusValue;
    static uint32_t lastCheckTime = 0;
    uint32_t currentTime = HAL_GetTick();
    
    // Check status every 1000ms to avoid flooding
    if (currentTime - lastCheckTime > 1000) {
        lastCheckTime = currentTime;
        
/*         if (AX5689_Read(0x0010, &statusValue) == HAL_OK) {
            if (statusValue != 0x0000) {
                RTT_printf("Status register warning: 0x%04X\r\n", statusValue);
            }
        } */
    }
}

/* Mode Management Functions ------------------------------------------------ */

/**
 * @brief  Set the current configuration mode
 * @param  mode: Target configuration mode
 */
void AX5689_SetConfigMode(AX5689_ConfigMode_t mode) 
{
    current_config_mode = mode;
}

/**
 * @brief  Get the current configuration mode
 * @retval Current configuration mode
 */
AX5689_ConfigMode_t AX5689_GetConfigMode(void) 
{
    return current_config_mode;
}

/**
 * @brief  Switch AX5689 to OpenLoop mode with proper stop/start sequence
 */
void AX5689_SwitchToOpenLoop(void) 
{
    RTT_printf("=== Switching to OpenLoop mode ===\r\n");
    
    // Step 1: Stop current control loop properly
    StopControlLoop();
    
    // Step 2: Set new configuration mode
    AX5689_SetConfigMode(AX5689_OPENLOOP_MODE);
    
    // Step 3: Write new register configuration
    AX5689_Setup();
    
    // Step 4: Start control loop with new configuration
    StartControlLoop();
    
    // Step 5: Verify mode switch by reading register 0x0003
    uint16_t reg03_value;
    if (AX5689_Read(0x0003, &reg03_value) == HAL_OK) {
        RTT_printf("Register 0x0003 = 0x%04X (expected 0x5500 for OpenLoop)\r\n", reg03_value);
        if (reg03_value == 0x5500) {
            HAL_GPIO_WritePin(LED_ON_OFF_GPIO_Port, LED_ON_OFF_Pin, GPIO_PIN_RESET);
            RTT_printf("✓ OpenLoop mode confirmed!\r\n");
        } else {
            RTT_printf("⚠ OpenLoop mode verification failed!\r\n");
        }
    }
    
    RTT_printf("OpenLoop mode switch completed.\r\n\r\n");
}

/**
 * @brief  Switch AX5689 to CloseLoop mode with proper stop/start sequence
 */
void AX5689_SwitchToCloseLoop(void) 
{
    RTT_printf("=== Switching to CloseLoop mode ===\r\n");
    
    // Step 1: Stop current control loop properly
    StopControlLoop();
    
    // Step 2: Set new configuration mode
    AX5689_SetConfigMode(AX5689_CLOSELOOP_MODE);
    
    // Step 3: Write new register configuration
    AX5689_Setup();
    
    // Step 4: Start control loop with new configuration
    StartControlLoop();
    
    // Step 5: Verify mode switch by reading register 0x0003
    uint16_t reg03_value;
    if (AX5689_Read(0x0003, &reg03_value) == HAL_OK) {
        RTT_printf("Register 0x0003 = 0x%04X (expected 0x1155 for CloseLoop)\r\n", reg03_value);
        if (reg03_value == 0x1155) {
            HAL_GPIO_WritePin(LED_ON_OFF_GPIO_Port, LED_ON_OFF_Pin, GPIO_PIN_SET);
            RTT_printf("✓ CloseLoop mode confirmed!\r\n");
        } else {
            RTT_printf("⚠ CloseLoop mode verification failed!\r\n");
        }
    }
    
    RTT_printf("CloseLoop mode switch completed.\r\n\r\n");
}

/* Private Functions ---------------------------------------------------------*/

/**
 * @brief  Initialize power sequence for AX5689
 */
static void AX5689_PowerSequence_Init(void) 
{
    // Step 1: Complete reset and power down
    HAL_GPIO_WritePin(AX_Reset_N_GPIO_Port, AX_Reset_N_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(AX_Mute_N_GPIO_Port, AX_Mute_N_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(D1V2_EN_GPIO_Port, D1V2_EN_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(AX_3V3_GPIO_Port, AX_3V3_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(AX_5V_GPIO_Port, AX_5V_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(D5V2_EN_GPIO_Port, D5V2_EN_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(PVDD_EN_GPIO_Port, PVDD_EN_Pin, GPIO_PIN_RESET);
    
    // Step 2: Wait for complete power down
    HAL_Delay(100);
    
    // Step 3: Core power supplies first
    HAL_GPIO_WritePin(D1V2_EN_GPIO_Port, D1V2_EN_Pin, GPIO_PIN_SET);
    HAL_Delay(20);
    
    HAL_GPIO_WritePin(AX_3V3_GPIO_Port, AX_3V3_Pin, GPIO_PIN_SET);
    HAL_Delay(30);
    
    // Step 4: Release reset after core power is stable
    HAL_GPIO_WritePin(AX_Reset_N_GPIO_Port, AX_Reset_N_Pin, GPIO_PIN_SET);
    HAL_Delay(100);
    
    // Step 5: Additional startup time for I2C readiness
    HAL_Delay(50);
}

/**
 * @brief  Enable AX5689 controller
 */
static void AX5689_EnableController(void) 
{
    HAL_GPIO_WritePin(AX_Reset_N_GPIO_Port, AX_Reset_N_Pin, GPIO_PIN_SET);
    HAL_Delay(100);
}

/**
 * @brief  Disable AX5689 controller
 */
static void AX5689_DisableController(void) 
{
    HAL_GPIO_WritePin(AX_Reset_N_GPIO_Port, AX_Reset_N_Pin, GPIO_PIN_RESET);
    HAL_Delay(50);
}
