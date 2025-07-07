cmake_minimum_required(VERSION 3.30)

project(Example C CXX)

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR cortex-m4)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_C_FLAGS_DEBUG "-g3 -Og -DDEBUG")
set(CMAKE_C_FLAGS_RELEASE "-O2 -DNDEBUG")

set(CMAKE_CXX_FLAGS_DEBUG "-g3 -Og -DDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -DNDEBUG")

set(ARCH_FLAGS
    -mcpu=cortex-m4
    -mfpu=fpv4-sp-d16
    -mfloat-abi=hard
    -mthumb
    -mabi=aapcs
)

set(COMPILE_FLAGS
    -ffunction-sections
    -fdata-sections
)

set(LINK_FLAGS
    -Wl,--gc-sections -static
    -T ${CMAKE_SOURCE_DIR}/../linker/linker_script.ld
    -flto
    -Wl,--start-group -lc -lm -lstdc++ -lsupc++ -Wl,--end-group
    -Wl,--print-memory-usage
    -u _printf_float
    -Xlinker -Map=output.map
    --specs=nosys.specs
)

set(SOURCE_FILES
    ./src/startup.cpp
    ./src/syscalls.cpp
    ./src/main.cpp
    vendor/CMSIS/Device/ST/STM32F4/Source/Templates/system_stm32f4xx.c

)

add_executable(firmware.elf ${SOURCE_FILES})

target_compile_definitions(firmware.elf PRIVATE STM32F411xE)

target_include_directories(firmware.elf PRIVATE
    vendor/CMSIS/Device/ST/STM32F4/Include
    vendor/CMSIS/CMSIS/Core/Include
    ./inc
)

target_compile_options(firmware.elf PRIVATE
    ${ARCH_FLAGS}
    ${COMPILE_FLAGS}
)

target_link_options(firmware.elf PRIVATE
    ${ARCH_FLAGS}
    ${LINK_FLAGS}
)