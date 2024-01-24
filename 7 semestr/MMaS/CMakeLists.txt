cmake_minimum_required(VERSION 3.5)

# Set C and C++ standards
set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_STANDARD 17)

# Set the toolchain file
set(CMAKE_TOOLCHAIN_FILE "${CMAKE_SOURCE_DIR}/generic-msp430-gcc.cmake")

# Name your project
project(msp)

#device dependent config, here for launchpad
set(MSP430_MCU msp430f5529)
set(MSP430_MCU_FREQ 1000000UL)

msp430_add_executable(msp
	./src/main.cpp
)

include_directories(msp
	PUBLIC /opt/ti/mspgcc/include
	PUBLIC ./libs
)
