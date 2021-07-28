cmake_minimum_required(VERSION 2.8)
project(seeta LANGUAGES C CXX )
add_library(seeta ./src/seeta.cxx ./src/seeta.h)
SET(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR}/install)
target_include_directories(seeta PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
    $<INSTALL_INTERFACE:include>)
set_target_properties(seeta PROPERTIES PUBLIC_HEADER "src/seeta.h")
install(TARGETS seeta
    EXPORT seeta-targets
    PUBLIC_HEADER DESTINATION include
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin)
install(EXPORT seeta-targets
    NAMESPACE seeta::
    FILE seeta-config.cmake
    DESTINATION lib/cmake/seeta)
