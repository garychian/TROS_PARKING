# generate *.h and *.cpp from *.boleidl
add_definitions(-DWITH_FYJ5LOG)
set(TROS_MESSAGE_DIR ${PROJECT_BINARY_DIR}/trosgen)
file(GLOB_RECURSE PROTO_FILES ${PROJECT_SOURCE_DIR}/proto/*.boleidl)
foreach(PROTO_FILE ${PROTO_FILES})
  execute_process(COMMAND boleidlc --intf=bole2 -I${PROJECT_SOURCE_DIR}/proto/ --lang-type=cpp --out=${TROS_MESSAGE_DIR}/proto/ ${PROTO_FILE})
endforeach()

include_directories(
  /opt/senseauto/senseauto-msgs/active/include/
  ${CMAKE_SOURCE_DIR}/src/perception_od/dependencies/j5/rscl/include/
  /opt/senseauto/senseauto-framework-sdk/active/include
  /opt/senseauto/senseauto-3rdparty/active/3rdparty/include/
  ${CMAKE_SOURCE_DIR}/include
  ${CMAKE_SOURCE_DIR}/include/common
)

link_directories(
  /opt/senseauto/senseauto-framework-sdk/active/lib
  /opt/senseauto_active/senseauto-3rdparty/3rdparty/lib/
  /opt/senseauto_active/senseauto-framework-sdk/3rdparty/cyber/lib
  /opt/senseauto_active/senseauto-framework-sdk/3rdparty/fastrtps/lib
  /opt/senseauto/senseauto-msgs/active/lib
  /opt/gcc-ubuntu-9.3.0-2020.03-x86_64-aarch64-linux-gnu/aarch64-linux-gnu/lib 
  /opt/senseauto/senseauto-perception/active/components/rscl_perception_camera/lib 
  /opt/senseauto/senseauto-perception/active/lib  
)

file(GLOB_RECURSE SOURCES *.cpp
  ${PROJECT_SOURCE_DIR}/src/common/utils.cpp
  ${TROS_MESSAGE_DIR}/proto/*.cpp
  ${TROS_MESSAGE_DIR}/proto/*.cc)

add_library(rd SHARED ${SOURCES})
target_include_directories(rd
  PUBLIC
    ${PROJECT_SOURCE_DIR}/include
  PRIVATE
    ${TROS_MESSAGE_DIR}/proto
    ${PROJECT_SOURCE_DIR}/src
    ${TROS_MESSAGE_DIR})

target_link_libraries(rd 
                      ${COMMON_LIB} 
                      ${HOBOT_BOLE_PROTOCOL_LIB} 
                      ${FANYA_PROTO_LIB} 
                      ${PROTO} 
                      opencv_world
                      rscl_perception_rd_so
                      ad_rscl
                      ad_msg_bridge_manager
                      time
                      ad_rscl_pb
                      atomic
                      glog)    

# install stage
install(TARGETS rd  DESTINATION ${TROS_INSTALL_LIB})