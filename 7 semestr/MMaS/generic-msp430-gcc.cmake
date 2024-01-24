# Toolchain CMake file for MSP430-GCC/AVRDude toolchain

# Needs the following variables:
# MSP430_MCU : MCU type
# MSP430_MCU_FREQ : Clock frequency (defines F_CPU)

# Generic AVR flags
set(MSP430_CFLAGS "-ffunction-sections -fdata-sections -Wall -Wextra" CACHE STRING "MSP430 compilation flags")
set(MSP430_LFLAGS "-Wl,--relax,--gc-sections" CACHE STRING "MSP430 link flags")

# Find toolchain programs
find_program(MSP430-GCC msp430-gcc)
find_program(MSP430-GXX msp430-g++)
find_program(MSP430-OBJCOPY msp430-objcopy)
find_program(MSP430-SIZE msp430-size)
find_program(MSP430-OBJDUMP msp430-objdump)
find_program(MSPDEBUG mspdebug)

# If the programs is not found, try with msp430-elf-gcc interfix
if(NOT MSP430-GCC)
    find_program(MSP430-GCC msp430-elf-gcc)
endif(NOT MSP430-GCC)
if(NOT MSP430-GXX)
    find_program(MSP430-GXX msp430-elf-g++)
endif(NOT MSP430-GXX)
if(NOT MSP430-OBJCOPY)
    find_program(MSP430-OBJCOPY msp430-elf-objcopy)
endif(NOT MSP430-OBJCOPY)
if(NOT MSP430-SIZE)
    find_program(MSP430-SIZE msp430-elf-size)
endif(NOT MSP430-SIZE)
if(NOT MSP430-OBJDUMP)
    find_program(MSP430-OBJDUMP msp430-elf-objdump)
endif(NOT MSP430-OBJDUMP)

# If any of the programs is not found, fail
if(NOT MSP430-GCC)
    message(FATAL_ERROR "msp430-gcc not found")
endif(NOT MSP430-GCC)
if(NOT MSP430-GXX)
    message(FATAL_ERROR "msp430-g++ not found")
endif(NOT MSP430-GXX)
if(NOT MSP430-OBJCOPY)
    message(FATAL_ERROR "msp430-objcopy not found")
endif(NOT MSP430-OBJCOPY)
if(NOT MSP430-SIZE)
    message(FATAL_ERROR "msp430-size not found")
endif(NOT MSP430-SIZE)
if(NOT MSP430-OBJDUMP)
    message(FATAL_ERROR "msp430-objdump not found")
endif(NOT MSP430-OBJDUMP)
if(NOT MSPDEBUG)
    message(FATAL_ERROR "mspdebug not found")
endif(NOT MSPDEBUG)

# Define toolchain
set(CMAKE_SYSTEM_NAME Generic)

# Set C and C++ compilers
set(CMAKE_C_COMPILER ${MSP430-GCC})
set(CMAKE_CXX_COMPILER ${MSP430-GXX})

# Release build by default
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
endif(NOT CMAKE_BUILD_TYPE)

function(msp430_add_executable_compilation EXECUTABLE)
    set(EXECUTABLE_ELF "${EXECUTABLE}.elf")

    # Main target for the executable depends on ELF
    add_custom_target(${EXECUTABLE} ALL DEPENDS ${EXECUTABLE_ELF})

    # Compile and link ELF file
    add_executable(${EXECUTABLE_ELF} ${ARGN})
    set_target_properties(${EXECUTABLE_ELF} PROPERTIES 
        COMPILE_FLAGS "-mmcu=${MSP430_MCU} -DF_CPU=${MSP430_MCU_FREQ} ${MSP430_CFLAGS}"
        LINK_FLAGS "-mmcu=${MSP430_MCU} ${MSP430_LFLAGS}")

    # Display size info after compilation
    add_custom_command(TARGET ${EXECUTABLE} POST_BUILD
        COMMAND ${MSP430-SIZE} ${EXECUTABLE_ELF})
endfunction(msp430_add_executable_compilation)

function(msp430_add_executable_upload EXECUTABLE)
    add_custom_target(upload_${EXECUTABLE} 
        COMMAND ${MSPDEBUG} -q rf2500 "prog ${EXECUTABLE}.elf"
        DEPENDS ${EXECUTABLE})
endfunction(msp430_add_executable_upload)

function(msp430_add_executable EXECUTABLE)
    if(NOT MSP430_MCU)
        message(FATAL_ERROR "MSP430_MCU not defined")
    endif(NOT MSP430_MCU)
    msp430_add_executable_compilation(${EXECUTABLE} ${ARGN})
    msp430_add_executable_upload(${EXECUTABLE})
endfunction(msp430_add_executable)
