cmake_minimum_required(VERSION 3.12)

include(pico_sdk_import.cmake)

project(remote C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()

add_executable(${PROJECT_NAME}
    ./src/main.cpp
)

add_library(signal ./libraries/signal.cpp)

target_link_libraries(signal
    pico_stdlib
    hardware_gpio
    hardware_adc
    hardware_i2c
    hardware_flash
    pico_i2c_slave
)

target_sources(${PROJECT_NAME} PRIVATE ./src/main.cpp)

target_link_libraries(${PROJECT_NAME}
    pico_stdlib
    hardware_gpio
    hardware_adc
    hardware_i2c
    hardware_flash
    pico_i2c_slave
    signal
)

pico_add_extra_outputs(${PROJECT_NAME})
pico_enable_stdio_usb(${PROJECT_NAME} 1)
pico_enable_stdio_uart(${PROJECT_NAME} 0)
