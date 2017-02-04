#ifndef _TIME_OP_H
#define _TIME_OP_H

#include <string>
#include "stdio.h"
using namespace std;

int getNowDay(string& sNowTime);
int getNowTm(string& sNowTime);
int getNowTm2(string& sNowTime);
int getNowTmYMD(string& sNowTime);

string timeToDate(const time_t timer);
string timeToDateTm(const time_t timer);

int offset_day(time_t &t,int iType,int iOffset);
int getOffsetTm(string& sNowTime,int iType,int iOffset);
int getOffsettmYMD(string& sNowTime,int iType,int iOffset);

#endif