cmake_minimum_required(VERSION 3.5.0)

project(lab3)

if(WIN32)
    set(PLATFORM "win")
else()
    set(PLATFORM "posix")
endif()

set(APP_SRCS ./src/main.cpp ./src/my_serial.hpp)
set(SIM_SRCS ./src/simulator.cpp ./src/my_serial.hpp)

add_executable(main ${APP_SRCS})
add_executable(simulator ${SIM_SRCS})