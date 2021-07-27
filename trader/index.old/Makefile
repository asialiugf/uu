CPP_SHARED := -std=c++11 -O3 -shared -fPIC -I ./ -I ../inc -I /usr/include -I /usr/local/include -I ../lib/seetalib-1.0.0/include \
-I /usr/include/postgresql \
Indicator.cpp

default: lib

lib:
	make `(uname -s)`
Linux:
	$(CXX) $(CPP_SHARED) -s -o ../../u/lib/libindicator.so

CC = g++
CPPFLAGS =  -std=c++11 -O3
CFLAGS =  -std=c++11 -O3 -fPIC  -luWS -lssl -lz -lpthread -lcjson -Wunused-result
LIBPATH = -L/home/riddle/uws -L ./ -L ../../u/lib
LINK =  $(CC)

ALLLIBS = -luBEE -luWS -lssl -lz -lpthread -lcjson -lpq

CTPLIBS:= ../ctp/thostmduserapi.so \
        ../ctp/thosttraderapi.so

EXE = ../../u/exe/

SRC = ../src/
INC = ../inc/
OBJ = ../../u/obj/

INCS = -I ../inc \
       -I /usr/include \
       -I /usr/local/include \
	   -I /usr/include/postgresql


DATASERVER:= \
	$(OBJ)Dataserver.o 

TQDATA:= \
	$(OBJ)Tqdata.o 

FUGEN:= \
    $(OBJ)FuGen.o

WEBSERVER:= \
	$(OBJ)Webserver.o 

