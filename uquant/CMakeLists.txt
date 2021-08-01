cmake_minimum_required(VERSION 3.12.0  FATAL_ERROR)

set(LINUX ON)
set(WIN32 OFF)
set(CMAKE_VERBOSE_MAKEFILE ON)

if(LINUX)
#if(NOT WIN32)
  string(ASCII 27 Esc)
  set(ColourReset "${Esc}[m")
  set(ColourBold  "${Esc}[1m")
  set(Red         "${Esc}[31m")
  set(Green       "${Esc}[32m")
  set(Yellow      "${Esc}[33m")
  set(Blue        "${Esc}[34m")
  set(Magenta     "${Esc}[35m")
  set(Cyan        "${Esc}[36m")
  set(White       "${Esc}[37m")
  set(BoldRed     "${Esc}[1;31m")
  set(BoldGreen   "${Esc}[1;32m")
  set(BoldYellow  "${Esc}[1;33m")
  set(BoldBlue    "${Esc}[1;34m")
  set(BoldMagenta "${Esc}[1;35m")
  set(BoldCyan    "${Esc}[1;36m")
  set(BoldWhite   "${Esc}[1;37m")
endif()

foreach(policy
        CMP0023
        CMP0048 # CMake 3.0
        CMP0074 # CMake 3.12
        CMP0077
        CMP0079
    )
    if(POLICY ${policy})
        cmake_policy(SET ${policy} NEW)
    endif()
endforeach()

project(UQUANT VERSION 1.0.0 LANGUAGES C CXX)

# globel setting
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
# or :
#set_target_properties(myTarget
#    PROPERTIES
#        CXX_STANDARD 11
#        CXX_STANDARD_REQUIRED YES
#        CXX_EXTENSIONS NO
#)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib) # 静态库
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib) # 动态库
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin) # 可执行文件

include(./cmake/glob_sources.cmake)

#macro(add_headers_and_sources prefix common_path)
#    add_glob(${prefix}_headers ${CMAKE_CURRENT_SOURCE_DIR} ${common_path}/*.h)
#    add_glob(${prefix}_sources ${common_path}/*.cpp ${common_path}/*.c ${common_path}/*.h)
#endmacro()
#
#macro(add_headers_only prefix common_path)
#    add_glob(${prefix}_headers ${CMAKE_CURRENT_SOURCE_DIR} ${common_path}/*.h)
#endmacro()

message("${Green} -------- Uquant  -----------  ${ColourReset}")


set (CONFIG_VERSION ${CMAKE_CURRENT_BINARY_DIR}/Common/config_version.h)
configure_file (src/Common/Version.h.in Version.h)


#add_subdirectory (extern EXCLUDE_FROM_ALL)
#add_subdirectory (base)
add_subdirectory (src)
add_subdirectory (tests)
#add_subdirectory (utils)

include(./cmake/cJSON.cmake)
include(./cmake/FinduWS.cmake)
include(./cmake/Findctp.cmake)

message("ccccccccccccccccc=  ${cJSON_DIR}")
message("------${CJSON_INCLUDE_DIR}")
message("cJSON LIB------${CJSON_LIBARY}")

message(" uws:------ ${uWS_INCLUDE_DIR}")
message(" uws:------ ${uWS_LIBRARY}")

message(" ctp:------ ${ctp_INCLUDE_DIR}")
message(" ctp:------ ${mduser_LIBRARY}")
message(" ctp:------ ${trader_LIBRARY}")

