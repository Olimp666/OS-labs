cmake_minimum_required(VERSION 3.5.0)

project(lab2)

if(WIN32)
    set(PLATFORM "win")
else()
    set(PLATFORM "posix")
endif()

set(APP_SRCS ${PLATFORM}/main.cpp)
set(SUB_SRCS ${PLATFORM}/subprogram.cpp)
set(LIB_SRCS ${PLATFORM}/lib/bg.cpp)
set(LIB_HDRS ${PLATFORM}/lib/bg.hpp)

add_library(background SHARED ${LIB_HDRS} ${LIB_SRCS})
add_executable(main ${APP_SRCS} ${LIB_HDRS} ${LIB_SRCS})
add_executable(sub ${SUB_SRCS})

target_link_libraries(main background)