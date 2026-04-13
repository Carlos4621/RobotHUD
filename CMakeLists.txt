cmake_minimum_required(VERSION 3.16)

project(RobotHUD VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_PREFIX_PATH "$ENV{HOME}/Qt/6.8.1/gcc_64/lib/cmake")

find_package(Qt6 REQUIRED COMPONENTS Core Widgets SerialPort Multimedia Concurrent)
find_package(SDL2 REQUIRED)
find_package(Protobuf REQUIRED)
find_package(OpenCV REQUIRED)

# Add the EasyLoRa subdirectory
set(BUILD_AS_LIBRARY ON CACHE BOOL "Build EasyLoRa as library" FORCE)
add_subdirectory(EasyLoRa)

# Add whisper.cpp as a vendored submodule
add_subdirectory(external/whisper.cpp)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOUIC_SEARCH_PATHS ui)

# Configure protobuf
set(PROTO_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/RobotHUD_Proto)
set(PROTO_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR}/proto_generated)

# Create directory for generated files
file(MAKE_DIRECTORY ${PROTO_BINARY_DIR})

# Define proto files
set(PROTO_FILES
    ${PROTO_SOURCE_DIR}/Buttons.proto
    ${PROTO_SOURCE_DIR}/Controller_Data.proto
    ${PROTO_SOURCE_DIR}/DPad.proto
    ${PROTO_SOURCE_DIR}/Joystick.proto
    ${PROTO_SOURCE_DIR}/WiFiPacket.proto
)

# Generate C++ files from proto files
protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS ${PROTO_FILES})

# Set the output directory for generated files
set_property(SOURCE ${PROTO_SRCS} ${PROTO_HDRS} PROPERTY GENERATED TRUE)

set(SOURCES
    src/main.cpp
    src/RobotHUD.cpp
    src/Controller.cpp
    src/EasyLoRa_Widget.cpp
    src/EasyUDP_Widget.cpp
    EasyUDP/EasyUDP.cpp
    src/Controller_Widget.cpp
    src/MatLabel.cpp
    src/MovementDetector.cpp
    src/QRDetector.cpp
    src/Susurrador.cpp
    src/VideoRecognizerConfiguration.cpp
    src/VideoRecognizer_Widget.cpp
    src/VoiceRecognizer.cpp
    src/VoiceRecorder.cpp
    src/YOLOv8Model.cpp
    ${PROTO_SRCS}
)

set(HEADERS
    include/RobotHUD.h
    include/RobotHUDExceptions.h
    include/Controller.h
    include/EasyLoRa_Widget.h
    include/EasyUDP_Widget.h
    EasyUDP/EasyUDP.h
    EasyUDP/IOCommons.hpp
    include/Controller_Widget.h
    include/MatLabel.h
    include/MovementDetector.h
    include/QRDetector.h
    include/Susurrador.h
    include/UIUtils.h
    include/VideoRecognizerConfiguration.h
    include/VideoRecognizer_Widget.h
    include/VoiceRecognizer.h
    include/VoiceRecorder.h
    include/YOLOv8Model.h
    ${PROTO_HDRS}
)

set(UI_FILES
    ui/RobotHUD.ui
    ui/EasyLoRa_Widget.ui
    ui/EasyUDP_Widget.ui
    ui/Controller_Widget.ui
    ui/VideoRecognizerConfiguration.ui
)

set(RESOURCE_FILES
    resources/hazmatClasses.txt
    resources/hazmatModel.onnx
)

add_executable(RobotHUD ${SOURCES} ${HEADERS} ${UI_FILES} ${RESOURCE_FILES})

target_link_libraries(RobotHUD 
    Qt6::Core 
    Qt6::Widgets 
    Qt6::SerialPort
    Qt6::Multimedia
    Qt6::Concurrent
    SDL2::SDL2 
    SDL2::SDL2main
    easylora
    protobuf::libprotobuf
    ${OpenCV_LIBS}
    whisper
)

target_include_directories(RobotHUD PRIVATE 
    include 
    src 
    ui 
    EasyLoRa/include
    EasyUDP
    external/serial/include
    ${CMAKE_CURRENT_BINARY_DIR}
    ${EASYLORA_INCLUDE_DIRS}
    ${OpenCV_INCLUDE_DIRS}
)
