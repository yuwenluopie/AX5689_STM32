# AX5689 Audio Amplifier Control System

Complete STM32G0 solution for controlling Axign AX5689 audio amplifier with dual-mode switching and fault protection.

## Getting Started

### What You Need

**Hardware:**
- STM32G0B0RET6 development board
- AX5689 audio amplifier module
- Jumper wires for I2C connection
- ST-Link debugger

**Software:**
- STM32CubeIDE (free download from STMicroelectronics)
- This project code

## Quick Setup (5 Minutes)

### Step 1: Hardware Connections

Connect these 5 essential wires between STM32 and AX5689:

| Function | STM32 Pin | AX5689 Pin | Wire Color |
|----------|-----------|------------|------------|
| I2C Clock | PB13 | SCL | Yellow |
| I2C Data | PB14 | SDA | Green |
| Reset Control | PA0 | RESET_N | Red |
| Status Monitor | PA1 | STATUS | Blue |
| Mute Control | PC0 | MUTE_N | White |

**Important**: Make sure both boards share the same ground (GND)!

### Step 2: Load the Code

1. **Download STM32CubeIDE** from ST's website (it's free)
2. **Clone this project**:
   ```
   git clone https://github.com/yuwenluopie/AX5689_STM32.git
   ```
3. **Open project** in STM32CubeIDE:
   - File → Import → Existing Projects into Workspace
   - Browse to the downloaded folder

### Step 3: Build and Flash

1. **Connect ST-Link** to your STM32 board
2. **Build the project**: Press `Ctrl+B`
3. **Flash to board**: Click the green "Run" button
4. **Done!** The amplifier should now be working

## How It Works

### Two Operating Modes

**OpenLoop Mode (Default)**
- Simple amplification
- Lower power consumption
- Good for most applications
- LED OFF = OpenLoop active

**CloseLoop Mode (Advanced)**
- Better audio quality with feedback control
- Higher precision
- LED ON = CloseLoop active

### Mode Switching

The system automatically detects which mode to use, or you can switch manually in code:

```c
// Switch to OpenLoop mode
AX5689_SwitchToOpenLoop();

// Switch to CloseLoop mode  
AX5689_SwitchToCloseLoop();
```

## Basic Configuration

### I2C Settings (Already configured)
```c
// The code uses these settings - no need to change
I2C Address: 0x72 (8-bit format)
Speed: 100kHz (standard mode)
```

### Startup Sequence (Automatic)
The system automatically:
1. Initializes power supplies
2. Resets the AX5689 chip
3. Loads configuration registers
4. Starts audio processing
5. Unmutes the amplifier

## Troubleshooting

### No Sound Output
- ✅ Check all 5 wire connections
- ✅ Verify power supply to both boards
- ✅ Ensure audio input is connected to AX5689
- ✅ Check if MUTE_N pin is HIGH (unmuted)

### I2C Communication Errors
- ✅ Double-check SDA and SCL connections
- ✅ Verify ground connection between boards
- ✅ Make sure both boards are powered on

### Frequent Faults
- ✅ Check power supply stability (use adequate current rating)
- ✅ Verify speaker impedance matches amplifier specs
- ✅ Ensure proper heat dissipation

## Debug Information

The system provides real-time debug output through SEGGER RTT. To view:

1. **Install J-Link Software** (free from SEGGER)
2. **Run RTT Viewer** while your program is running
3. **See live status messages**:
   ```
   === AX5689 Initialization ===
   I2C communication: OK
   Register configuration: Complete
   Control loop: Started
   Current mode: OpenLoop
   Status: Ready
   ```

## Sample Code

Here's the complete main function (already included in the project):

```c
int main(void)
{
    // System initialization
    HAL_Init();
    SystemClock_Config();
    
    // Initialize peripherals
    MX_GPIO_Init();
    MX_I2C2_Init();
    MX_USART1_UART_Init();
    
    // Initialize AX5689 amplifier
    IO_Init();              // Setup GPIO pins
    AX5689_Setup();         // Configure amplifier registers
    StartControlLoop();     // Start audio processing
    
    while(1) {
        // Monitor for faults every 100ms
        checkStatusRegister();
        HAL_Delay(100);
        
        // Your application code here...
    }
}
```

## Key Functions You Can Use

```c
// Basic control
void AX5689_SwitchToOpenLoop(void);    // Switch to simple mode
void AX5689_SwitchToCloseLoop(void);   // Switch to advanced mode
void StartControlLoop(void);           // Start amplifier
void StopControlLoop(void);            // Stop amplifier safely

// Communication
HAL_StatusTypeDef AX5689_Read(uint16_t address, uint16_t *data);
HAL_StatusTypeDef AX5689_Write(uint16_t address, uint16_t value);

// Status monitoring
void checkStatusRegister(void);        // Check for faults
```

## Important Notes

- **Power Sequence**: Always power STM32 first, then AX5689
- **Safety**: The system includes automatic fault protection
- **Audio Quality**: Use CloseLoop mode for best audio performance
- **Debugging**: Enable RTT output for detailed system information

## Support

- **Issues**: Report problems on GitHub Issues
- **Documentation**: Check the code comments for details
- **Updates**: Pull latest changes for bug fixes and improvements

## Quick Reference

| Register | Function | OpenLoop Value | CloseLoop Value |
|----------|----------|----------------|-----------------|
| 0x0001 | Main Enable | 0xFFFF | 0xFFFF |
| 0x0003 | Mode Control | 0x5500 | 0x0055 |
| 0x0007 | Volume Ramp | 0x0001 | 0x0001 |

---

**That's it!** Your AX5689 amplifier should now be working. The system handles all the complex initialization and fault protection automatically.
