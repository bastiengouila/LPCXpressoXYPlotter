################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/GParser.cpp \
../src/LPCXProject.cpp \
../src/XY-Plotter.cpp \
../src/cr_cpp_config.cpp \
../src/cr_startup_lpc15xx.cpp \
../src/drawingObject.cpp 

C_SRCS += \
../src/ITM_write.c \
../src/crp.c \
../src/sysinit.c 

OBJS += \
./src/GParser.o \
./src/ITM_write.o \
./src/LPCXProject.o \
./src/XY-Plotter.o \
./src/cr_cpp_config.o \
./src/cr_startup_lpc15xx.o \
./src/crp.o \
./src/drawingObject.o \
./src/sysinit.o 

CPP_DEPS += \
./src/GParser.d \
./src/LPCXProject.d \
./src/XY-Plotter.d \
./src/cr_cpp_config.d \
./src/cr_startup_lpc15xx.d \
./src/drawingObject.d 

C_DEPS += \
./src/ITM_write.d \
./src/crp.d \
./src/sysinit.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C++ Compiler'
	arm-none-eabi-c++ -D__NEWLIB__ -DDEBUG -D__CODE_RED -DCORE_M3 -D__USE_LPCOPEN -DCPP_USE_HEAP -D__LPC15XX__ -I"C:\Users\Olli\Documents\LPCXpresso_8.0.0_526\Project\lpc_board_nxp_lpcxpresso_1549\inc" -I"C:\Users\Olli\Documents\LPCXpresso_8.0.0_526\Project\lpc_chip_15xx\inc" -I"C:\Users\Olli\Documents\LPCXpresso_8.0.0_526\Project\XY-Plotter\freertos\inc" -I"C:\Users\Olli\Documents\LPCXpresso_8.0.0_526\Project\XY-Plotter\src" -O0 -fno-common -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -fno-rtti -fno-exceptions -mcpu=cortex-m3 -mthumb -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__NEWLIB__ -DDEBUG -D__CODE_RED -DCORE_M3 -D__USE_LPCOPEN -DCPP_USE_HEAP -D__LPC15XX__ -I"C:\Users\Olli\Documents\LPCXpresso_8.0.0_526\Project\lpc_board_nxp_lpcxpresso_1549\inc" -I"C:\Users\Olli\Documents\LPCXpresso_8.0.0_526\Project\lpc_chip_15xx\inc" -I"C:\Users\Olli\Documents\LPCXpresso_8.0.0_526\Project\XY-Plotter\freertos\inc" -I"C:\Users\Olli\Documents\LPCXpresso_8.0.0_526\Project\XY-Plotter\src" -O0 -fno-common -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -mcpu=cortex-m3 -mthumb -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


