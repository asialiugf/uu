### cjson库
https://github.com/DaveGamble/cJSON/releases
#### 编译安装
root 用户·
```c
wget https://github.com/DaveGamble/cJSON/archive/v1.7.0.tar.gz
tar xvf v1.7.0.tar.gz
root@:~/source# cd cJSON-1.7.0/
root@:~/source/cJSON-1.7.0# mkdir build
root@:~/source/cJSON-1.7.0# cd build
root@:~/source/cJSON-1.7.0/build# cmake ..
root@:~/source/cJSON-1.7.0/build# make
root@:~/source/cJSON-1.7.0/build# make install
```
请仔细阅读 https://github.com/DaveGamble/cJSON


### uWebsockets库
https://github.com/uNetworking/uWebSockets/releases
$ cd ./uquant/uws
$ make
root@VM-0-12-ubuntu:/usr/local/include/uWS# cp /home/rabbit/uquant/uws/src/*.h ./
### c-ares
https://github.com/c-ares/c-ares/releases
c-ares-1.17.1.tar.gz

cd c-ares-1.17.1
make
make install

### protobuf

https://github.com/protocolbuffers/protobuf/releases/tag/v3.17.3
wget https://github.com/protocolbuffers/protobuf/releases/download/v3.17.3/protobuf-all-3.17.3.tar.gz

root用户：

tar xvf protobuf-all-3.17.3.tar.gz
cd  protobuf-all-3.17.3

./autogen.sh
./configure
make
make check
sudo make install
sudo ldconfig # refresh shared library cache.


### CTP的目录

  把CTP相关的文件移到下面两个目录里面去。
  /usr/local/include
  /usr/local/lib

```c
root@d:/usr/local/ctp# ll
total 6400
-rwxrwxr-x 1 riddle riddle     184 Dec 12 17:02 error.dtd
-rwxrwxr-x 1 riddle riddle   15366 Dec 12 17:02 error.xml
-rwxrwxr-x 1 riddle riddle    5398 Dec 12 17:02 ThostFtdcMdApi.h
-rwxrwxr-x 1 riddle riddle   31045 Dec 12 17:02 ThostFtdcTraderApi.h
-rwxrwxr-x 1 riddle riddle  238859 Dec 12 17:02 ThostFtdcUserApiDataType.h
-rwxrwxr-x 1 riddle riddle  201803 Dec 12 17:02 ThostFtdcUserApiStruct.h
-rwxrwxr-x 1 riddle riddle 1900961 Dec 12 17:02 thostmduserapi.so
-rwxrwxr-x 1 riddle riddle 4140081 Dec 12 17:02 thosttraderapi.so
root@d:/usr/local/ctp# 
root@d:/usr/local/ctp# 
root@d:/usr/local/ctp# 
root@d:/usr/local/ctp# 
root@d:/usr/local/ctp# 
root@d:/usr/local/ctp# 
root@d:/usr/local/ctp# cp *.so ../lib
root@d:/usr/local/ctp# cp *.h ../include
root@d:/usr/local/ctp# 
```
```
/extern/cJSON-1.7.0/build$ cmake .. -DCMAKE_INSTALL_PREFIX=../install
/extern/uws$ make PREFIX=./install install
```
