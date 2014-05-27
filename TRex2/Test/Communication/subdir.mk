################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Communication/SimpleRoutingTable.cpp \
../Communication/TRexServer.cpp 

OBJS += \
./Communication/SimpleRoutingTable.o \
./Communication/TRexServer.o 

CPP_DEPS += \
./Communication/SimpleRoutingTable.d \
./Communication/TRexServer.d 


# Each subdirectory must supply rules for building sources it contributes
Communication/%.o: ../Communication/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/home/lele/git/SemTRex/rdf3x-0.3.5/include" -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


