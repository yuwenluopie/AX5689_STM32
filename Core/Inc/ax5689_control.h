/*
 * ax5689_control.h
 *
 *  Created on: Jul 9, 2025
 *      Author: ncyu
 *  
 *  @brief AX5689 Audio Amplifier Control Library
 *         Supports OpenLoop and CloseLoop mode switching with GPIO control
 */

#ifndef AX5689_CONTROL_H_
#define AX5689_CONTROL_H_

#include "stm32g0xx_hal.h"

/* AX5689 Configuration Constants ------------------------------------------- */
#define AX5689_I2C_ADDR (0x39 << 1)  // AX5689 I2C slave address (0x72 in 8-bit format)
// Note: AX5689 datasheet specifies 7-bit address 0x39, which becomes 0x72 when left-shifted

/* Type Definitions --------------------------------------------------------- */
/**
 * @brief AX5689 Configuration Mode Enumeration
 */
typedef enum {
    AX5689_OPENLOOP_MODE = 0,   ///< OpenLoop mode for basic amplification
    AX5689_CLOSELOOP_MODE = 1   ///< CloseLoop mode for enhanced control
} AX5689_ConfigMode_t;

/* Global Variables --------------------------------------------------------- */
extern AX5689_ConfigMode_t current_config_mode;  ///< Current active configuration mode

/* External Configuration Variables ----------------------------------------- */
// OpenLoop Configuration (defined in OpenLoop.h)
extern const uint16_t amplifierCommands[];
extern float PVDD;
extern uint32_t SAMPLE_RATE;
extern float UVP;
extern float OVP;
extern uint16_t VolumeHex;

// CloseLoop Configuration (defined in CloseLoop.h)
extern const uint16_t amplifierCommands_CloseLoop[];
extern float PVDD_CloseLoop;
extern uint32_t SAMPLE_RATE_CloseLoop;
extern float UVP_CloseLoop;
extern float OVP_CloseLoop;
extern uint16_t VolumeHex_CloseLoop;

/* Function Prototypes ------------------------------------------------------ */

/**
 * @brief Initialize GPIO pins for AX5689 and power stage control
 * @note  Based on PDF Step 2: Initialize I/O
 */
void IO_Init(void);

/**
 * @brief Read 16-bit data from AX5689 register
 * @param regAddr: Register address to read from
 * @param data: Pointer to store the read 16-bit data
 * @retval HAL status
 */
HAL_StatusTypeDef AX5689_Read(uint16_t regAddr, uint16_t *data);

/**
 * @brief Write 16-bit data to AX5689 register
 * @param regAddr: Register address to write to
 * @param regVal: 16-bit value to write
 * @retval HAL status
 */
HAL_StatusTypeDef AX5689_Write(uint16_t regAddr, uint16_t regVal);

/**
 * @brief Write a complete register configuration set to AX5689
 * @param ptrConfigArray: Pointer to configuration array
 */
void AX5689_WriteRegisterSet(const uint16_t *ptrConfigArray);

/**
 * @brief Setup AX5689 with current configuration mode
 */
void AX5689_Setup(void);

/**
 * @brief Start the AX5689 control loop
 */
void StartControlLoop(void);

/**
 * @brief Stop the AX5689 control loop
 */
void StopControlLoop(void);

/**
 * @brief Clear AX5689 status registers
 */
void clearStatusRegisters(void);

/**
 * @brief Check AX5689 status register for errors
 */
void checkStatusRegister(void);

/* Mode Management Functions ------------------------------------------------ */

/**
 * @brief Set the current configuration mode
 * @param mode: Target configuration mode
 */
void AX5689_SetConfigMode(AX5689_ConfigMode_t mode);

/**
 * @brief Get the current configuration mode
 * @retval Current configuration mode
 */
AX5689_ConfigMode_t AX5689_GetConfigMode(void);

/**
 * @brief Switch AX5689 to OpenLoop mode with register verification
 */
void AX5689_SwitchToOpenLoop(void);

/**
 * @brief Switch AX5689 to CloseLoop mode with register verification
 */
void AX5689_SwitchToCloseLoop(void);

#endif /* AX5689_CONTROL_H_ */
