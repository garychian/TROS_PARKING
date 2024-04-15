# generate *.h and *.cpp from *.boleidl
set(TROS_MESSAGE_DIR ${PROJECT_BINARY_DIR}/trosgen)
file(GLOB_RECURSE PROTO_FILES ${PROJECT_SOURCE_DIR}/proto/*.boleidl)
foreach(PROTO_FILE ${PROTO_FILES})
  execute_process(COMMAND boleidlc --intf=bole2 -I${PROJECT_SOURCE_DIR}/proto/ --lang-type=cpp --out=${TROS_MESSAGE_DIR}/proto/ ${PROTO_FILE})
endforeach()

file(GLOB_RECURSE SOURCES *.cpp
  ${TROS_MESSAGE_DIR}/proto/*.cpp
  ${TROS_MESSAGE_DIR}/proto/*.cc)

add_library(loc SHARED ${SOURCES})
target_include_directories(loc
  PUBLIC
    ${PROJECT_SOURCE_DIR}/include
  PRIVATE
    ${TROS_MESSAGE_DIR}/proto
    ${PROJECT_SOURCE_DIR}/src
    ${TROS_MESSAGE_DIR})

if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
  target_link_libraries(loc
    dataflowd
    communicationd
    timed
    hlogd
    schedulegroupd
    fmt
    ${PROTO})
else()
  target_link_libraries(loc
    fanya_protocol
    dataflow
    communication
    time
    hlog
    schedulegroup
    fmt
    ${PROTO})
endif()

# install stage
#   binary should be installed into ${TROS_INSTALL_BIN}
#   library should be installed into ${TROS_INSTALL_LIB}
install(TARGETS loc  DESTINATION ${TROS_INSTALL_LIB})


# this required by tros package manager pack command, THIS CAN NOT BE DELTED
# install(DIRECTORY ${PROJECT_SOURCE_DIR}/include DESTINATION ${TROS_INSTALL_PREFIX})
# install(DIRECTORY ${PROJECT_SOURCE_DIR}/config DESTINATION ${TROS_INSTALL_PREFIX})

# set(INIT_FILE "${CMAKE_SOURCE_DIR}/config/init.sh")
# install(FILES ${INIT_FILE} DESTINATION ${CMAKE_SOURCE_DIR}/output/loc)