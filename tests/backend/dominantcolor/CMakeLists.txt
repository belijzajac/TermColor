set(DOMINANTCOLORTEST_TARGET "DominantColorTest")

add_executable(${DOMINANTCOLORTEST_TARGET})

target_sources(${DOMINANTCOLORTEST_TARGET}
  PRIVATE
  DominantColorTest.h
  DominantColorTest.cpp
)

target_link_libraries(${DOMINANTCOLORTEST_TARGET}
  PRIVATE
  Qt5::Core
  Qt5::Test
  ${OpenCV_LIBS}
)

add_test(DominantColorTest
  ${DOMINANTCOLORTEST_TARGET}
)
