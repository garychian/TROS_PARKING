cmake_minimum_required(VERSION 3.15)
set(CMAKE_OSX_DEPLOYMENT_TARGET "11.0" CACHE STRING "Minimum OS X deployment version" FORCE)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(fanya_parking LANGUAGES CXX)

set(DEBUG_POSTFIX "d")

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

find_program(CCACHE_PROGRAM ccache)
if(CCACHE_PROGRAM)
  set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${CCACHE_PROGRAM}")
endif()

SET(CMAKE_BUILD_TYPE ${build_type})

# -fPIC, platform independent
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

if(MSVC)
    set(PROTO debug libprotobufd libprotobuf)
elseif(APPLE)
    set(PROTO protobuf)
elseif(UNIX)
    set(PROTO protobuf)
else()
    set(PROTO protobuf)
endif()

if(MSVC)
  add_compile_options(/MP)
  add_compile_options(-bigobj)
  set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CMAKE_CXX_FLAGS_DEBUG} /MDd")
  set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CMAKE_CXX_FLAGS_RELEASE} /MD")
  set(CMAKE_C_FLAGS_DEBUG "$ENV{CMAKE_C_FLAGS_DEBUG} /MDd")
  set(CMAKE_C_FLAGS_RELEASE "$ENV{CMAKE_C_FLAGS_RELEASE} /MD")
else()
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -pthread")
  set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CMAKE_CXX_FLAGS_DEBUG} -Wall -Werror -Wextra -Wno-unused-parameter -O0 -g3 -ggdb ")
  set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CMAKE_CXX_FLAGS_RELEASE} -Wall -Werror -Wextra -Wno-unused-parameter -O3  ")
  set(CMAKE_C_FLAGS_DEBUG "$ENV{CMAKE_C_FLAGS_DEBUG} -Wall -Werror -Wextra -O0 -Wno-unused-parameter -g3 -ggdb ")
  set(CMAKE_C_FLAGS_RELEASE "$ENV{CMAKE_C_FLAGS_RELEASE} -Wall -Werror -Wextra -Wno-unused-parameter -O3  ")
endif()

if(UNIX)
  set(DVB_SYSTEM hbmem ion alog)
endif()

# RPATH : https://gitlab.kitware.com/cmake/community/-/wikis/doc/cmake/RPATH-handling
set(CMAKE_SKIP_BUILD_RPATH FALSE)
set(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)
set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

list(FIND CMAKE_PLATFORM_IMPLICIT_LINK_DIRECTORIES "${CMAKE_INSTALL_PREFIX}/lib" isSystemDir)

if("${isSystemDir}" STREQUAL "-1")
  set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
endif("${isSystemDir}" STREQUAL "-1")

# install directory
set(TROS_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/output/parking)
set(TROS_INSTALL_BIN ${CMAKE_SOURCE_DIR}/output/parking/bin)
set(TROS_INSTALL_LIB ${CMAKE_SOURCE_DIR}/output/parking/lib)

add_subdirectory(src)

if(EXISTS ${CMAKE_SOURCE_DIR}/example/CMakeLists.txt)
  add_subdirectory(example)
endif()

if(EXISTS ${CMAKE_SOURCE_DIR}/test/CMakeLists.txt)
  add_subdirectory(test)
endif()
