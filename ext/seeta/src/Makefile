#MAKEFILE************  

CC = g++
CPPFLAGS =  -std=c++11  -O3
CCFLAGS =  -std=c++11 -O3 -fPIC 
   
#CC=cc
#CCFLAGS =  -pipe  -O -W -Wall -Wpointer-arith -Wno-unused-parameter -Werror -g -pthread
   
LDFLAGS=-fPIC -shared  

build:all
   
all: libsee_talib.a \
	libsee_talib.so
   
libsee_talib.a: see_talib.o
	ar r ../lib/libsee_talib.a ../lib/see_talib.o  

libsee_talib.so: see_talib.cpp
	$(CXX) $(CCFLAGS) -o ../lib/libsee_talib.so -shared -fPIC -I ../include see_talib.cpp

see_talib.o: see_talib.cpp
	$(CXX) -c $(CCFLAGS) -o ../lib/see_talib.o -I ../include see_talib.cpp
