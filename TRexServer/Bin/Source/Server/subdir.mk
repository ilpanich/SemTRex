################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Source/Server/SOEPServer.cpp 

OBJS += \
./Source/Server/SOEPServer.o 

CPP_DEPS += \
./Source/Server/SOEPServer.d 


# Each subdirectory must supply rules for building sources it contributes
Source/Server/%.o: ../Source/Server/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/home/gmc/git/greenmove/TRex2 -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


