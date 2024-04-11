file(GLOB_RECURSE TEST_SOURCE_FILES
  *.cpp
  *.cc)

if(not ${TEST_SOURCE_FILES})
  add_executable(fanya_parking_test ${TEST_SOURCE_FILES})
endif()