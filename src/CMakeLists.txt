# generate *.h and *.cpp from *.boleidl

set(HOBOT_BOLE_PROTOCOL_LIB hobot-bole-protocol)
set(FANYA_PROTO_LIB fanya_protocol)
set(COMMON_LIB dataflow communication time hlog schedulegroup fmt vehicleio)

add_definitions("-Wall -g")

set(TROS_MESSAGE_DIR ${PROJECT_BINARY_DIR}/trosgen)
file(GLOB_RECURSE PROTO_FILES ${PROJECT_SOURCE_DIR}/proto/*.boleidl)

add_subdirectory(apa_handle)
add_subdirectory(location_map)
add_subdirectory(perception_rd)
add_subdirectory(ipm_transformation)
add_subdirectory(perception_od)
# add_subdirectory(freespace)

install(DIRECTORY ${PROJECT_SOURCE_DIR}/config DESTINATION ${TROS_INSTALL_PREFIX})

set(SCRIPTS_DIR "${CMAKE_SOURCE_DIR}/scripts")
install(FILES ${SCRIPTS_DIR}/init.sh DESTINATION ${CMAKE_SOURCE_DIR}/output/parking)
install(FILES ${SCRIPTS_DIR}/communication_tool.sh DESTINATION ${CMAKE_SOURCE_DIR}/output/parking)