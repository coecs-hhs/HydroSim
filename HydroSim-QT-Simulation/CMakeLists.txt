cmake_minimum_required(VERSION 3.16)

# -------------------------
# Project settings
# -------------------------
project(HydroSimGUI VERSION 1.0 LANGUAGES CXX)

# Use C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable Qt automatic features
set(CMAKE_AUTOMOC ON)   # Automatically handle moc for Q_OBJECT
set(CMAKE_AUTORCC ON)   # Automatically handle Qt resource files
set(CMAKE_AUTOUIC ON)   # Automatically handle .ui files (not used here, but safe)

# Output directory for the executable
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

# Qt6 installation path (adjust to your Qt6 installation)
set(CMAKE_PREFIX_PATH "/home/glenn/Qt/6.10.0/gcc_64" CACHE PATH "Qt6 prefix path")

# -------------------------
# Find required Qt6 modules
# -------------------------
find_package(Qt6 REQUIRED COMPONENTS
    Core
    Gui
    Widgets
    Qml
    Quick
    Mqtt
)

# -------------------------
# Collect all device source/header files
# -------------------------
file(GLOB DEVICE_SOURCES "devices/*.cpp")
file(GLOB DEVICE_HEADERS "devices/*.h")

# -------------------------
# Add the main executable
# -------------------------
qt_add_executable(HydroSimGUI
    main.cpp
    backend.cpp
    backend.h
    ${DEVICE_SOURCES}
    ${DEVICE_HEADERS}
    mqttclientwrapper.cpp
    mqttclientwrapper.h
    resources.qrc
)

# -------------------------
# Add QML module
# -------------------------
qt_add_qml_module(HydroSimGUI
    URI HydroSimGUI
    VERSION 1.0
    QML_FILES qml/Main.qml
    RESOURCES resources.qrc
)

# -------------------------
# Link required Qt libraries
# -------------------------
target_link_libraries(HydroSimGUI PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::Qml
    Qt6::Quick
    Qt6::Mqtt
)
