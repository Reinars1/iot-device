cmake_minimum_required(VERSION 3.20)
project(air-quality-monitor LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

add_executable(${PROJECT_NAME}
    src/main.cpp
    src/sensor.cpp
    src/temperature_sensor.cpp
    src/humidity_sensor.cpp
    # src/ring_buffer.cpp
    # src/uploader.cpp
    src/logger.cpp
    # src/power.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE include)