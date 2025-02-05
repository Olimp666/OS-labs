cmake_minimum_required(VERSION 3.10.0)

project(lab4-5)

set(CMAKE_CXX_STANDARD 20)

set(APP_SRCS ./src/main.cpp ./src/my_serial.hpp ./src/server.hpp)
set(SIM_SRCS ./src/simulator.cpp ./src/my_serial.hpp)

file(COPY ${CMAKE_SOURCE_DIR}/src/public DESTINATION ${CMAKE_BINARY_DIR})

if(WIN32)
link_libraries(ws2_32 wsock32)
endif()

add_executable(main ${APP_SRCS})
add_executable(simulator ${SIM_SRCS})