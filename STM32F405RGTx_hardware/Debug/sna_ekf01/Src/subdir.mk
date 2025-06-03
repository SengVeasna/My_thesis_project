################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../snaekf01/Src/ekf10Init.c \
../snaekf01/Src/mcalib.c \
../snaekf01/Src/rtGetInf.c \
../snaekf01/Src/rtGetNaN.c \
../snaekf01/Src/rt_nonfinite.c 

OBJS += \
./snaekf01/Src/ekf10Init.o \
./snaekf01/Src/mcalib.o \
./snaekf01/Src/rtGetInf.o \
./snaekf01/Src/rtGetNaN.o \
./snaekf01/Src/rt_nonfinite.o 

C_DEPS += \
./snaekf01/Src/ekf10Init.d \
./snaekf01/Src/mcalib.d \
./snaekf01/Src/rtGetInf.d \
./snaekf01/Src/rtGetNaN.d \
./snaekf01/Src/rt_nonfinite.d 


# Each subdirectory must supply rules for building sources it contributes
snaekf01/Src/%.o snaekf01/Src/%.su snaekf01/Src/%.cyclo: ../snaekf01/Src/%.c snaekf01/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F405xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"D:/STM32CubeIDE/workspace_1.12.0/Thesis_ProjectF405RGT/snaekf01" -I"D:/STM32CubeIDE/workspace_1.12.0/Thesis_ProjectF405RGT/snaekf01/Inc" -I"D:/STM32CubeIDE/workspace_1.12.0/Thesis_ProjectF405RGT/snaekf01/Src" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-snaekf01-2f-Src

clean-snaekf01-2f-Src:
	-$(RM) ./snaekf01/Src/ekf10Init.cyclo ./snaekf01/Src/ekf10Init.d ./snaekf01/Src/ekf10Init.o ./snaekf01/Src/ekf10Init.su ./snaekf01/Src/mcalib.cyclo ./snaekf01/Src/mcalib.d ./snaekf01/Src/mcalib.o ./snaekf01/Src/mcalib.su ./snaekf01/Src/rtGetInf.cyclo ./snaekf01/Src/rtGetInf.d ./snaekf01/Src/rtGetInf.o ./snaekf01/Src/rtGetInf.su ./snaekf01/Src/rtGetNaN.cyclo ./snaekf01/Src/rtGetNaN.d ./snaekf01/Src/rtGetNaN.o ./snaekf01/Src/rtGetNaN.su ./snaekf01/Src/rt_nonfinite.cyclo ./snaekf01/Src/rt_nonfinite.d ./snaekf01/Src/rt_nonfinite.o ./snaekf01/Src/rt_nonfinite.su

.PHONY: clean-snaekf01-2f-Src

