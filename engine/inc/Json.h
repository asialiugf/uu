/*
 * Copyright (C) AsiaLine
 * Copyright (C) kkk, Inc.
 */

#ifndef UBEE_JSON_H
#define UBEE_JSON_H

#include <uBEE.h>

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <vector>

namespace uBEE
{
int Tqjson(const char*message,int tt);
int MkRequest(const char * filename, char *cmd,size_t cmd_len);
std::vector<std::string> TicksCmd(const char *filename);
std::vector<std::string> KlinesCmd(const char *filename);
}
#endif // end UBEE_JSON_H
