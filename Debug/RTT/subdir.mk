################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../RTT/SEGGER_RTT.c \
../RTT/SEGGER_RTT_printf.c 

S_UPPER_SRCS += \
../RTT/SEGGER_RTT_ASM_ARMv7M.S 

OBJS += \
./RTT/SEGGER_RTT.o \
./RTT/SEGGER_RTT_ASM_ARMv7M.o \
./RTT/SEGGER_RTT_printf.o 

S_UPPER_DEPS += \
./RTT/SEGGER_RTT_ASM_ARMv7M.d 

C_DEPS += \
./RTT/SEGGER_RTT.d \
./RTT/SEGGER_RTT_printf.d 


# Each subdirectory must supply rules for building sources it contributes
RTT/%.o RTT/%.su RTT/%.cyclo: ../RTT/%.c RTT/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0plus -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G0B0xx -c -I../Core/Inc -I../Drivers/STM32G0xx_HAL_Driver/Inc -I../Drivers/STM32G0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G0xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
RTT/%.o: ../RTT/%.S RTT/subdir.mk
	arm-none-eabi-gcc -mcpu=cortex-m0plus -g3 -DDEBUG -c -I"C:/Users/ncyu/Downloads/AX5689_STM32-8cb31875031f0ff0190111b8f8f88773739e11c2/RTT" -I"C:/Users/ncyu/Downloads/AX5689_STM32-8cb31875031f0ff0190111b8f8f88773739e11c2/Core/Inc" -x assembler-with-cpp -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@" "$<"

clean: clean-RTT

clean-RTT:
	-$(RM) ./RTT/SEGGER_RTT.cyclo ./RTT/SEGGER_RTT.d ./RTT/SEGGER_RTT.o ./RTT/SEGGER_RTT.su ./RTT/SEGGER_RTT_ASM_ARMv7M.d ./RTT/SEGGER_RTT_ASM_ARMv7M.o ./RTT/SEGGER_RTT_printf.cyclo ./RTT/SEGGER_RTT_printf.d ./RTT/SEGGER_RTT_printf.o ./RTT/SEGGER_RTT_printf.su

.PHONY: clean-RTT

