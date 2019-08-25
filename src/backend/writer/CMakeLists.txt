add_subdirectory(konsolewriter)
add_subdirectory(xfce4terminalwriter)

set(TERMCOLOR_SOURCES
  ${TERMCOLOR_SOURCES}
  backend/writer/Writer.h
  backend/writer/Writer.cpp
  PARENT_SCOPE
)
