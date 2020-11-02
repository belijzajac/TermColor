set(DOMINANTCOLORTEST_TARGET "DominantColorTest")

add_executable(${DOMINANTCOLORTEST_TARGET})

# We need this because some source files depend on other headers
# which location is unknown to cmake
include_directories(
  ${CMAKE_SOURCE_DIR}/src
  ${PROJECT_SOURCE_DIR}/src/backend/dominantcolor/ # to have a shorter path to DominantColor.h
)

target_sources(${DOMINANTCOLORTEST_TARGET}
  PRIVATE
  DominantColorTest.h
  DominantColorTest.cpp
  ${CMAKE_SOURCE_DIR}/src/backend/dominantcolor/DominantColor.cpp
)

target_link_libraries(${DOMINANTCOLORTEST_TARGET}
  PRIVATE
  Qt5::Core
  Qt5::Test
  ${OpenCV_LIBS}
  std::filesystem
)

add_test(DominantColorTest
  ${DOMINANTCOLORTEST_TARGET}
)
