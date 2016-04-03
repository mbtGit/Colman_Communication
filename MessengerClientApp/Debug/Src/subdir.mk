################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Src/Friend.cpp \
../Src/MThread.cpp \
../Src/Main.cpp \
../Src/MessengerClient.cpp \
../Src/MultipleTCPSocketsListener.cpp \
../Src/TCPSocket.cpp \
../Src/UDPMessenger.cpp \
../Src/UDPMessengerClient.cpp \
../Src/UDPSocket.cpp \
../Src/Utils.cpp 

OBJS += \
./Src/Friend.o \
./Src/MThread.o \
./Src/Main.o \
./Src/MessengerClient.o \
./Src/MultipleTCPSocketsListener.o \
./Src/TCPSocket.o \
./Src/UDPMessenger.o \
./Src/UDPMessengerClient.o \
./Src/UDPSocket.o \
./Src/Utils.o 

CPP_DEPS += \
./Src/Friend.d \
./Src/MThread.d \
./Src/Main.d \
./Src/MessengerClient.d \
./Src/MultipleTCPSocketsListener.d \
./Src/TCPSocket.d \
./Src/UDPMessenger.d \
./Src/UDPMessengerClient.d \
./Src/UDPSocket.d \
./Src/Utils.d 


# Each subdirectory must supply rules for building sources it contributes
Src/%.o: ../Src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


