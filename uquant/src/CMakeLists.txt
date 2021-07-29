set (CONFIG_VERSION ${CMAKE_CURRENT_BINARY_DIR}/Common/config_version.h)
#configure_file (Common/config.h.in ${CONFIG_COMMON})

message(" src ---------: ${CMAKE_CURRENT_SOURCE_DIR}")
message(" uquant/src ---psd------: ${PROJECT_SOURCE_DIR}")
add_executable(Dataserver "")
add_headers_and_sources(data ./)
target_sources(Dataserver
  PRIVATE
    ${data_sources}
  )

target_link_directories(Dataserver PUBLIC /home/rabbit/uu/uquant/extern/ctp)

target_link_libraries(Dataserver
  PUBLIC
    mduser 
    trader
    cjson
    uWS
    -luuid -lssl -pthread -lz
  )

target_include_directories(Dataserver PUBLIC
                           ${CMAKE_CURRENT_LIST_DIR}
                           #${PROJECT_BINARY_DIR}
                           #${EXTRA1_INCLUDES}
                          )

