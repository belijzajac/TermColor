add_subdirectory(model)
add_subdirectory(widget)
add_subdirectory(controller)
add_subdirectory(utils)

set(TERMCOLOR_SOURCES
  ${TERMCOLOR_SOURCES}
  gui/MainWindow.h
  gui/MainWindow.cpp
  PARENT_SCOPE
)
