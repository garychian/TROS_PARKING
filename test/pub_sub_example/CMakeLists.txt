include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(..)

add_executable(publisher publisher_example.cpp)
set_target_properties(publisher PROPERTIES FOLDER "Examples")
target_link_libraries(publisher ${COMM_LIB} ${PROTO} fanya_protocol hobot-bole-protocol protobuf)

add_executable(loc_publisher loc_publisher_example.cpp)
set_target_properties(loc_publisher PROPERTIES FOLDER "Examples")
target_link_libraries(loc_publisher ${COMM_LIB} ${PROTO} fanya_protocol hobot-bole-protocol protobuf)

# add_executable(subscriber subscriber_example.cpp)
# set_target_properties(subscriber PROPERTIES FOLDER "Examples")
# target_link_libraries(subscriber ${COMM_LIB} ${PROTO})

add_executable(loc_subscriber loc_subscriber_example.cpp)
set_target_properties(loc_subscriber PROPERTIES FOLDER "Examples")
target_link_libraries(loc_subscriber ${COMM_LIB} ${PROTO} fanya_protocol hobot-bole-protocol protobuf)


install(TARGETS publisher DESTINATION bin)
install(TARGETS loc_publisher DESTINATION bin)
# install(TARGETS subscriber DESTINATION bin)
install(TARGETS loc_subscriber DESTINATION bin)

# install(DIRECTORY ${CONFILG_DIR}
#           DESTINATION ${CMAKE_BINARY_DIR}/bin)


