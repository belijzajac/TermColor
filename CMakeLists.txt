cmake_minimum_required(VERSION 3.13)
project(TermColor)

set(CMAKE_CXX_STANDARD 17)

# For running moc/uic/rcc automatically (for Qt5)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(OpenCV 3.2 REQUIRED)

find_package(Qt5 5.11.3 REQUIRED
  COMPONENTS
  Core
  Gui
  Widgets
)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_subdirectory(src)
