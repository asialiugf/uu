set (CONFIG_VERSION ${CMAKE_CURRENT_BINARY_DIR}/Common/config_version.h)
#configure_file (Common/config.h.in ${CONFIG_COMMON})
add_executable(Dataserver "")

add_headers_and_sources(data ./)

target_sources(Dataserver
  PRIVATE
#    ${CMAKE_CURRENT_LIST_DIR}/main.cpp
    ${data_sources}
  )

#include_directories(/home/rabbit/uu/uquant/extern/ctp)
target_link_directories(Dataserver PUBLIC /home/rabbit/uu/uquant/extern/ctp)

target_link_libraries(Dataserver
  PUBLIC
    #thostmduserapi_se
    mduser 
    trader
    #thosttraderapi_se
    cjson
    uWS
    -luuid -lssl -pthread -lz
  )

#message( "-------------------------: ${CMAKE_CURRENT_LIST_DIR}" )

target_include_directories(Dataserver PUBLIC
                           ${CMAKE_CURRENT_LIST_DIR}
                           #${PROJECT_BINARY_DIR}
                           #${EXTRA1_INCLUDES}
                          )
