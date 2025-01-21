################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/devices/HMC5883L.c \
../Core/Src/devices/JDY18.c \
../Core/Src/devices/L293D.c 

OBJS += \
./Core/Src/devices/HMC5883L.o \
./Core/Src/devices/JDY18.o \
./Core/Src/devices/L293D.o 

C_DEPS += \
./Core/Src/devices/HMC5883L.d \
./Core/Src/devices/JDY18.d \
./Core/Src/devices/L293D.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/devices/%.o Core/Src/devices/%.su Core/Src/devices/%.cyclo: ../Core/Src/devices/%.c Core/Src/devices/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F446xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-devices

clean-Core-2f-Src-2f-devices:
	-$(RM) ./Core/Src/devices/HMC5883L.cyclo ./Core/Src/devices/HMC5883L.d ./Core/Src/devices/HMC5883L.o ./Core/Src/devices/HMC5883L.su ./Core/Src/devices/JDY18.cyclo ./Core/Src/devices/JDY18.d ./Core/Src/devices/JDY18.o ./Core/Src/devices/JDY18.su ./Core/Src/devices/L293D.cyclo ./Core/Src/devices/L293D.d ./Core/Src/devices/L293D.o ./Core/Src/devices/L293D.su

.PHONY: clean-Core-2f-Src-2f-devices

