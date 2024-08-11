################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Application/FSM/Src/Application_FSM.c 

OBJS += \
./Core/Application/FSM/Src/Application_FSM.o 

C_DEPS += \
./Core/Application/FSM/Src/Application_FSM.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Application/FSM/Src/%.o Core/Application/FSM/Src/%.su Core/Application/FSM/Src/%.cyclo: ../Core/Application/FSM/Src/%.c Core/Application/FSM/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F429xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"/home/ubuntu/Desktop/MSc in Embedded Systems/2. Programación de Microcontroladores (PdM)/PdM_VAZQUEZ_workspace/WeatherStation_FinalProject/Drivers/API/displayLCD/Inc" -I"/home/ubuntu/Desktop/MSc in Embedded Systems/2. Programación de Microcontroladores (PdM)/PdM_VAZQUEZ_workspace/WeatherStation_FinalProject/Drivers/API/BME680/Inc" -I"/home/ubuntu/Desktop/MSc in Embedded Systems/2. Programación de Microcontroladores (PdM)/PdM_VAZQUEZ_workspace/WeatherStation_FinalProject/Drivers/API/Relay/Inc" -I"/home/ubuntu/Desktop/MSc in Embedded Systems/2. Programación de Microcontroladores (PdM)/PdM_VAZQUEZ_workspace/WeatherStation_FinalProject/Core/Application/FSM/Inc" -I"/home/ubuntu/Desktop/MSc in Embedded Systems/2. Programación de Microcontroladores (PdM)/PdM_VAZQUEZ_workspace/WeatherStation_FinalProject/Drivers/API/Delay/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Application-2f-FSM-2f-Src

clean-Core-2f-Application-2f-FSM-2f-Src:
	-$(RM) ./Core/Application/FSM/Src/Application_FSM.cyclo ./Core/Application/FSM/Src/Application_FSM.d ./Core/Application/FSM/Src/Application_FSM.o ./Core/Application/FSM/Src/Application_FSM.su

.PHONY: clean-Core-2f-Application-2f-FSM-2f-Src

