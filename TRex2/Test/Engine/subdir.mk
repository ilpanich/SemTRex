################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Engine/CompositeEventGenerator.cpp \
../Engine/TRexEngine.cpp 

CC_SRCS += \
../Engine/AbstractConstraintIndex.cc \
../Engine/BoolConstraintIndex.cc \
../Engine/FloatConstraintIndex.cc \
../Engine/IndexingTable.cc \
../Engine/IntConstraintIndex.cc \
../Engine/NoConstraintIndex.cc \
../Engine/Stack.cc \
../Engine/StacksRule.cc \
../Engine/StringConstraintIndex.cc 

OBJS += \
./Engine/AbstractConstraintIndex.o \
./Engine/BoolConstraintIndex.o \
./Engine/CompositeEventGenerator.o \
./Engine/FloatConstraintIndex.o \
./Engine/IndexingTable.o \
./Engine/IntConstraintIndex.o \
./Engine/NoConstraintIndex.o \
./Engine/Stack.o \
./Engine/StacksRule.o \
./Engine/StringConstraintIndex.o \
./Engine/TRexEngine.o 

CC_DEPS += \
./Engine/AbstractConstraintIndex.d \
./Engine/BoolConstraintIndex.d \
./Engine/FloatConstraintIndex.d \
./Engine/IndexingTable.d \
./Engine/IntConstraintIndex.d \
./Engine/NoConstraintIndex.d \
./Engine/Stack.d \
./Engine/StacksRule.d \
./Engine/StringConstraintIndex.d 

CPP_DEPS += \
./Engine/CompositeEventGenerator.d \
./Engine/TRexEngine.d 


# Each subdirectory must supply rules for building sources it contributes
Engine/%.o: ../Engine/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/home/lele/git/SemTRex/rdf3x-0.3.5/include" -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Engine/%.o: ../Engine/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/home/lele/git/SemTRex/rdf3x-0.3.5/include" -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


