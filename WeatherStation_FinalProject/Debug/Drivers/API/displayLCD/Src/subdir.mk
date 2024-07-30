################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/API/displayLCD/Src/API_display.c \
../Drivers/API/displayLCD/Src/API_display_HAL.c 

OBJS += \
./Drivers/API/displayLCD/Src/API_display.o \
./Drivers/API/displayLCD/Src/API_display_HAL.o 

C_DEPS += \
./Drivers/API/displayLCD/Src/API_display.d \
./Drivers/API/displayLCD/Src/API_display_HAL.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/API/displayLCD/Src/%.o Drivers/API/displayLCD/Src/%.su Drivers/API/displayLCD/Src/%.cyclo: ../Drivers/API/displayLCD/Src/%.c Drivers/API/displayLCD/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F429xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"/home/ubuntu/Desktop/MSc in Embedded Systems/2. Programación de Microcontroladores (PdM)/PdM_VAZQUEZ_workspace/WeatherStation_FinalProject/Drivers/API/displayLCD/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-API-2f-displayLCD-2f-Src

clean-Drivers-2f-API-2f-displayLCD-2f-Src:
	-$(RM) ./Drivers/API/displayLCD/Src/API_display.cyclo ./Drivers/API/displayLCD/Src/API_display.d ./Drivers/API/displayLCD/Src/API_display.o ./Drivers/API/displayLCD/Src/API_display.su ./Drivers/API/displayLCD/Src/API_display_HAL.cyclo ./Drivers/API/displayLCD/Src/API_display_HAL.d ./Drivers/API/displayLCD/Src/API_display_HAL.o ./Drivers/API/displayLCD/Src/API_display_HAL.su

.PHONY: clean-Drivers-2f-API-2f-displayLCD-2f-Src

