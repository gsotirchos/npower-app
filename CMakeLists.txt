cmake_minimum_required(VERSION 3.10)

project(npower_app LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Core Quick REQUIRED)
include("${PROJECT_SOURCE_DIR}/cmake/CPMdependencies.cmake")

add_compile_options(-Wall -Wextra -pedantic)

add_executable(npower_app
    "${PROJECT_SOURCE_DIR}/src/main.cpp"
    "${PROJECT_SOURCE_DIR}/forms/qml.qrc"
)

add_library(backend
    "${PROJECT_SOURCE_DIR}/include/backend.hpp"
    "${PROJECT_SOURCE_DIR}/src/backend.cpp"
    "${PROJECT_SOURCE_DIR}/include/fake_sensors.hpp"
    "${PROJECT_SOURCE_DIR}/src/fake_sensors.cpp"
)

option(WITH_FAKE_SENSORS "Use simulated fake sensors." ON)

if(WITH_FAKE_SENSORS)
    target_compile_definitions(backend
        PUBLIC
        -DUSE_FAKE_SENSORS
    )
endif()

target_compile_definitions(npower_app
    PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>
)

target_link_libraries(npower_app
    PRIVATE
    Qt5::Core
    Qt5::Quick
    backend
)

target_link_libraries(backend
    PUBLIC
    Qt5::Core
    Qt5::Quick
    pthread
    npower_sensors
)

target_include_directories(npower_app
    PRIVATE
    "${PROJECT_SOURCE_DIR}/include"
)

target_include_directories(backend
    PUBLIC
    "${npower_sensors_SOURCE_DIR}/include"
    "${PROJECT_SOURCE_DIR}/include"
)
