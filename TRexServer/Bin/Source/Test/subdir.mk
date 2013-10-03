################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Source/Test/RuleGathering.cpp \
../Source/Test/RuleGathering2.cpp \
../Source/Test/RuleGathering3.cpp \
../Source/Test/RuleR0.cpp \
../Source/Test/RuleR1.cpp \
../Source/Test/RuleR5.cpp \
../Source/Test/RuleStoppedPosition.cpp \
../Source/Test/TestResultListener.cpp \
../Source/Test/TestRule.cpp \
../Source/Test/RuleTheft.cpp \
../Source/Test/RuleTheft2.cpp \
../Source/Test/RuleNotGivenBack.cpp \
../Source/Test/RuleOpenDoors.cpp 

OBJS += \
./Source/Test/RuleGathering.o \
./Source/Test/RuleGathering2.o \
./Source/Test/RuleGathering3.o \
./Source/Test/RuleR0.o \
./Source/Test/RuleR1.o \
./Source/Test/RuleR5.o \
./Source/Test/RuleStoppedPosition.o \
./Source/Test/TestResultListener.o \
./Source/Test/TestRule.o \
./Source/Test/RuleTheft.o \
./Source/Test/RuleTheft2.o \
./Source/Test/RuleNotGivenBack.o \
./Source/Test/RuleOpenDoors.o 

CPP_DEPS += \
./Source/Test/RuleGathering.d \
./Source/Test/RuleGathering2.d \
./Source/Test/RuleGathering3.d \
./Source/Test/RuleR0.d \
./Source/Test/RuleR1.d \
./Source/Test/RuleR5.d \
./Source/Test/RuleStoppedPosition.d \
./Source/Test/TestResultListener.d \
./Source/Test/TestRule.d \
./Source/Test/RuleTheft.d \
./Source/Test/RuleTheft2.d \
./Source/Test/RuleNotGivenBack.d \
./Source/Test/RuleOpenDoors.d


# Each subdirectory must supply rules for building sources it contributes
Source/Test/%.o: ../Source/Test/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/home/gmc/git/greenmove/TRex2 -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


