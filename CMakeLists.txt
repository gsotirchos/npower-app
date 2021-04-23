cmake_minimum_required(VERSION 3.5)

project(npower_app LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


set(Qt5Quick_DIR "/usr/lib/aarch64-linux-gnu/cmake/Qt5")
find_package(Qt5 COMPONENTS Core Quick REQUIRED)

add_executable(npower_app
  main.cpp
  qml.qrc
)

target_compile_definitions(npower_app
  PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)
target_link_libraries(npower_app
  PRIVATE Qt5::Core Qt5::Quick)
