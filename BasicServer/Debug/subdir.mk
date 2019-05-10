################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../http_response.c \
../server.c \
../utilities.c 

O_SRCS += \
../http_parser.o \
../http_response.o \
../server.o \
../utilities.o 

OBJS += \
./http_response.o \
./server.o \
./utilities.o 

C_DEPS += \
./http_response.d \
./server.d \
./utilities.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


