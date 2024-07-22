cmake_minimum_required(VERSION 2.8)

#message(STATUS ${CMAKE_CURRENT_SOURCE_DIR})
file(GLOB_RECURSE proto_files "${CMAKE_CURRENT_SOURCE_DIR}/raw/*.proto")
#message(STATUS ${proto_files})
set(target_files, "")
set(PROTO_RAW ${CMAKE_BINARY_DIR}/proto_raw/)
file(MAKE_DIRECTORY ${PROTO_RAW})
foreach(proto_file ${proto_files})
  #message(STATUS ${proto_file})
  STRING(REGEX REPLACE ".+/(.+)\\..*" "\\1" target_file ${proto_file})
  #message(STATUS ${target_file})
  #message(STATUS "${PROTO_RAW}${target_file}.proto")
  set(tmp ${PROTO_RAW}${target_file}.proto)
  message(STATUS "copy ${proto_file} to ${tmp}")
  configure_file(${proto_file} ${tmp} COPYONLY)
  list(APPEND target_files ${target_file})
endforeach()
#message(STATUS ${target_files})

if(NOT DEFINED python_cmd)
	set(python_cmd "python")
endif()
message(STATUS "python_cmd : ${python_cmd}")

set(pb_files, "")
set(dds_files, "")
if (${ACCELERATE} STREQUAL "none")
  #do nothing
elseif(${ACCELERATE} STREQUAL "pb")
  execute_process(
    COMMAND ${python_cmd} ${CMAKE_CURRENT_SOURCE_DIR}/../tools/download_boleidlc.py
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  )

  set(BOLEIDLC_CMD boleidlc)
  find_program(EXECUTABLE_FOUND ${BOLEIDLC_CMD})
  if(EXECUTABLE_FOUND)
    message("Found ${BOLEIDLC_CMD} in system PATH: ${EXECUTABLE_FOUND}")
  else()
    if(MSVC)
      set(BOLEIDLC_CMD ${CMAKE_CURRENT_SOURCE_DIR}/../build/tools_tmp/boleidlc.exe)
    elseif(APPLE)
      set(BOLEIDLC_CMD ${CMAKE_CURRENT_SOURCE_DIR}/../build/tools_tmp/boleidlc)
    else()
      set(BOLEIDLC_CMD ${CMAKE_CURRENT_SOURCE_DIR}/../build/tools_tmp/boleidlc)
    endif()
  endif()

  set(PB_RAW ${CMAKE_BINARY_DIR}/pb_raw/)
  file(MAKE_DIRECTORY ${PB_RAW})
  #拷贝proto文件到目标路径
  if(MSVC)
    execute_process(COMMAND cmd /c "${CMAKE_CURRENT_SOURCE_DIR}/../tools/copy_pb_list_win64.bat"
                    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/../tools/")
  else()
    execute_process(COMMAND sh ${CMAKE_CURRENT_SOURCE_DIR}/../tools/copy_pb_list.sh
                    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../tools/)
  endif()
  #用boleidlc生成bolemsg.h等文件，生成的目录就在${PB_RAW}/
  file(GLOB PROTO_NAMES "${PB_RAW}*.proto")
  foreach(proto_name ${PROTO_NAMES})
    STRING(REGEX REPLACE ".+/(.+)\\..*" "\\1" file_name ${proto_name})
    list(APPEND pb_files ${file_name})
    execute_process(COMMAND ${BOLEIDLC_CMD} "${file_name}.proto"
                    WORKING_DIRECTORY ${PB_RAW}
                    RESULT_VARIABLE RESULT_PRESENT
                    OUTPUT_VARIABLE _RESULT_VALUES
                    ERROR_VARIABLE _RESULT_ERROR_VALUE)
    message("[bolemsg] generate action only result:${proto_name}")
  endforeach()
elseif(${ACCELERATE} STREQUAL "cyclonedds")
  set(DDS_RAW ${CMAKE_BINARY_DIR}/dds_raw/)
  file(MAKE_DIRECTORY ${DDS_RAW})
  if(MSVC)
    execute_process(COMMAND cmd /c "${CMAKE_CURRENT_SOURCE_DIR}/../tools/copy_pb_list_win64.bat"
                    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/../tools/")
  else()
    execute_process(COMMAND sh ${CMAKE_CURRENT_SOURCE_DIR}/../tools/copy_dds_list.sh
                    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../tools/)
  endif()
  file(GLOB PROTO_NAMES "${DDS_RAW}*.proto")
  foreach(proto_name ${PROTO_NAMES})
    STRING(REGEX REPLACE ".+/(.+)\\..*" "\\1" file_name ${proto_name})
    list(APPEND dds_files ${file_name})
    execute_process(COMMAND ${BOLEIDLC_CMD} "${file_name}.proto"
                    WORKING_DIRECTORY ${DDS_RAW}
                    RESULT_VARIABLE RESULT_PRESENT
                    OUTPUT_VARIABLE _RESULT_VALUES
                    ERROR_VARIABLE _RESULT_ERROR_VALUE)
    message("[bolemsg] generate action only result:${proto_name}")
  endforeach()
elseif(${ACCELERATE} STREQUAL "fastdds")
  set(DDS_RAW ${CMAKE_BINARY_DIR}/dds_raw/)
  file(MAKE_DIRECTORY ${DDS_RAW})
  if(MSVC)
    execute_process(COMMAND cmd /c "${CMAKE_CURRENT_SOURCE_DIR}/../tools/copy_pb_list_win64.bat"
                    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/../tools/")
  else()
    execute_process(COMMAND sh ${CMAKE_CURRENT_SOURCE_DIR}/../tools/copy_dds_list.sh
                    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../tools/)
  endif()
  file(GLOB PROTO_NAMES "${DDS_RAW}*.proto")
  foreach(proto_name ${PROTO_NAMES})
    STRING(REGEX REPLACE ".+/(.+)\\..*" "\\1" file_name ${proto_name})
    list(APPEND dds_files ${file_name})
    execute_process(COMMAND ${BOLEIDLC_CMD} "${file_name}.proto"
                    WORKING_DIRECTORY ${DDS_RAW}
                    RESULT_VARIABLE RESULT_PRESENT
                    OUTPUT_VARIABLE _RESULT_VALUES
                    ERROR_VARIABLE _RESULT_ERROR_VALUE)
    message("[bolemsg] generate action only result:${proto_name}")
  endforeach()
endif ()

#use last .proto file name
set(protocol_header_files "")
set(wrapper_header_files "")
set(protocol_src_files "")

foreach(file ${target_files})
  if(${toolchain} MATCHES "^python")
  else()
    if(USE_BOLEIDLC)
      list(APPEND wrapper_header_files ${CMAKE_CURRENT_SOURCE_DIR}/src/${file}_idl_wrapper.h)
    endif()
    list(APPEND protocol_header_files ${CMAKE_CURRENT_SOURCE_DIR}/src/${file}.pb.h)
    list(APPEND protocol_src_files ${CMAKE_CURRENT_SOURCE_DIR}/src/${file}.pb.cc)
    if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/src/${file}.pb.cc")
    else()
      file(WRITE "${CMAKE_CURRENT_SOURCE_DIR}/src/${file}.pb.h" "")
      file(WRITE "${CMAKE_CURRENT_SOURCE_DIR}/src/${file}.pb.cc" "")
    endif()
  endif()
endforeach()

if (${ACCELERATE} STREQUAL "pb")
  foreach(file ${pb_files})
    if(${toolchain} MATCHES "^python")
    else()
      configure_file(${PB_RAW}${file}.pb.h ${CMAKE_CURRENT_SOURCE_DIR}/src/${file}.pb.h COPYONLY)
      configure_file(${PB_RAW}${file}.boletype.h ${CMAKE_CURRENT_SOURCE_DIR}/src/${file}.boletype.h COPYONLY)
      configure_file(${PB_RAW}${file}.bolemsg.h ${CMAKE_CURRENT_SOURCE_DIR}/src/${file}.bolemsg.h COPYONLY)
      configure_file(${PB_RAW}${file}.pb.cc ${CMAKE_CURRENT_SOURCE_DIR}/src/${file}.pb.cc COPYONLY)
      configure_file(${PB_RAW}${file}.boletype.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/${file}.boletype.cpp COPYONLY)
      configure_file(${PB_RAW}${file}.bolemsg.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/${file}.bolemsg.cpp COPYONLY)
      list(APPEND protocol_header_files ${CMAKE_CURRENT_SOURCE_DIR}/src/${file}.pb.h)
      list(APPEND protocol_header_files ${CMAKE_CURRENT_SOURCE_DIR}/src/${file}.boletype.h)
      list(APPEND protocol_header_files ${CMAKE_CURRENT_SOURCE_DIR}/src/${file}.bolemsg.h)
      list(APPEND protocol_src_files ${CMAKE_CURRENT_SOURCE_DIR}/src/${file}.pb.cc)
      list(APPEND protocol_src_files ${CMAKE_CURRENT_SOURCE_DIR}/src/${file}.boletype.cpp)
      list(APPEND protocol_src_files ${CMAKE_CURRENT_SOURCE_DIR}/src/${file}.bolemsg.cpp)
      if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/src/${file}.bolemsg.cpp")
      else()
        message(STATUS "error ---------------")
      endif()
    endif()
  endforeach()
elseif(${ACCELERATE} STREQUAL "cyclonedds" OR ${ACCELERATE} STREQUAL "fastdds")
  foreach(file ${dds_files})
  if(${toolchain} MATCHES "^python")
  else()
    configure_file(${DDS_RAW}${file}.pb.h ${CMAKE_CURRENT_SOURCE_DIR}/src/${file}.pb.h COPYONLY)
    configure_file(${DDS_RAW}${file}.boletype.h ${CMAKE_CURRENT_SOURCE_DIR}/src/${file}.boletype.h COPYONLY)
    configure_file(${DDS_RAW}${file}.bolemsg.h ${CMAKE_CURRENT_SOURCE_DIR}/src/${file}.bolemsg.h COPYONLY)
    configure_file(${DDS_RAW}${file}.pb.cc ${CMAKE_CURRENT_SOURCE_DIR}/src/${file}.pb.cc COPYONLY)
    configure_file(${DDS_RAW}${file}.boletype.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/${file}.boletype.cpp COPYONLY)
    configure_file(${DDS_RAW}${file}.bolemsg.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/${file}.bolemsg.cpp COPYONLY)
    list(APPEND protocol_header_files ${CMAKE_CURRENT_SOURCE_DIR}/src/${file}.pb.h)
    list(APPEND protocol_header_files ${CMAKE_CURRENT_SOURCE_DIR}/src/${file}.boletype.h)
    list(APPEND protocol_header_files ${CMAKE_CURRENT_SOURCE_DIR}/src/${file}.bolemsg.h)
    list(APPEND protocol_src_files ${CMAKE_CURRENT_SOURCE_DIR}/src/${file}.pb.cc)
    list(APPEND protocol_src_files ${CMAKE_CURRENT_SOURCE_DIR}/src/${file}.boletype.cpp)
    list(APPEND protocol_src_files ${CMAKE_CURRENT_SOURCE_DIR}/src/${file}.bolemsg.cpp)
    if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/src/${file}.bolemsg.cpp")
    else()
      message(STATUS "error ---------------")
    endif()
  endif()
  endforeach()
endif ()

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

SET(FANYA_LIB fanya_protocol)
add_library(
  ${FANYA_LIB} SHARED ${protocol_src_files} ${protocol_header_files} ${wrapper_header_files})

# build protocol
add_custom_target(build-auto-protocol)
add_dependencies(${FANYA_LIB} build-auto-protocol)

set(PROTO_OUT "${CMAKE_CURRENT_SOURCE_DIR}/src/")
message(STATUS ${PROTOC_PATH})

set(build_protocol_cmd ${python_cmd})
set(build_protocol_script ${BIGREPO_ROOT}/tools/build-protocol.py)
if(NOT EXISTS ${build_protocol_script})
    set(build_protocol_script ${CMAKE_SOURCE_DIR}/tools/build-protocol.py)
    set(PROTOC_CMD ${CMAKE_SOURCE_DIR}/tools/protoc${PROTOC_CMD})
endif()
message(STATUS "build_protocol_cmd: ${build_protocol_cmd}")
message(STATUS "build_protocol_script: ${build_protocol_script}")
message(STATUS "PROTOC_CMD: ${PROTOC_CMD}")

##################################################

add_custom_command(
		TARGET     build-auto-protocol
		PRE_BUILD
		COMMAND    "${build_protocol_cmd}"
		ARGS	${build_protocol_script} 
                ${PROTOC_CMD} 
                ${PROTO_RAW} 
                ${PROTO_OUT}
		)

##################################################
# install configuration
set(output_dirs ${MY_OUTPUT_ROOT})
if(${USE_WS_LIB})
	list(APPEND output_dirs ${BIGREPO_OUTPUT})
endif()
foreach( dir ${output_dirs})
  install(TARGETS ${FANYA_LIB} DESTINATION ${dir}/lib)
  if(USE_BOLEIDLC)
    foreach( wrapper_file ${wrapper_header_files})
      install(FILES ${wrapper_file}
      DESTINATION ${dir}/include/fanya_protocol/)
    endforeach()
  endif()
  foreach( protocol_file ${protocol_header_files})
    install(FILES ${protocol_file}
    DESTINATION ${dir}/include/fanya_protocol/)
  endforeach()

  foreach( protocol_file ${protocol_src_files})
    install(FILES ${protocol_file}
    DESTINATION ${dir}/src/fanya_protocol/)
  endforeach()
endforeach()
