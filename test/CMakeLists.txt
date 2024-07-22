set(TARGET_DEFAULT_PREFIX "example_")
set(TARGET_DEFAULT_LABEL_PREFIX "Examples")
cmake_minimum_required(VERSION 3.6)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)


project(communication)
include(cmake/utils.cmake)
set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/output/ CACHE PATH "The install prefix" FORCE)
set(CONFIG_DIR ${PROJECT_SOURCE_DIR}/scripts)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_BINARY_DIR}/..")
file(GLOB_RECURSE PACKAGES ${CMAKE_BINARY_DIR}/../Find*.cmake)
foreach(PACKAGE ${PACKAGES})
  string(REGEX REPLACE ".*[/\\]Find(.*\).cmake" "\\1" PACKAGE_NAME ${PACKAGE})
  find_package(${PACKAGE_NAME} REQUIRED)
  if(${PACKAGE_NAME}_FOUND)
    message("find package ${PACKAGE_NAME}")
    include_directories(SYSTEM ${${PACKAGE_NAME}_INCLUDE_DIRS})
    link_directories(${${PACKAGE_NAME}_LIB_DIRS})
  endif()
endforeach()

message("[example][cmake] ENABLE_PERFETTO: ${ENABLE_PERFETTO}")
if (${ENABLE_PERFETTO})
  message("[example][cmake] link ${PERFETTO_SHARED_LIB}")
  list(APPEND SHARED_LIB ${PERFETTO_SHARED_LIB})
  message("[example][cmake] SHARED_LIB: ${SHARED_LIB}")
else()
  message("[example][cmake] no need to link ${PERFETTO_SHARED_LIB}")
endif()

if(MSVC)
  set(COMM_LIB ${SHARED_LIB})
else()
  set(COMM_LIB  ${SHARED_LIB})
endif()

add_subdirectory(pub_sub_example )

if(MSVC)
  set(INSTALL_BINARY_DIR ${CMAKE_BINARY_DIR}/bin/${CMAKE_BUILD_TYPE})
else()
  set(INSTALL_BINARY_DIR ${CMAKE_BINARY_DIR}/bin)
endif()

install(FILES
        ${CONFIG_DIR}/communication.json
        ${CONFIG_DIR}/mem_pool_config.json
        ${CONFIG_DIR}/pub.sh
        DESTINATION ${INSTALL_BINARY_DIR})

# if(INDEPENDENT_COMPILATION)
#   install(FILES
#           ${CONFIG_DIR}/communication.json
#           ${CONFIG_DIR}/mem_pool_config.json
#           DESTINATION  ${CMAKE_SOURCE_DIR}/output/${CMAKE_PROJECT_NAME}/bin)
# endif()

install(DIRECTORY ${CMAKE_BINARY_DIR}/bin
          DESTINATION ${CMAKE_SOURCE_DIR}/output/)