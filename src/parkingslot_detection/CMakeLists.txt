# generate *.h and *.cpp from *.boleidl
add_definitions(-DWITH_FYJ5LOG)
set(TROS_MESSAGE_DIR ${PROJECT_BINARY_DIR}/trosgen)
file(GLOB_RECURSE PROTO_FILES ${PROJECT_SOURCE_DIR}/proto/*.boleidl)
foreach(PROTO_FILE ${PROTO_FILES})
  execute_process(COMMAND boleidlc --intf=bole2 -I${PROJECT_SOURCE_DIR}/proto/ --lang-type=cpp --out=${TROS_MESSAGE_DIR}/proto/ ${PROTO_FILE})
endforeach()

include_directories(
  # ${CMAKE_SOURCE_DIR}/src/parkingslot_detection/dependencies/j5/hlog/include
  # ${CMAKE_SOURCE_DIR}/src/parkingslot_detection/dependencies/j5/fmt/include
  # ${CMAKE_SOURCE_DIR}/src/parkingslot_detection/dependencies/j5/protobuf/include
  # ${CMAKE_SOURCE_DIR}/src/parkingslot_detection/dependencies/j5/message/include
  # ${CMAKE_SOURCE_DIR}/src/parkingslot_detection/dependencies/j5/schedulegroup/include
  # ${CMAKE_SOURCE_DIR}/src/parkingslot_detection/dependencies/j5/j5dvb_system/include
  # ${CMAKE_SOURCE_DIR}/src/parkingslot_detection/dependencies/j5/nlohmann_json/include
  # ${CMAKE_SOURCE_DIR}/src/parkingslot_detection/dependencies/j5/cppcodec/include
  # ${CMAKE_SOURCE_DIR}/src/parkingslot_detection/dependencies/j5/communication/include
  /opt/senseauto/senseauto-msgs/active/include/
  ${CMAKE_SOURCE_DIR}/src/parkingslot_detection/dependencies/j5/rscl/include
  /opt/senseauto/senseauto-framework-sdk/active/include
  /opt/senseauto/senseauto-3rdparty/active/3rdparty/include/
  ${CMAKE_SOURCE_DIR}/include
  ${CMAKE_SOURCE_DIR}/include/common
)

link_directories(
  # ${CMAKE_SOURCE_DIR}/src/parkingslot_detection/dependencies/j5/communication/lib
  # ${CMAKE_SOURCE_DIR}/src/parkingslot_detection/dependencies/j5/hlog/lib
  # ${CMAKE_SOURCE_DIR}/src/parkingslot_detection/dependencies/j5/fmt/lib
  # ${CMAKE_SOURCE_DIR}/src/parkingslot_detection/dependencies/j5/protobuf/lib
  # ${CMAKE_SOURCE_DIR}/src/parkingslot_detection/dependencies/j5/schedulegroup/lib
  # ${CMAKE_SOURCE_DIR}/src/parkingslot_detection/dependencies/j5/j5dvb_system/lib
  /opt/senseauto/senseauto-framework-sdk/active/lib
  /opt/senseauto_active/senseauto-3rdparty/3rdparty/lib/
  /opt/senseauto_active/senseauto-framework-sdk/3rdparty/cyber/lib
  /opt/senseauto_active/senseauto-framework-sdk/3rdparty/fastrtps/lib
  /opt/senseauto/senseauto-msgs/active/lib
  /opt/gcc-ubuntu-9.3.0-2020.03-x86_64-aarch64-linux-gnu/aarch64-linux-gnu/lib 
  /opt/senseauto/senseauto-perception/active/components/rscl_perception_camera/lib 
  /#opt/senseauto/senseauto-perception/active/components/rscl_perception_uss/lib
  /opt/senseauto/senseauto-perception/active/lib 
  #/userdata/j5_nfs/qcg/fy/parking/tros_common_lib 
)


file(GLOB_RECURSE SOURCES *.cpp
  ${PROJECT_SOURCE_DIR}/src/common/utils.cpp
  ${TROS_MESSAGE_DIR}/proto/*.cpp
  ${TROS_MESSAGE_DIR}/proto/*.cc)

add_library(psd SHARED ${SOURCES})
target_include_directories(psd
  PUBLIC
    ${PROJECT_SOURCE_DIR}/include
  PRIVATE
    ${TROS_MESSAGE_DIR}/proto
    ${PROJECT_SOURCE_DIR}/src
    ${TROS_MESSAGE_DIR})

# message(FATAL_ERROR "test ${PROTO}")
target_link_libraries(psd 
                  ${COMMON_LIB} 
                  ${HOBOT_BOLE_PROTOCOL_LIB} 
                  ${FANYA_PROTO_LIB} 
                  ${PROTO}
                  # fy_pub_component
                  rscl_perception_rd_so
                  ad_rscl
                  ad_msg_bridge_manager
                  time
                  ad_rscl_pb
                  atomic
                  glog)

# install stage
install(TARGETS psd  DESTINATION ${TROS_INSTALL_LIB})