/*
 * Copyright (C) AsiaLine
 * Copyright (C) kkk, Inc.
 */

#ifndef UBEE_FILE_H
#define UBEE_FILE_H

#include <uBEE.h>

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <iostream>
#include <fstream>
#include <string>

namespace uBEE
{
using namespace std;
int SaveLine(const char *File, const char *Linebuf) ;
//int UpdateLine(const char *FileName, char *pc_line) ;
int SaveBin(const char *File, const char * buf, size_t len);
int CountLines(const char *File);
string ReadLine(const char *File,int line,int lines);
//int SaveTick(const char *pca_buf, int len);
//int DispTick(const char *buf) ;

} //namespace

#endif // end UBEE_FILE_H
