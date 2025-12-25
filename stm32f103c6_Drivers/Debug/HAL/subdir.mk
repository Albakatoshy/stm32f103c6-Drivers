################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../HAL/Keypad.c \
../HAL/lcd.c 

OBJS += \
./HAL/Keypad.o \
./HAL/lcd.o 

C_DEPS += \
./HAL/Keypad.d \
./HAL/lcd.d 


# Each subdirectory must supply rules for building sources it contributes
HAL/Keypad.o: ../HAL/Keypad.c
	arm-none-eabi-gcc -gdwarf-2 "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DSTM32 -DSTM32F1 -DSTM32F103C6Tx -DDEBUG -c -I"D:/Embedded sys Course/ARM/Diploma/stm32f103c6_Drivers/HAL/includes" -I../Inc -I"D:/Embedded sys Course/ARM/Diploma/stm32f103c6_Drivers/Stm32f103c6_Drivers/include" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"HAL/Keypad.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
HAL/lcd.o: ../HAL/lcd.c
	arm-none-eabi-gcc -gdwarf-2 "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DSTM32 -DSTM32F1 -DSTM32F103C6Tx -DDEBUG -c -I"D:/Embedded sys Course/ARM/Diploma/stm32f103c6_Drivers/HAL/includes" -I../Inc -I"D:/Embedded sys Course/ARM/Diploma/stm32f103c6_Drivers/Stm32f103c6_Drivers/include" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"HAL/lcd.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

