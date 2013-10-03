################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Source/Packet/BufferedPacketUnmarshaller.cpp \
../Source/Packet/PacketMarshaller.cpp \
../Source/Packet/PingPkt.cpp \
../Source/Packet/UnSubPkt.cpp 

OBJS += \
./Source/Packet/BufferedPacketUnmarshaller.o \
./Source/Packet/PacketMarshaller.o \
./Source/Packet/PingPkt.o \
./Source/Packet/UnSubPkt.o 

CPP_DEPS += \
./Source/Packet/BufferedPacketUnmarshaller.d \
./Source/Packet/PacketMarshaller.d \
./Source/Packet/PingPkt.d \
./Source/Packet/UnSubPkt.d 


# Each subdirectory must supply rules for building sources it contributes
Source/Packet/%.o: ../Source/Packet/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/home/gmc/git/greenmove/TRex2 -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


