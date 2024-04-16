# generate *.h and *.cpp from *.boleidl
set(TROS_MESSAGE_DIR ${PROJECT_BINARY_DIR}/trosgen)
file(GLOB_RECURSE PROTO_FILES ${PROJECT_SOURCE_DIR}/proto/*.boleidl)
foreach(PROTO_FILE ${PROTO_FILES})
  execute_process(COMMAND boleidlc --intf=bole2 -I${PROJECT_SOURCE_DIR}/proto/ --lang-type=cpp --out=${TROS_MESSAGE_DIR}/proto/ ${PROTO_FILE})
endforeach()

file(GLOB_RECURSE SOURCES *.cpp
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

target_link_libraries(psd ${COMMON_LIB} ${HOBOT_BOLE_PROTOCOL_LIB} ${FANYA_PROTO_LIB} ${PROTO})

# install stage
install(TARGETS psd  DESTINATION ${TROS_INSTALL_LIB})