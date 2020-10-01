cmake_minimum_required(VERSION 2.8.12)

project(parameters)

if(NOT MSVC)
    add_definitions("-std=c++11 -Wall -Wextra -Werror")
else()
    add_definitions("-std=c++11 -WX -W2")
endif()

find_package(MAVSDK REQUIRED)

add_executable(parameters
    parameters.cpp
)

target_link_libraries(parameters
    MAVSDK::mavsdk_param
    MAVSDK::mavsdk_telemetry
    MAVSDK::mavsdk
)
