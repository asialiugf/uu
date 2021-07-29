# 可以设置ctp_INCLUDE_DIR这个变量进行路径的提示
message(" uquant/cmake ---------: ${CMAKE_CURRENT_SOURCE_DIR}")
message(" uquant/cmake ---psd------: ${PROJECT_SOURCE_DIR}")

find_path(
  ctp_INCLUDE_DIR
  #NAMES ThostFtdcMdApi.h
  NAMES TesT.h
  PATHS ${PROJECT_SOURCE_DIR}/extern/ctp
  #PATHS /home/rabbit/uu/uquant/extern/ctp
)

#set(ctp_INCLUDE_DIR "/home/rabbit/uu/uquant/extern/ctp")
# 可以设置ctp_LIB_DIR这个变量进行路径的提示
find_library(
  mduser_LIBRARY
  NAMES thostmduserapi_se
  PATHS ${PROJECT_SOURCE_DIR}/extern/ctp
  #PATHS /home/rabbit/uu/uquant/extern/ctp
)
find_library(
  trader_LIBRARY
  NAMES thosttraderapi_se
  PATHS ${PROJECT_SOURCE_DIR}/extern/ctp
  #PATHS /home/rabbit/uu/uquant/extern/ctp
)

message("ctp --------- ${ctp_INCLUDE_DIR}")
message("ctp --------- ${mduser_LIBRARY}")
message("ctp --------- ${trader_LIBRARY}")

set(ctp_INCLUDE_DIRS ${ctp_INCLUDE_DIR})
set(ctp_LIBRARYS ${ctp_LIBRARY})
