################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../EngineTest/FilteringTest.cpp \
../EngineTest/RunTest.cpp \
../EngineTest/StacksRuleTest.cpp \
../EngineTest/TimeMsTest.cpp 

OBJS += \
./EngineTest/FilteringTest.o \
./EngineTest/RunTest.o \
./EngineTest/StacksRuleTest.o \
./EngineTest/TimeMsTest.o 

CPP_DEPS += \
./EngineTest/FilteringTest.d \
./EngineTest/RunTest.d \
./EngineTest/StacksRuleTest.d \
./EngineTest/TimeMsTest.d 


# Each subdirectory must supply rules for building sources it contributes
EngineTest/%.o: ../EngineTest/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


