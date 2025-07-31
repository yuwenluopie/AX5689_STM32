# AX5689 Audio Amplifier Control System

A comprehensive STM32-based control system for the Axign AX5689 audio amplifier, featuring dual-mode operation, advanced fault handling, and anti-pop noise protection.

## 🎯 Project Overview

This project implements a professional audio amplifier control system using the STM32G0B0RET6 microcontroller and Axign AX5689 Class-D audio amplifier. The system supports both OpenLoop and CloseLoop operational modes with seamless switching capabilities, comprehensive fault protection, and adherence to official Axign documentation for optimal audio performance.

### Key Features

- **Dual Operating Modes**: OpenLoop and CloseLoop with dynamic switching
- **I2C Communication**: Full register access and configuration control  
- **Advanced Fault Handling**: Real-time monitoring and automatic recovery
- **Anti-Pop Noise**: Official Axign startup/shutdown sequences
- **Power Management**: Complete power sequencing and control
- **Debug Support**: SEGGER RTT integration for real-time debugging
- **Hardware Abstraction**: Clean API for amplifier control

## 📋 Hardware Specifications

### Microcontroller
- **MCU**: STM32G0B0RET6 (Cortex-M0+, 64-pin LQFP)
- **Flash**: 512KB
- **RAM**: 144KB
- **Clock**: 64MHz maximum

### Audio Amplifier Configuration
- **Chip**: Axign AX5689 Class-D Audio Amplifier
- **Reference Design**: 4x MP1916, 8x GaNFET
- **Audio Format**: I2S, 2 channels, 192 kHz sample rate
- **Output**: 2x BTL (Bridge-Tied Load)
- **Power Supply**: PVDD = 50V
- **I2C Address**: 0x39 (7-bit) / 0x72 (8-bit)

### Output Filter Configuration
- **OpenLoop Mode**: L = 4.7µH, C = 2.2µF
- **CloseLoop Mode**: 4x L = 4.7µH, 4x C = 1.0µF, 2x C = 0.47µF
- **ADC Input Resistors**: 10kΩ (0.1% tolerance)

## 🔌 Pin Configuration

### Power Control
| Function | STM32 Pin | Description |
|----------|-----------|-------------|
| D1V2_EN | PC11 | 1.2V Digital Supply Enable |
| AX_3V3 | PC12 | 3.3V Analog Supply Enable |
| AX_5V | PC13 | 5V Supply Enable |
| D5V2_EN | PC14 | 5.2V Digital Supply Enable |
| PVDD_EN | PA2 | Power Stage Supply Enable |

### Control Signals
| Function | STM32 Pin | Description |
|----------|-----------|-------------|
| AX_Reset_N | PA0 | Amplifier Reset (Active Low) |
| AX_State | PA1 | Fault Status Input |
| AX_Mute_N | PC0 | Amplifier Mute Control (Active Low) |
| MCU_Pstart | PA8 | Power Stage Start Control |

### Communication Interfaces
| Function | STM32 Pin | Description |
|----------|-----------|-------------|
| I2C2_SCL | PB13 | I2C Clock Line |
| I2C2_SDA | PB14 | I2C Data Line |
| SPI2_CLK | PA5 | SPI Clock (LED Display) |
| SPI2_MOSI | PA7 | SPI Data (LED Display) |
| USART1_TX | PA9 | Debug UART TX |
| USART1_RX | PA10 | Debug UART RX |

### User Interface
| Function | STM32 Pin | Description |
|----------|-----------|-------------|
| LED_ON_OFF | PC8 | Status LED |
| Mode_Select | PC9 | Mode Selection Switch |
| LED_Display_CS | PA4 | LED Display Chip Select |

## 🏗️ Software Architecture

### Project Structure
```
MyGPIOtest/
├── Core/
│   ├── Inc/
│   │   ├── main.h                 # Main header with pin definitions
│   │   ├── ax5689_control.h       # AX5689 control library header
│   │   ├── OpenLoop.h             # OpenLoop configuration
│   │   └── CloseLoop.h            # CloseLoop configuration
│   └── Src/
│       ├── main.c                 # Main application
│       ├── ax5689_control.c       # AX5689 control implementation
│       ├── stm32g0xx_hal_msp.c    # HAL MSP configuration
│       └── stm32g0xx_it.c         # Interrupt handlers
├── Drivers/                       # STM32 HAL drivers
├── RTT/                          # SEGGER RTT debug support
└── Documentation/
    ├── FAULT_HANDLING_IMPLEMENTATION.md
    └── OFFICIAL_INIT_SEQUENCE.md
```

### Key Components

#### 1. AX5689 Control Library (`ax5689_control.c/.h`)
- **I2C Communication**: Register read/write functions
- **Mode Management**: OpenLoop/CloseLoop switching
- **Power Sequencing**: Official Axign initialization
- **Fault Handling**: Real-time monitoring and recovery
- **Control Sequences**: Anti-pop startup/shutdown

#### 2. Configuration Headers
- **OpenLoop.h**: Register array for basic amplification
- **CloseLoop.h**: Register array for enhanced feedback control

#### 3. Main Application (`main.c`)
- **System Initialization**: Clock, GPIO, I2C, SPI, UART setup
- **Mode Selection**: Switch-based mode changing
- **Main Loop**: Periodic fault monitoring and status updates

## 🔧 API Reference

### Core Functions

#### Initialization
```c
void IO_Init(void);                    // Initialize GPIO and power sequence
void AX5689_Setup(void);               // Configure AX5689 with current mode
```

#### Control Loop Management
```c
void StartControlLoop(void);           // Start amplifier with anti-pop sequence
void StopControlLoop(void);            // Stop amplifier with anti-pop sequence
```

#### Mode Switching
```c
void AX5689_SwitchToOpenLoop(void);    // Switch to OpenLoop mode
void AX5689_SwitchToCloseLoop(void);   // Switch to CloseLoop mode
void AX5689_SetConfigMode(AX5689_ConfigMode_t mode);
AX5689_ConfigMode_t AX5689_GetConfigMode(void);
```

#### Communication
```c
HAL_StatusTypeDef AX5689_Read(uint16_t regAddr, uint16_t *data);
HAL_StatusTypeDef AX5689_Write(uint16_t regAddr, uint16_t regVal);
void AX5689_WriteRegisterSet(const uint16_t *ptrConfigArray);
```

#### Fault Handling
```c
void checkStatusRegister(void);        // Immediate fault check
void AX5689_PeriodicFaultCheck(void);  // Periodic monitoring (call from main loop)
void clearStatusRegisters(void);       // Clear status registers
```

## ⚡ Operational Modes

### OpenLoop Mode
- **Purpose**: Basic amplification without feedback
- **Register 0x0003**: 0x5500
- **Use Case**: High-efficiency operation
- **LED Status**: OFF (PC8 = LOW)

### CloseLoop Mode  
- **Purpose**: Enhanced control with ADC feedback
- **Register 0x0003**: 0x1155
- **Use Case**: Improved linearity and control
- **LED Status**: ON (PC8 = HIGH)

### Mode Switching Process
1. **Stop Control Loop**: Safe shutdown with anti-pop sequence
2. **Set New Mode**: Update configuration mode variable
3. **Reconfigure Registers**: Write new register set
4. **Start Control Loop**: Restart with new configuration
5. **Verify Mode**: Read register 0x0003 to confirm

## 🛡️ Fault Protection System

### Fault Detection
- **AX_STATUS Pin Monitoring**: Real-time fault status (PA1)
- **Status Register Reading**: Registers 60, 61, 62 (0x003C-0x003E)
- **sys_fault Bit Check**: Register 62, bit 5
- **Monitoring Frequency**: Every 100ms

### Fault Types Supported
- **Overcurrent Protection (OCP)**
- **Overvoltage Protection (OVP)**  
- **Undervoltage Protection (UVP)**
- **Overtemperature Protection (OTP)**
- **Power Stage Faults**

### Fault Recovery Sequence
1. **Fault Detection**: AX_STATUS pin goes HIGH
2. **Status Analysis**: Read all status registers
3. **Safe Shutdown**: Execute StopControlLoop()
4. **PFAULT_N Reset**: Clear PSTART latch (Register 56)
5. **Status Clear**: Reset all status registers
6. **Stabilization**: 100ms power stage recovery
7. **Verification**: Check fault clearance
8. **Recovery**: Restart control loop or maintain safe state

## 🚀 Getting Started

### Prerequisites
- **STM32CubeIDE**: Version 1.18.1 or later
- **STM32G0 HAL Library**: Latest version
- **SEGGER RTT**: For debug output
- **Hardware**: STM32G0B0RET6 development board + AX5689 amplifier board

### Build Instructions
1. **Clone Repository**:
   ```bash
   git clone <repository-url>
   cd MyGPIOtest
   ```

2. **Open in STM32CubeIDE**:
   - Import existing project
   - Select workspace folder

3. **Build Project**:
   - Right-click project → Build Project
   - Or use Ctrl+B

4. **Flash and Debug**:
   - Connect ST-Link debugger
   - Use Debug configuration

### Quick Start Code
```c
int main(void) {
    // System initialization
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_I2C2_Init();
    
    // Initialize AX5689 system
    IO_Init();              // GPIO and power sequence
    AX5689_Setup();         // Configure amplifier
    StartControlLoop();     // Start audio processing
    
    while (1) {
        // Periodic fault monitoring
        AX5689_PeriodicFaultCheck();
        
        // Mode switching based on user input
        if (HAL_GPIO_ReadPin(Mode_Select_GPIO_Port, Mode_Select_Pin)) {
            AX5689_SwitchToCloseLoop();
        } else {
            AX5689_SwitchToOpenLoop();
        }
        
        HAL_Delay(10);
    }
}
```

## 🔍 Debug and Monitoring

### SEGGER RTT Output
The system provides comprehensive debug information via SEGGER RTT:

```
=== Step 2: Initialize I/O ===
AX_MUTE_N set to LOW (amplifier muted)
Power stage set to Hi-Z state
Signal stabilization delay: 20ms
AX_RESET_N set to LOW (Axign controller disabled)

=== Step 4: Set up AX5688/AX5689 ===
AX_RESET_N set to HIGH (Enable AX5689)
Writing OpenLoop register configuration...
Writing reg 0x0012 = 0x0371
...
System stabilization delay: 500ms

Starting control loop in OpenLoop mode...
Writing reg 0x0001 = 0xFF00
...
Control loop started successfully.
```

### Status Monitoring
- **Register Writes**: Real-time I2C transaction logging
- **Mode Changes**: Detailed switching sequence tracking
- **Fault Events**: Complete fault analysis and recovery logging
- **Performance Metrics**: Timing and status information

## 📊 Performance Characteristics

### Timing Specifications
- **I2C Communication**: 400kHz Fast Mode
- **Startup Time**: ~500ms (including stabilization)
- **Mode Switch Time**: ~1s (stop → configure → start)
- **Fault Response**: <100ms detection and response
- **Anti-pop Sequence**: 20ms ramp time

### Power Consumption
- **Idle Current**: <10mA (STM32 + control logic)
- **Active Current**: Variable based on audio signal
- **Standby Mode**: Power stage disabled, control active

## 🛠️ Configuration Options

### Compile-Time Configuration
```c
// I2C Address (ax5689_control.h)
#define AX5689_I2C_ADDR (0x39 << 1)

// Fault handling register values (OpenLoop.h)
#define REG56_VALUE 0x0C35
#define REG56_RESET_VALUE 0x0435

// Debug output control
#define RTT_printf(fmt, ...) SEGGER_RTT_printf(0, fmt, ##__VA_ARGS__)
```

### Runtime Configuration
- **Mode Selection**: GPIO-based or API-based switching
- **Fault Monitoring**: Configurable check intervals
- **Volume Control**: Register-based or GPIO mute control

## 🧪 Testing and Validation

### Functional Tests
- **I2C Communication**: Register read/write verification
- **Mode Switching**: Seamless transition testing
- **Fault Injection**: Controlled fault scenario testing
- **Audio Quality**: THD+N and frequency response measurement

### Stress Tests
- **Continuous Operation**: 24+ hour stability testing
- **Thermal Cycling**: Temperature variation testing
- **Fault Recovery**: Repeated fault/recovery cycles

## 🔒 Safety Features

### Hardware Protection
- **Overcurrent Protection**: Automatic current limiting
- **Thermal Protection**: Temperature monitoring and shutdown
- **Voltage Monitoring**: Supply voltage range checking
- **Isolation**: Proper ground and power isolation

### Software Protection
- **Watchdog Timer**: System health monitoring
- **Exception Handling**: Graceful error recovery
- **State Machine**: Controlled operational states
- **Input Validation**: Parameter range checking

## 📈 Future Enhancements

### Planned Features
- **DSP Integration**: Advanced audio processing
- **Network Connectivity**: Remote monitoring and control
- **Advanced Diagnostics**: Extended fault analysis
- **User Interface**: LCD display and rotary encoder
- **Firmware Updates**: Over-the-air update capability

### Expansion Possibilities
- **Multi-Channel Support**: Support for additional amplifiers
- **Audio Effects**: Built-in EQ and effects processing
- **Smart Features**: Automatic gain control and room correction

## 📚 Documentation References

- **Axign AX5689 Datasheet**: Official hardware documentation
- **STM32G0B0 Reference Manual**: Microcontroller specifications
- **Application Notes**: Audio amplifier design guidelines
- **FAULT_HANDLING_IMPLEMENTATION.md**: Detailed fault handling documentation
- **OFFICIAL_INIT_SEQUENCE.md**: Initialization sequence documentation

## 🤝 Contributing

Contributions are welcome! Please follow these guidelines:
1. Fork the repository
2. Create a feature branch
3. Follow coding standards (see style guide)
4. Add comprehensive tests
5. Update documentation
6. Submit pull request

## 📄 License

This project is licensed under the MIT License - see the LICENSE file for details.

## 📞 Support

For technical support and questions:
- **Issue Tracker**: GitHub Issues
- **E-mail**: nicole.yu@monolithicpower.com
- **Developer**: ncyu (Project Author)

## 👥 Contributors

- **Nicole YU** - Project Author (@MPS Axign Design Center. AE)
- **GitHub Repository**: https://github.com/yuwenluopie/AX5689_STM32

## 🙏 Acknowledgments

- **Axign BV** - For providing AX5689 register configuration parameters and official documentation
- **STMicroelectronics** - For STM32 HAL library support and development tools
- **SEGGER** - For Real-Time Transfer (RTT) debugging technology

---

**Note**: This project implements professional audio amplifier control following official Axign documentation and industry best practices. Always refer to the official AX5689 datasheet for the most current specifications and requirements.
