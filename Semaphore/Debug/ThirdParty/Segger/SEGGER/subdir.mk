################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../ThirdParty/Segger/SEGGER/SEGGER_RTT.c \
../ThirdParty/Segger/SEGGER/SEGGER_SYSVIEW.c 

OBJS += \
./ThirdParty/Segger/SEGGER/SEGGER_RTT.o \
./ThirdParty/Segger/SEGGER/SEGGER_SYSVIEW.o 

C_DEPS += \
./ThirdParty/Segger/SEGGER/SEGGER_RTT.d \
./ThirdParty/Segger/SEGGER/SEGGER_SYSVIEW.d 


# Each subdirectory must supply rules for building sources it contributes
ThirdParty/Segger/SEGGER/%.o: ../ThirdParty/Segger/SEGGER/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DSTM32 -DSTM32F4 -DSTM32F407VGTx -DSTM32F407G_DISC1 -DDEBUG -DSTM32F40XX -DSTM32F40_41xxx -DUSE_STDPERIPH_DRIVER -I"C:/Users/Utilisateur/workspace/Semaphore/StdPeriph_Driver/inc" -I"C:/Users/Utilisateur/workspace/Semaphore/ThirdParty/Segger/Config" -I"C:/Users/Utilisateur/workspace/Semaphore/ThirdParty/Segger/OS" -I"C:/Users/Utilisateur/workspace/Semaphore/ThirdParty/Segger/SEGGER" -I"C:/Users/Utilisateur/workspace/Semaphore/ThirdParty/FreeRTOS/org/Source/portable/GCC/ARM_CM4F" -I"C:/Users/Utilisateur/workspace/Semaphore/Config" -I"C:/Users/Utilisateur/workspace/Semaphore/ThirdParty/FreeRTOS/org/Source/include" -I"C:/Users/Utilisateur/workspace/Semaphore/inc" -I"C:/Users/Utilisateur/workspace/Semaphore/CMSIS/device" -I"C:/Users/Utilisateur/workspace/Semaphore/CMSIS/core" -O0 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


