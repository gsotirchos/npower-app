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
find_package(Qt5QuickCompiler)

#qtquick_compiler_add_resources(RESOURCES forms/qml.qrc)
add_executable(npower_app
    main.cpp
    somelib.cpp
    forms/qml.qrc
    #${RESOURCES}
)

#qt5_use_modules(npower_app Quick Core)

target_compile_definitions(npower_app
    PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>
)

target_link_libraries(npower_app
    PRIVATE Qt5::Core Qt5::Quick
)
