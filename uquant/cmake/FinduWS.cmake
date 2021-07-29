# 可以设置uWS_INCLUDE_DIR这个变量进行路径的提示
find_path(
  uWS_INCLUDE_DIR
  NAMES uWS.h
  PATHS ${PROJECT_SOURCE_DIR}/extern/uws/install/include/uWS
)
# 可以设置uWS_LIB_DIR这个变量进行路径的提示
find_library(
  uWS_LIBRARY
  NAMES uWS
  PATHS ${PROJECT_SOURCE_DIR}/extern/uws/install/lib64
  #PATHS /home/rabbit/uu/uquant/extern/uws/install/lib64
)


set(uWS_INCLUDE_DIRS ${uWS_INCLUDE_DIR})
set(uWS_LIBRARYS ${uWS_LIBRARY})
