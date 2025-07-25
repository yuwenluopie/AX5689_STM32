/*
 * ax5689_control.c
 *
 *  Created on: Jul 9, 2025
 *      Author: ncyu
 */


#include "ax5689_control.h"
#include <stdio.h>

extern I2C_HandleTypeDef hi2c2;

float PVDD = 50;
uint32_t SAMPLE_RATE = 192000;
float UVP = 45;
float OVP = 55;
uint16_t VolumeHex = 0x8FF6;
/**
 * @brief  初始化与AX5689和电源级相关的GPIO引脚.
 * @note   根据PDF Step 2: Initialize I/O.
 */
void IO_Init(void) {
    // 根据PDF P13, IO_Init示例
    // 将功放静音 (AX_Mute_N_Pin -> PC6)
    HAL_GPIO_WritePin(AX_Mute_N_GPIO_Port, AX_Mute_N_Pin, GPIO_PIN_RESET);

    // 将电源级置于高阻态 (Hi-Z), 假设您有一个PS_RESET_N引脚, 这里用一个通用输出来示意
    // HAL_GPIO_WritePin(PS_RESET_N_GPIO_Port, PS_RESET_N_Pin, GPIO_PIN_RESET);

    HAL_Delay(20); // 等待信号稳定

    // 禁用Axign控制器 (AX_Reset_N_Pin -> PA8)
    HAL_GPIO_WritePin(AX_Reset_N_GPIO_Port, AX_Reset_N_Pin, GPIO_PIN_RESET);
    printf("IO Initialized.\r\n");
}

/**
 * @brief  从AX5689寄存器读取16位数据.
 * @param  regAddr: 寄存器地址.
 * @param  data: 指向存储读取数据的16位变量的指针.
 * @retval HAL status.
 */
HAL_StatusTypeDef AX5689_Read(uint16_t regAddr, uint16_t *data) {
    HAL_StatusTypeDef status = HAL_ERROR;
    uint8_t buffer[2];

    // 将16位寄存器地址转换为两个8位字节
    buffer[0] = (regAddr >> 8) & 0xFF;
    buffer[1] = regAddr & 0xFF;

    // 发送寄存器地址
    status = HAL_I2C_Master_Transmit(&hi2c2, AX5689_I2C_ADDR, buffer, 2, HAL_MAX_DELAY);
    if (status == HAL_OK) {
        // 接收数据
        status = HAL_I2C_Master_Receive(&hi2c2, AX5689_I2C_ADDR, buffer, 2, HAL_MAX_DELAY);
        if (status == HAL_OK) {
            *data = ((uint16_t)buffer[0] << 8) | (uint16_t)buffer[1];
        }
    }
    return status;
}

/**
 * @brief  向AX5689寄存器写入16位数据.
 * @param  regAddr: 寄存器地址.
 * @param  regVal: 要写入的16位数据.
 * @retval HAL status.
 */
HAL_StatusTypeDef AX5689_Write(uint16_t regAddr, uint16_t regVal) {
    HAL_StatusTypeDef status;
    uint8_t data[4];

    // 格式化数据: [RegAddr_MSB, RegAddr_LSB, Data_MSB, Data_LSB]
    data[0] = (regAddr >> 8) & 0xFF;
    data[1] = regAddr & 0xFF;
    data[2] = (regVal >> 8) & 0xFF;
    data[3] = regVal & 0xFF;

    status = HAL_I2C_Master_Transmit(&hi2c2, AX5689_I2C_ADDR, data, 4, HAL_MAX_DELAY);
    return status;
}

/**
 * @brief  将寄存器配置数组写入AX5689.
 * @param  ptrConfigArray: 指向配置数组的指针.
 */
void AX5689_WriteRegisterSet(const uint16_t *ptrConfigArray) {
    // 循环直到遇到数组结束命令0xFFFF
    while (*ptrConfigArray != 0xFFFF) {
        uint16_t regAddr = ptrConfigArray[0];
        uint16_t regVal = ptrConfigArray[1];
        if (AX5689_Write(regAddr, regVal) != HAL_OK) {
            printf("Error writing register 0x%04X\r\n", regAddr);
            // 可以加入错误处理
        }
        ptrConfigArray += 2; // 移动到数组中的下一个地址/数据对
    }
}

/**
 * @brief  设置AX5689控制器.
 * @note   根据PDF Step 4: Setup the AX5688/AX5689.
 */
void AX5689_Setup(void) {
    // 使能AX5689控制器 (AX_Reset_N_Pin -> PA8)
    HAL_GPIO_WritePin(AX_Reset_N_GPIO_Port, AX_Reset_N_Pin, GPIO_PIN_SET);
    printf("AX5689 Enabled (Reset pin HIGH).\r\n");

    // 等待芯片稳定
    HAL_Delay(10);

    // 从您的头文件写入寄存器设置
    printf("Writing register settings...\r\n");
    AX5689_WriteRegisterSet(&amplifierCommands[0]);
    printf("Register settings written.\r\n");

    // 系统稳定时间
    HAL_Delay(500);
}

/**
 * @brief  启动Axign控制循环.
 * @note   严格遵循PDF P15的流程以避免pop噪音.
 */
void StartControlLoop(void) {
    printf("Starting control loop...\r\n");

    // 禁用所有环路滤波器和PWM调制器
    AX5689_Write(0x0001, REG01_VALUE_DEFAULT);
    // 设置音量斜坡时间为快速
    AX5689_Write(0x0007, REG07_VALUE);
    // 设置PSTART为高电平以使能电源级
    AX5689_Write(0x0002, REG02_PSTART_ENABLE);
    // 使能所有环路滤波器和PWM调制器
    AX5689_Write(0x0001, REG01_VALUE);
    // 使能外部ADC反馈
    AX5689_Write(0x0003, REG03_VALUE);

    HAL_Delay(1);

    // 取消静音 (AX_Mute_N_Pin -> PC6)
    HAL_GPIO_WritePin(AX_Mute_N_GPIO_Port, AX_Mute_N_Pin, GPIO_PIN_SET);
    printf("Amplifier Unmuted.\r\n");

    // 等待音量斜坡上升
    HAL_Delay(20);

    // 将音量斜坡时间设回默认值
    AX5689_Write(0x0007, REG07_VALUE_DEFAULT);
    printf("Control loop started.\r\n");
}

/**
 * @brief  停止Axign控制循环.
 * @note   严格遵循PDF P16的流程以最小化关机pop噪音.
 */
void StopControlLoop(void) {
    printf("Stopping control loop...\r\n");

    // 设置音量斜坡时间为快速
    AX5689_Write(0x0007, REG07_VALUE);
    // 功放静音 (AX_Mute_N_Pin -> PC6)
    HAL_GPIO_WritePin(AX_Mute_N_GPIO_Port, AX_Mute_N_Pin, GPIO_PIN_RESET);
    printf("Amplifier Muted.\r\n");

    // 等待音量斜坡下降
    HAL_Delay(20);

    // 设置PSTART为低电平以禁用电源级
    AX5689_Write(0x0002, REG02_PSTART_DISABLE);
    // 等待三态变为低电平
    HAL_Delay(20);

    // 禁用/复位所有环路滤波器和PWM调制器
    AX5689_Write(0x0001, REG01_VALUE_DEFAULT);
    // 设置音量斜坡时间为默认值
    AX5689_Write(0x0007, REG07_VALUE_DEFAULT);
    // 将所有斜坡设回其值
    AX5689_Write(0x0003, REG03_VALUE_DEFAULT);

    // 清除状态寄存器
    clearStatusRegisters();
    printf("Control loop stopped.\r\n");
}

/**
 * @brief 
 */
void clearStatusRegisters(void) {
    uint16_t temp;
    AX5689_Read(60, &temp);
    AX5689_Read(61, &temp);
    AX5689_Read(62, &temp);
    printf("Status registers cleared.\r\n");
}

/**
 * @brief 检查AX5689的状态寄存器并在发生故障时处理.
 * @note  此函数应在主循环中定期调用.
 */
void checkStatusRegister(void) {
    // 假设AX_STATUS连接到MCU的某个输入引脚 (例如 PA10)
    // if (HAL_GPIO_ReadPin(AX_STATUS_GPIO_Port, AX_STATUS_Pin) == GPIO_PIN_SET) {
    // 简单的轮询实现
    uint16_t reg62val;
    if (AX5689_Read(62, &reg62val) == HAL_OK) {
        // 检查 sys_fault 位 (bit 5)
        if ((reg62val & (1 << 5)) > 0) {
            printf("System fault detected! Restarting control loop.\r\n");

            // 停止控制循环
            StopControlLoop();

            // 根据PDF P17, 复位PFAULT_N掩码以清除PSTART锁存
            AX5689_Write(56, REG56_RESET_VALUE);
            // 将PFAULT_N掩码设置回粘滞模式
            AX5689_Write(56, REG56_VALUE);

            // 清除状态寄存器
            clearStatusRegisters();

            // 等待电源级在故障后稳定
            HAL_Delay(100);

            // 重新检查故障位是否已清除
            AX5689_Read(62, &reg62val);
            if ((reg62val & (1 << 5)) == 0) {
                 printf("Fault cleared. Restarting...\r\n");
                 StartControlLoop();
            } else {
                 printf("Fault persists. System halted.\r\n");
                 // 在这里可以执行永久性关机或错误指示
            }
        }
    }
}
