cmake_minimum_required(VERSION 3.10)

project(npower_app LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Core Quick REQUIRED)

add_compile_options(-Wall -Wextra -pedantic)

include("${PROJECT_SOURCE_DIR}/cmake/CPMdependencies.cmake")

add_executable(npower_app
  src/main.cpp
  forms/qml.qrc
)

add_library(fake_sensors
    "${PROJECT_SOURCE_DIR}/include/fake_sensors.hpp"
    "${PROJECT_SOURCE_DIR}/src/fake_sensors.cpp"
)

target_compile_definitions(npower_app
  PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>
)

target_link_libraries(npower_app
  PRIVATE
  Qt5::Core
  Qt5::Quick
  npower_sensors
  fake_sensors
)

target_include_directories(npower_app
    PRIVATE
    "${npower_sensors_SOURCE_DIR}/include"
    "${PROJECT_SOURCE_DIR}/include"
)

target_include_directories(fake_sensors
    PUBLIC
    "${PROJECT_SOURCE_DIR}/include"
)
