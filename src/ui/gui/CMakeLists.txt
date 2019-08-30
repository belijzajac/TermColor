add_subdirectory(application)
add_subdirectory(model)
add_subdirectory(widget)

set(TERMCOLOR_SOURCES
  ${TERMCOLOR_SOURCES}
  ui/gui/MainWindow.h
  ui/gui/MainWindow.cpp
  PARENT_SCOPE
)
