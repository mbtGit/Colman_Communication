################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Src/MThread.cpp \
../Src/Main.cpp \
../Src/Messenger.cpp \
../Src/MultipleTCPSocketsListener.cpp \
../Src/Room.cpp \
../Src/TCPMsnDispatcher.cpp \
../Src/TCPMsnServer.cpp \
../Src/TCPSocket.cpp \
../Src/User.cpp \
../Src/Utils.cpp 

OBJS += \
./Src/MThread.o \
./Src/Main.o \
./Src/Messenger.o \
./Src/MultipleTCPSocketsListener.o \
./Src/Room.o \
./Src/TCPMsnDispatcher.o \
./Src/TCPMsnServer.o \
./Src/TCPSocket.o \
./Src/User.o \
./Src/Utils.o 

CPP_DEPS += \
./Src/MThread.d \
./Src/Main.d \
./Src/Messenger.d \
./Src/MultipleTCPSocketsListener.d \
./Src/Room.d \
./Src/TCPMsnDispatcher.d \
./Src/TCPMsnServer.d \
./Src/TCPSocket.d \
./Src/User.d \
./Src/Utils.d 


# Each subdirectory must supply rules for building sources it contributes
Src/%.o: ../Src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


