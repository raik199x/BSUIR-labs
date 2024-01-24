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
	# main
	./src/origin.cpp
	# CTS
	./libs/msp-exp/CTS/CTS_Layer.cpp
	./libs/msp-exp/CTS/CTS_HAL.cpp
	./libs/msp-exp/CTS/structure.cpp
	# F5xx_F6xx_Core_Lib
	./libs/msp-exp/F5xx_F6xx_Core_Lib/HAL_FLASH.cpp
	./libs/msp-exp/F5xx_F6xx_Core_Lib/HAL_PMAP.cpp
	./libs/msp-exp/F5xx_F6xx_Core_Lib/HAL_PMM.cpp
	./libs/msp-exp/F5xx_F6xx_Core_Lib/HAL_TLV.cpp
	./libs/msp-exp/F5xx_F6xx_Core_Lib/HAL_UCS.cpp
	# MSP-EXP430F5529_HAL
	./libs/msp-exp/MSP-EXP430F5529_HAL/HAL_AppUart.cpp
	./libs/msp-exp/MSP-EXP430F5529_HAL/HAL_Board.cpp
	./libs/msp-exp/MSP-EXP430F5529_HAL/HAL_Buttons.cpp
	./libs/msp-exp/MSP-EXP430F5529_HAL/HAL_Cma3000.cpp
	./libs/msp-exp/MSP-EXP430F5529_HAL/HAL_Dogs102x6.cpp
	./libs/msp-exp/MSP-EXP430F5529_HAL/HAL_Menu.cpp
	./libs/msp-exp/MSP-EXP430F5529_HAL/HAL_SDCard.cpp
	./libs/msp-exp/MSP-EXP430F5529_HAL/HAL_Wheel.cpp
	# FatFs
	./libs/msp-exp/FatFs/mmc.cpp
	./libs/msp-exp/FatFs/ff.cpp
)

include_directories(msp
	PUBLIC /opt/ti/mspgcc/include
	PUBLIC ./libs
)
