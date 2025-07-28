/*
 * ax5689_control.h
 *
 *  Created on: Jul 9, 2025
 *      Author: ncyu
 */

#ifndef SRC_AX5689_CONTROL_H_
#define SRC_AX5689_CONTROL_H_

#include "main.h"


#define AX5689_I2C_ADDR (0x39 << 1)
#include "AXN050-AX5689_register_settings_hybrid_HiRes_PWM_BD.h"

// 函数原型声明
void IO_Init(void);
HAL_StatusTypeDef AX5689_Read(uint16_t regAddr, uint16_t *data);
HAL_StatusTypeDef AX5689_Write(uint16_t regAddr, uint16_t regVal);
void AX5689_WriteRegisterSet(const uint16_t *ptrConfigArray);
void AX5689_Setup(void);
void StartControlLoop(void);
void StopControlLoop(void);
void clearStatusRegisters(void);
void checkStatusRegister(void);

#endif /* __AX5689_CONTROL_H */



