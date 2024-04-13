include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(..)

add_executable(publisher publisher_example.cpp)
set_target_properties(publisher PROPERTIES FOLDER "Examples")
target_link_libraries(publisher ${COMM_LIB} ${PROTO} fanya_protocol protobuf)

# add_executable(subscriber subscriber_example.cpp)
# set_target_properties(subscriber PROPERTIES FOLDER "Examples")
# target_link_libraries(subscriber ${COMM_LIB} ${PROTO})


install(TARGETS publisher DESTINATION bin)
# install(TARGETS subscriber DESTINATION bin)


# install(DIRECTORY ${CONFILG_DIR}
#           DESTINATION ${CMAKE_BINARY_DIR}/bin)


