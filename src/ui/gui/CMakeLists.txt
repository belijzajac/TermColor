add_subdirectory(colorbutton)
add_subdirectory(colorstablewidget)
add_subdirectory(displaywidget)
add_subdirectory(guimodel)
add_subdirectory(imagedropwidget)
add_subdirectory(droparea)
add_subdirectory(exportwidget)
add_subdirectory(bgfgchooser)

set(TERMCOLOR_SOURCES
  ${TERMCOLOR_SOURCES}
  ui/gui/MainWindow.h
  ui/gui/MainWindow.cpp
  PARENT_SCOPE
)
