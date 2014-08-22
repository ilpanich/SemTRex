################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Eval/BasicEval.cpp \
../Eval/EvalResultListener.cpp \
../Eval/EvaluationRunner.cpp \
../Eval/ParamHandler.cpp \
../Eval/Producer.cpp \
../Eval/RulesGenerator.cpp \
../Eval/RunEval.cpp \
../Eval/RunEvalCommon.cpp 

OBJS += \
./Eval/BasicEval.o \
./Eval/EvalResultListener.o \
./Eval/EvaluationRunner.o \
./Eval/ParamHandler.o \
./Eval/Producer.o \
./Eval/RulesGenerator.o \
./Eval/RunEval.o \
./Eval/RunEvalCommon.o 

CPP_DEPS += \
./Eval/BasicEval.d \
./Eval/EvalResultListener.d \
./Eval/EvaluationRunner.d \
./Eval/ParamHandler.d \
./Eval/Producer.d \
./Eval/RulesGenerator.d \
./Eval/RunEval.d \
./Eval/RunEvalCommon.d 


# Each subdirectory must supply rules for building sources it contributes
Eval/%.o: ../Eval/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/home/lele/git/SemTRex/rdf3x-0.3.5/include" -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


