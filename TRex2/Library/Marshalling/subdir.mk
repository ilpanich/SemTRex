################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Marshalling/Marshaller.cpp \
../Marshalling/Unmarshaller.cpp 

OBJS += \
./Marshalling/Marshaller.o \
./Marshalling/Unmarshaller.o 

CPP_DEPS += \
./Marshalling/Marshaller.d \
./Marshalling/Unmarshaller.d 


# Each subdirectory must supply rules for building sources it contributes
Marshalling/%.o: ../Marshalling/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


