################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Stm32f103c6_Drivers/Lcd_Driver/ecu_chr_lcd.c \
../Stm32f103c6_Drivers/Lcd_Driver/lcd.c 

OBJS += \
./Stm32f103c6_Drivers/Lcd_Driver/ecu_chr_lcd.o \
./Stm32f103c6_Drivers/Lcd_Driver/lcd.o 

C_DEPS += \
./Stm32f103c6_Drivers/Lcd_Driver/ecu_chr_lcd.d \
./Stm32f103c6_Drivers/Lcd_Driver/lcd.d 


# Each subdirectory must supply rules for building sources it contributes
Stm32f103c6_Drivers/Lcd_Driver/ecu_chr_lcd.o: ../Stm32f103c6_Drivers/Lcd_Driver/ecu_chr_lcd.c
	arm-none-eabi-gcc -gdwarf-2 "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DSTM32 -DSTM32F1 -DSTM32F103C6Tx -DDEBUG -c -I"D:/Embedded sys Course/ARM/Diploma/stm32f103c6_Drivers/HAL/includes" -I../Inc -I"D:/Embedded sys Course/ARM/Diploma/stm32f103c6_Drivers/Stm32f103c6_Drivers/include" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Stm32f103c6_Drivers/Lcd_Driver/ecu_chr_lcd.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
Stm32f103c6_Drivers/Lcd_Driver/lcd.o: ../Stm32f103c6_Drivers/Lcd_Driver/lcd.c
	arm-none-eabi-gcc -gdwarf-2 "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DSTM32 -DSTM32F1 -DSTM32F103C6Tx -DDEBUG -c -I"D:/Embedded sys Course/ARM/Diploma/stm32f103c6_Drivers/HAL/includes" -I../Inc -I"D:/Embedded sys Course/ARM/Diploma/stm32f103c6_Drivers/Stm32f103c6_Drivers/include" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Stm32f103c6_Drivers/Lcd_Driver/lcd.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

