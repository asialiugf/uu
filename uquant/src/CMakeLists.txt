set (CONFIG_VERSION ${CMAKE_CURRENT_BINARY_DIR}/Common/config_version.h)
#configure_file (Common/config.h.in ${CONFIG_COMMON})
add_executable(Dataserver "")

add_headers_and_sources(data ./)

target_sources(Dataserver
  PRIVATE
#    ${CMAKE_CURRENT_LIST_DIR}/main.cpp
    ${data_sources}
  )

target_link_libraries(Dataserver
  PRIVATE
    cJSON
    uws
  )

#message( "-------------------------: ${CMAKE_CURRENT_LIST_DIR}" )

target_include_directories(Dataserver PUBLIC
                           ${CMAKE_CURRENT_LIST_DIR}
                           #${PROJECT_BINARY_DIR}
                           #${EXTRA1_INCLUDES}
                          )
