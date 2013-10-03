################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Source/Connection/Connection.cpp \
../Source/Connection/ConnectionProxy.cpp \
../Source/Connection/PubPktListener.cpp \
../Source/Connection/RequestHandler.cpp \
../Source/Connection/SubscriptionTable.cpp 

OBJS += \
./Source/Connection/Connection.o \
./Source/Connection/ConnectionProxy.o \
./Source/Connection/PubPktListener.o \
./Source/Connection/RequestHandler.o \
./Source/Connection/SubscriptionTable.o 

CPP_DEPS += \
./Source/Connection/Connection.d \
./Source/Connection/ConnectionProxy.d \
./Source/Connection/PubPktListener.d \
./Source/Connection/RequestHandler.d \
./Source/Connection/SubscriptionTable.d 


# Each subdirectory must supply rules for building sources it contributes
Source/Connection/%.o: ../Source/Connection/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/home/gmc/git/greenmove/TRex2 -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


