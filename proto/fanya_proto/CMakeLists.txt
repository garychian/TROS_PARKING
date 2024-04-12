cmake_minimum_required(VERSION 2.8)

project(fanya-protocol)

if(${arch} STREQUAL "AARCH64_J3")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Os -fdata-sections -ffunction-sections -Wl,--gc-sections -flto -ffat-lto-objects")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Os -fdata-sections -ffunction-sections -Wl,--gc-sections -flto -ffat-lto-objects")
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Os -fdata-sections -ffunction-sections -Wl,--gc-sections -flto -ffat-lto-objects")
  set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -Os -fdata-sections -ffunction-sections -Wl,--gc-sections -flto -ffat-lto-objects")
endif()

#using ccache
find_program(CCACHE_PROGRAM ccache)
if(CCACHE_PROGRAM)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${CCACHE_PROGRAM}")
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 11)

set(CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR} ${CMAKE_MODULE_PATH})
set(CMAKE_PREFIX_PATH ${CMAKE_BINARY_DIR} ${CMAKE_PREFIX_PATH})


find_package(protobuf REQUIRED)
if(protobuf_FOUND)
  include_directories(SYSTEM ${protobuf_INCLUDE_DIRS})
  list(APPEND LINK_SEARCH_PATH ${protobuf_LIB_DIRS})
endif()
link_directories(${LINK_SEARCH_PATH})

if (${ACCELERATE} STREQUAL "none")
else()
    find_package(message REQUIRED)
    if(message_FOUND)
        include_directories(SYSTEM ${message_INCLUDE_DIRS})
    endif()
    list(APPEND LINK_SEARCH_PATH ${message_LIB_DIRS})
endif()

if(MSVC)
  add_compile_options($<$<CXX_COMPILER_ID:MSVC>:/MP>)
  add_compile_options($<$<CXX_COMPILER_ID:MSVC>:-bigobj>)
  if (${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    add_compile_options(/O2)
    add_compile_options(/Ob1)
    string(REPLACE "RTC1" "" CMAKE_CXX_FLAGS_DEBUG ${CMAKE_CXX_FLAGS_DEBUG})
  endif ()
endif()

if (${CMAKE_BUILD_TYPE} STREQUAL "Debug")
  set(DEBUG_POSTFIX "d")
  set(CMAKE_DEBUG_POSTFIX ${DEBUG_POSTFIX})
endif ()

set(OUTPUT_ROOT ${PROJECT_SOURCE_DIR}/output)
if(NOT DEFINED MY_OUTPUT_ROOT)
  set(MY_OUTPUT_ROOT ${OUTPUT_ROOT}/fanya-protocol)
endif()

# as .so link succcess
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(PROTOC_PATH ${BIGREPO_PROTOC_PATH})
set(USE_BOLEIDLC true)

if(MSVC)
    set(PROTOC_CMD ${PROTOC_PATH}/protoc.exe)
elseif(APPLE)
    set(PROTOC_CMD ${PROTOC_PATH}/protoc-mac)
elseif(USE_BOLEIDLC)
    set(PROTOC_CMD ${PROTOC_PATH}/boleidlc)
else()
    set(PROTOC_CMD ${PROTOC_PATH}/protoc)
endif()

add_subdirectory(fanya-protocol)
