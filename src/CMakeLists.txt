# generate *.h and *.cpp from *.boleidl

set(PROTOBUF_LIB protobuf)
set(HOBOT_BOLE_PROTOCOL_LIB hobot-bole-protocold)
set(TROS_COMMON_LIB libprotobuf)
set(PROTOBUF_LIB libprotobuf)
set(DATAFLOW_LIB libprotobuf)

set(TROS_MESSAGE_DIR ${PROJECT_BINARY_DIR}/trosgen)
file(GLOB_RECURSE PROTO_FILES ${PROJECT_SOURCE_DIR}/proto/*.boleidl)
# foreach(PROTO_FILE ${PROTO_FILES})
#   execute_process(COMMAND boleidlc --intf=bole2 -I${PROJECT_SOURCE_DIR}/proto/ --lang-type=cpp --out=${TROS_MESSAGE_DIR}/proto/ ${PROTO_FILE})
# endforeach()

# file(GLOB_RECURSE SOURCES *.cpp
#   ${TROS_MESSAGE_DIR}/proto/*.cpp
#   ${TROS_MESSAGE_DIR}/proto/*.cc)

# add_library(test SHARED ${SOURCES})
# target_include_directories(test
#   PUBLIC
#     ${PROJECT_SOURCE_DIR}/include
#   PRIVATE
#     ${TROS_MESSAGE_DIR}/proto
#     ${PROJECT_SOURCE_DIR}/src
#     ${TROS_MESSAGE_DIR})

# if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
#   target_link_libraries(test
#     dataflowd
#     communicationd
#     timed
#     hlogd
#     schedulegroupd
#     fmt
#     ${PROTO})
# else()
#   target_link_libraries(test
#     dataflow
#     communication
#     time
#     hlog
#     schedulegroup
#     fmt
#     ${PROTO})
# endif()

add_subdirectory(apa_handle)
add_subdirectory(location_map)
add_subdirectory(parkingslot_detection)
add_subdirectory(ipm_transformation)
# add_subdirectory(object_detect)
# add_subdirectory(freespace)


# install stage
#   binary should be installed into ${TROS_INSTALL_BIN}
#   library should be installed into ${TROS_INSTALL_LIB}
# install(TARGETS test  DESTINATION ${TROS_INSTALL_LIB})


# this required by tros package manager pack command, THIS CAN NOT BE DELTED
# install(DIRECTORY ${PROJECT_SOURCE_DIR}/include DESTINATION ${TROS_INSTALL_PREFIX})
install(DIRECTORY ${PROJECT_SOURCE_DIR}/config DESTINATION ${TROS_INSTALL_PREFIX})

set(SCRIPTS_DIR "${CMAKE_SOURCE_DIR}/scripts")
install(FILES ${SCRIPTS_DIR}/init.sh DESTINATION ${CMAKE_SOURCE_DIR}/output/parking)
install(FILES ${SCRIPTS_DIR}/communication_tool.sh DESTINATION ${CMAKE_SOURCE_DIR}/output/parking)