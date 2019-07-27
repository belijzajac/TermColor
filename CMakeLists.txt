cmake_minimum_required(VERSION 3.13)
project(TermColor)

set(CMAKE_CXX_STANDARD 17)

# For running moc/uic/rcc automatically (for Qt5)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Look for moc files in the binary dir
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(OpenCV 3.2 REQUIRED)

find_package(Qt5 5.11.3 REQUIRED
  COMPONENTS
  Core
  Gui
  Widgets
)

# For UNIT testing
enable_testing()
find_package(Qt5Test REQUIRED)
add_subdirectory(tests)

# Project source code location
add_subdirectory(src)
