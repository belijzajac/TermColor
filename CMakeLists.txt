cmake_minimum_required(VERSION 3.13)
project(TermColor)

# We're using C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# For running moc/uic/rcc automatically (for Qt5)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Look for moc files in the binary dir
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# OpenCV
find_package(OpenCV REQUIRED)
if(OpenCV_VERSION VERSION_LESS "3.2")
  message(FATAL_ERROR "ERROR: Can't find OpenCV 3.2+")
endif()

# Qt5
find_package(Qt5 5.9.5 REQUIRED
  COMPONENTS
  Core
  Gui
  Widgets
)

# To have a shorter path to the include paths
include_directories(
  ${PROJECT_SOURCE_DIR}/external/rapidjson/include/
)

# For UNIT testing
enable_testing()
find_package(Qt5Test REQUIRED)
add_subdirectory(tests)

# Project source code location
add_subdirectory(src)
