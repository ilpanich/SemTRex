################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Common/CompositeEventTemplate.cpp \
../Common/OpTree.cpp \
../Common/QueryItem.cpp \
../Common/TimeMs.cpp \
../Common/Timer.cpp 

CC_SRCS += \
../Common/Funs.cc 

OBJS += \
./Common/CompositeEventTemplate.o \
./Common/Funs.o \
./Common/OpTree.o \
./Common/QueryItem.o \
./Common/TimeMs.o \
./Common/Timer.o 

CC_DEPS += \
./Common/Funs.d 

CPP_DEPS += \
./Common/CompositeEventTemplate.d \
./Common/OpTree.d \
./Common/QueryItem.d \
./Common/TimeMs.d \
./Common/Timer.d 


# Each subdirectory must supply rules for building sources it contributes
Common/%.o: ../Common/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Common/%.o: ../Common/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


