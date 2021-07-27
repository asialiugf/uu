/*
 * Copyright (C) AsiaLine
 * Copyright (C) kkk, Inc.
 */

#ifndef UBEE_PSQLFUNC_H
#define UBEE_PSQLFUNC_H

#include <iostream>
#include <thread>
#include <fstream>
#include <sstream>
#include "Psqlpool.h"

namespace uBEE
{

void createBarTable(std::shared_ptr<uBEE::DBPool> dbpool,const char * future, const char * period);
void createTickTable(std::shared_ptr<uBEE::DBPool> dbpool,const char * future);

} //end namesapce

#endif //UBEE_PSQLFUNC_H
