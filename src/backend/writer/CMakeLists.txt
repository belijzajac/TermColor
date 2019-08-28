add_subdirectory(konsolewriter)
add_subdirectory(xfce4terminalwriter)
add_subdirectory(gnometerminalwriter)

set(TERMCOLOR_SOURCES
  ${TERMCOLOR_SOURCES}
  backend/writer/Writer.h
  backend/writer/Writer.cpp
  PARENT_SCOPE
)
