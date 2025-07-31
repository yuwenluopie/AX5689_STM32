# AX5689 功率级故障处理实现

## 根据官方Axign文档实现的完整故障处理系统

### 🔧 实现功能

#### 1. **故障检测机制**
- **AX_STATUS引脚监控**: 通过`AX_State_Pin` (PA1)监控故障状态
- **状态寄存器读取**: 读取寄存器60、61、62获取详细故障信息
- **sys_fault位检查**: 检查寄存器62的第5位确认系统故障

#### 2. **故障处理流程**
按照官方文档的精确步骤：
```c
if (digitalRead(AX_STATUS) == HIGH) // 检查状态引脚是否为高电平
{
    AX568X_Read(axAddrI2C, 60); // 读出寄存器 60
    uint16_t reg61val = AX568X_Read(axAddrI2C, 61); // 读出寄存器 61
    uint16_t reg62val = AX568X_Read(axAddrI2C, 62); // 读出寄存器 62
    if ((reg62val & 32) > 0) // sys_fault 位被激活
    {
        StopControlLoop(axAddrI2C); // 使 AX5688/AX5689 准备好重启
        // 复位 PFAULT_N 掩码以清除 PSTART 锁存
        AX568X_Write(axAddrI2C, 56, REG56_RESET_VALUE); 
        // 将 PFAULT_N 掩码设置回粘滞模式
        AX568X_Write(axAddrI2C, 56, REG56_VALUE); 
        clearStatusRegisters(axAddrI2C); // 复位状态寄存器
        delay(100); // 故障后等待功率级稳定
        if (digitalRead(AX_STATUS) == LOW) // sys_fault 位未激活
        {
            StartControlLoop(axAddrI2C); // 启动控制环路以恢复播放
        }
    }
}
```

### 📋 关键寄存器配置

| 寄存器 | 地址 | 功能 | 值 |
|--------|------|------|-----|
| **寄存器56** | 0x0038 | PFAULT_N掩码控制 | REG56_VALUE = 0x0C35 |
| **寄存器56** | 0x0038 | PFAULT_N复位值 | REG56_RESET_VALUE = 0x0435 |
| **寄存器60** | 0x003C | 状态寄存器1 | 读取清除 |
| **寄存器61** | 0x003D | 状态寄存器2 | 读取清除 |
| **寄存器62** | 0x003E | 状态寄存器3 | bit5=sys_fault |

### 🎯 硬件连接

| 功能 | 引脚 | STM32引脚 | 说明 |
|------|------|-----------|------|
| **AX_STATUS** | AX_State_Pin | PA1 | 故障状态指示 |
| **LED指示** | LED_ON_OFF_Pin | PC8 | 故障状态显示 |

### 🔄 故障处理步骤

#### 步骤1: 故障检测
```c
// 检查AX_STATUS引脚状态
if (HAL_GPIO_ReadPin(AX_State_GPIO_Port, AX_State_Pin) == GPIO_PIN_SET)
```

#### 步骤2: 读取状态寄存器
```c
AX5689_Read(0x003C, &reg60val); // 寄存器60
AX5689_Read(0x003D, &reg61val); // 寄存器61  
AX5689_Read(0x003E, &reg62val); // 寄存器62
```

#### 步骤3: 检查sys_fault位
```c
if ((reg62val & (1 << 5)) > 0) // 检查第5位
```

#### 步骤4: 停止控制循环
```c
StopControlLoop(); // 安全停止
```

#### 步骤5: 复位PFAULT_N掩码
```c
AX5689_Write(0x0038, REG56_RESET_VALUE); // 清除锁存
AX5689_Write(0x0038, REG56_VALUE);       // 恢复粘滞模式
```

#### 步骤6: 清除状态寄存器
```c
clearStatusRegisters(); // 复位所有状态
```

#### 步骤7: 等待稳定
```c
HAL_Delay(100); // 100ms稳定时间
```

#### 步骤8: 验证故障清除
```c
if (HAL_GPIO_ReadPin(AX_State_GPIO_Port, AX_State_Pin) == GPIO_PIN_RESET) {
    StartControlLoop(); // 重启成功
} else {
    // 故障持续，系统保护
}
```

### 📞 函数接口

#### 主要函数
```c
// 故障检查函数（立即检查）
void checkStatusRegister(void);

// 定期故障监控函数（从主循环调用）
void AX5689_PeriodicFaultCheck(void);
```

#### 使用方法
```c
// 在main()函数的主循环中添加：
while (1) {
    AX5689_PeriodicFaultCheck(); // 每100ms检查一次故障
    // 其他应用代码...
}
```

### 🚨 故障类型

#### 支持的故障检测
1. **过流保护 (OCP)** - 输出电流过大
2. **过压保护 (OVP)** - 电源电压过高  
3. **欠压保护 (UVP)** - 电源电压过低
4. **过温保护 (OTP)** - 芯片温度过高
5. **功率级故障** - PFAULT_N信号触发

#### 故障响应
- **即时保护**: 检测到故障立即停止输出
- **自动恢复**: 故障清除后自动重启
- **安全锁定**: 持续故障时保持保护状态
- **视觉指示**: LED显示故障状态

### 💡 最佳实践

1. **定期监控**: 在主循环中调用`AX5689_PeriodicFaultCheck()`
2. **快速响应**: 100ms检查间隔确保及时故障处理
3. **安全优先**: 故障持续时保持保护状态
4. **调试支持**: RTT输出详细故障信息
5. **文档对照**: 严格按照官方Axign文档实现

### 🔍 调试输出示例

```
⚠ AX_STATUS pin HIGH - Fault detected!
Status Register 60: 0x0000
Status Register 61: 0x0000  
Status Register 62: 0x0020
🔴 System fault (bit 5) detected in register 62!
Stopping control loop...
Resetting PFAULT_N mask to clear PSTART latch...
Setting PFAULT_N mask back to sticky mode...
Status registers cleared.
Waiting for power stage stabilization (100ms)...
✅ Fault cleared. Restarting control loop...
```

## 📝 总结

现在的故障处理系统完全符合官方Axign文档要求，提供了：
- ✅ 实时故障监控
- ✅ 自动故障恢复  
- ✅ 安全保护机制
- ✅ 详细调试信息
- ✅ 标准化接口

这确保了AX5689在各种故障条件下都能安全可靠地工作。
