################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/main.c \
../src/system_stm32f4xx.c 

OBJS += \
./src/main.o \
./src/system_stm32f4xx.o 

C_DEPS += \
./src/main.d \
./src/system_stm32f4xx.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DSTM32 -DSTM32F4 -DSTM32F407VGTx -DSTM32F407G_DISC1 -DDEBUG -DSTM32F40XX -DSTM32F40_41xxx -DUSE_STDPERIPH_DRIVER -I"C:/Users/Utilisateur/workspace/HelloWorld_STM32_Dainguyen/StdPeriph_Driver/inc" -I"C:/Users/Utilisateur/workspace/HelloWorld_STM32_Dainguyen/Config" -I"C:/Users/Utilisateur/workspace/HelloWorld_STM32_Dainguyen/ThirdParty/FreeRTOS/org/Source/portable/GCC/ARM_CM4F" -I"C:/Users/Utilisateur/workspace/HelloWorld_STM32_Dainguyen/ThirdParty/FreeRTOS/org/Source/include" -I"C:/Users/Utilisateur/workspace/HelloWorld_STM32_Dainguyen/inc" -I"C:/Users/Utilisateur/workspace/HelloWorld_STM32_Dainguyen/CMSIS/device" -I"C:/Users/Utilisateur/workspace/HelloWorld_STM32_Dainguyen/CMSIS/core" -O0 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


