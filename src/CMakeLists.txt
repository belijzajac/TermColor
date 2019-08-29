set(TERMCOLOR_SOURCES
  ${TERMCOLOR_SOURCES}
  main.cpp
)

add_subdirectory(backend)
add_subdirectory(ui)

set(TERMCOLOR_TARGET "TermColor")

add_executable(${TERMCOLOR_TARGET}
  ${TERMCOLOR_SOURCES}
)

target_link_libraries(${TERMCOLOR_TARGET}
  PRIVATE
  Qt5::Core
  Qt5::Gui
  Qt5::Widgets
  ${OpenCV_LIBS}
)
