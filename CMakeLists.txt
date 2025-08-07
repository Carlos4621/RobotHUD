cmake_minimum_required(VERSION 3.16)

project(RobotHUD VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_PREFIX_PATH "$ENV{HOME}/Qt/6.8.1/gcc_64/lib/cmake")

find_package(Qt6 REQUIRED COMPONENTS Core Widgets SerialPort)
find_package(SDL2 REQUIRED)

# Add the EasyLoRa_Widget subdirectory
add_subdirectory(EasyLoRa_Widget)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOUIC_SEARCH_PATHS ui EasyLoRa_Widget/ui)

set(SOURCES
    src/main.cpp
    src/RobotHUD.cpp
    src/Controller.cpp
)

set(HEADERS
    include/RobotHUD.h
    include/RobotHUDExceptions.h
    include/Controller.h
)

set(UI_FILES
    ui/RobotHUD.ui
)

add_executable(RobotHUD ${SOURCES} ${HEADERS} ${UI_FILES})

target_link_libraries(RobotHUD 
    Qt6::Core 
    Qt6::Widgets 
    Qt6::SerialPort
    SDL2::SDL2 
    SDL2::SDL2main
    EasyLoRa_WidgetPlugin
)

target_include_directories(RobotHUD PRIVATE 
    include 
    src 
    ui 
    EasyLoRa_Widget/include
    EasyLoRa_Widget/EasyLoRa/include
    EasyLoRa_Widget/EasyLoRa/external/serial/include
    ${EASYLORA_INCLUDE_DIRS}
)
