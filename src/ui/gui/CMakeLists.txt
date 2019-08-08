add_subdirectory(colorbutton)
add_subdirectory(colorstablewidget)
add_subdirectory(displaywidget)
add_subdirectory(guimodel)
add_subdirectory(imagedropwidget)

set(TERMCOLOR_SOURCES
  ${TERMCOLOR_SOURCES}
  ui/gui/MainWindow.h
  ui/gui/MainWindow.cpp
  PARENT_SCOPE
)
