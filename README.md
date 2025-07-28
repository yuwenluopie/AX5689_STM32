# AX5689 STM32 Audio Amplifier Control System

## 项目简介

这是一个基于STM32G0B0芯片和AX5689音频功率放大器控制器的音频放大器控制系统。项目实现了对AX5689芯片的完整控制，包括初始化、音量控制、状态监控和故障检测等功能。

## 系统架构

```
STM32G0B0 MCU
    ├── I2C2 ──────────► AX5689 控制器
    ├── SPI2 ──────────► LED 显示模块
    ├── UART1 ─────────► 调试输出
    └── GPIO ──────────► 控制信号
                           ├── AX_Reset_N (PA8)
                           ├── AX_Mute_N (PC6)
                           ├── PVDD_EN
                           └── 电源控制信号
```

## 硬件配置

### 主控芯片
- **MCU**: STM32G0B0RETX
- **时钟**: 外部HSE振荡器，PLL倍频到64MHz
- **调试接口**: SWD

### 音频放大器
- **芯片**: AX5689 数字音频功率放大器控制器
- **通信接口**: I2C (地址: 0x18)
- **音频输入**: I2S, 2通道, 192kHz采样率
- **输出配置**: 2x BTL (桥接负载)
- **输出滤波**: L=4.7μH, C=2.2μF
- **电源电压**: PVDD = 50V
- **ADC输入电阻**: 10kΩ (0.1%精度)

### 接口配置
- **I2C2**: 与AX5689通信
- **SPI2**: LED显示模块控制
- **UART1**: 调试信息输出 (115200, 8N1)

## 软件功能

### 核心功能
1. **系统初始化**
   - GPIO引脚配置
   - 时钟系统配置
   - 外设初始化 (I2C, SPI, UART)

2. **AX5689控制**
   - 芯片初始化和寄存器配置
   - 音量控制和斜坡调节
   - 静音/取消静音功能
   - 控制环路启动/停止

3. **状态监控**
   - 实时状态寄存器检查
   - 故障检测和自动恢复
   - 电源电压监控

4. **调试支持**
   - UART串口输出调试信息
   - 系统状态实时监控

### 分支说明
- **OpenLoop**: 开环控制版本，基础功能实现
- **CloseLoop**: 闭环控制版本，包含完整的反馈控制和故障处理

## 文件结构

```
MyGPIOtest/
├── Core/
│   ├── Inc/                                    # 头文件目录
│   │   ├── main.h                             # 主程序头文件
│   │   ├── ax5689_control.h                   # AX5689控制头文件
│   │   └── AXN050-AX5689_register_settings_*  # 寄存器配置文件
│   ├── Src/                                   # 源文件目录
│   │   ├── main.c                            # 主程序
│   │   ├── ax5689_control.c                  # AX5689控制实现
│   │   ├── stm32g0xx_hal_msp.c              # HAL MSP配置
│   │   ├── stm32g0xx_it.c                   # 中断处理
│   │   └── system_stm32g0xx.c               # 系统配置
│   └── Startup/
│       └── startup_stm32g0b0retx.s           # 启动文件
├── Drivers/                                   # STM32 HAL驱动
├── Debug/                                     # 调试输出目录
└── *.ld                                      # 链接脚本
```

## 快速开始

### 环境要求
- **IDE**: STM32CubeIDE 1.18.1 或更高版本
- **工具链**: ARM GCC
- **调试器**: ST-Link
- **硬件**: 基于STM32G0B0的开发板 + AX5689放大器模块

### 编译和下载
1. 在STM32CubeIDE中打开项目
2. 选择合适的分支 (OpenLoop/CloseLoop)
3. 编译项目: `Project` → `Build All`
4. 连接ST-Link调试器
5. 下载程序: `Run` → `Debug` 或 `Run`

### 串口调试
- **波特率**: 115200
- **数据位**: 8
- **停止位**: 1
- **校验位**: None
- **流控**: None

连接UART1到PC，可以看到系统运行状态和调试信息。

## 主要功能API

### 初始化函数
```c
void IO_Init(void);                    // GPIO初始化
void AX5689_Setup(void);              // AX5689芯片设置
```

### 控制函数
```c
void StartControlLoop(void);          // 启动控制环路
void StopControlLoop(void);           // 停止控制环路
void checkStatusRegister(void);       // 状态检查
```

### I2C通信函数
```c
HAL_StatusTypeDef AX5689_Read(uint16_t regAddr, uint16_t *data);
HAL_StatusTypeDef AX5689_Write(uint16_t regAddr, uint16_t regVal);
void AX5689_WriteRegisterSet(const uint16_t *ptrConfigArray);
```

## 配置参数

### 关键寄存器定义
```c
#define REG01_VALUE           0xFFFF    // 主通道使能
#define REG02_PSTART_ENABLE   0x1FFF    // 电源级使能
#define REG03_VALUE           0x5500    // 反馈控制
#define REG07_VALUE           0x0001    // 音量斜坡控制
```

### 系统参数
```c
float PVDD = 50;                      // 主电源电压 (V)
uint32_t SAMPLE_RATE = 192000;        // 音频采样率 (Hz)
float UVP = 45;                       // 欠压保护阈值 (V)
float OVP = 55;                       // 过压保护阈值 (V)
uint16_t VolumeHex = 0x8FF6;         // 默认音量设置
```

## 故障处理

系统包含完整的故障检测和自动恢复机制：

1. **实时监控**: 每100ms检查一次状态寄存器
2. **故障检测**: 自动检测系统故障位 (寄存器62, bit 5)
3. **自动恢复**: 检测到故障时自动重启控制环路
4. **故障清除**: 自动清除故障锁存和状态寄存器

## 开发注意事项

### I2C通信
- AX5689使用7位地址模式
- 寄存器地址为16位，数据为16位
- 写入格式: [RegAddr_MSB, RegAddr_LSB, Data_MSB, Data_LSB]

### GPIO控制
- `AX_Reset_N`: 开漏输出，控制芯片复位
- `AX_Mute_N`: 推挽输出，控制静音
- 所有电源控制引脚默认为低电平

### 音量控制
- 使用线性斜坡控制
- 支持快速静音和渐变调节
- 可配置斜坡时间和步长

## 版本历史

- **v1.0** (OpenLoop): 基础开环控制实现
- **v2.0** (CloseLoop): 添加闭环控制和故障处理

## 许可证

本项目遵循MIT许可证。

## 联系信息

- **作者**: Nicole YU @MPS Axign Design Center. AE
- **项目**: AX5689_STM32
- **GitHub**: https://github.com/yuwenluopie/AX5689_STM32

## 致谢

- Axign BV 提供的AX5689寄存器配置参数
- STMicroelectronics HAL库支持

---

**注意**: 寄存器配置文件中的参数由Axign BV提供，仅作为参考建议。具体应用中的寄存器值设置由用户负责确定。
