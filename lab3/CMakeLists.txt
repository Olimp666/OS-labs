cmake_minimum_required(VERSION 3.5.0)

project(lab3)

if(WIN32)
    set(PLATFORM "win")
else()
    set(PLATFORM "posix")
endif()

set(APP_SRCS ./src/main.cpp)
set(UTL_SRCS ./src/util_${PLATFORM}.cpp)
set(UTL_HDRS ./src/util.hpp my_shmem.hpp)

add_executable(main ${APP_SRCS} ${UTL_SRCS})