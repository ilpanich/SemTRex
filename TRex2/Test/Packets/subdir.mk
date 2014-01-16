################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Packets/AdvPkt.cpp \
../Packets/JoinPkt.cpp \
../Packets/PubPkt.cpp \
../Packets/RulePkt.cpp \
../Packets/RulePktValueReference.cpp \
../Packets/SubPkt.cpp 

OBJS += \
./Packets/AdvPkt.o \
./Packets/JoinPkt.o \
./Packets/PubPkt.o \
./Packets/RulePkt.o \
./Packets/RulePktValueReference.o \
./Packets/SubPkt.o 

CPP_DEPS += \
./Packets/AdvPkt.d \
./Packets/JoinPkt.d \
./Packets/PubPkt.d \
./Packets/RulePkt.d \
./Packets/RulePktValueReference.d \
./Packets/SubPkt.d 


# Each subdirectory must supply rules for building sources it contributes
Packets/%.o: ../Packets/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/home/lele/git/SemTRex/rdf3x-0.3.5/include" -O2 -g -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


