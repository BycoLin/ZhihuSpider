#include "time_op.h"

int getNowDay(string& sNowTime)
{
	time_t t; 
	struct tm * a;
	char buf[20];
	
	time(&t); 
	a=localtime(&t); 
	
	sprintf(buf,"%4d%02d%02d",1900+a->tm_year,1+a->tm_mon,a->tm_mday); 
	
	sNowTime = buf;
	
	return 0;
}

int getNowTm(string& sNowTime)
{
	time_t t; 
	struct tm * a;
	char buf[20];
	
	time(&t); 
	a=localtime(&t); 
	
	sprintf(buf,"%4d%02d%02d_%02d%02d%02d",1900+a->tm_year,1+a->tm_mon,a->tm_mday,a->tm_hour,a->tm_min,a->tm_sec); 
	
	sNowTime = buf;
	
	return 0;
}

int getNowTm2(string& sNowTime)
{
	time_t t; 
	struct tm * a;
	char buf[30];
	
	time(&t); 
	a=localtime(&t); 
	
	sprintf(buf,"%4d-%02d-%02d_%02d:%02d:%02d",1900+a->tm_year,1+a->tm_mon,a->tm_mday,a->tm_hour,a->tm_min,a->tm_sec); 
	
	sNowTime = buf;
	
	return 0;
}

int getNowTmYMD(string& sNowTime)
{
	time_t t; 
	struct tm * a;
	char buf[30];
	
	time(&t); 
	a=localtime(&t); 
	
	snprintf(buf,sizeof(buf),"%04d年%02d月%02d日",a->tm_year+1900,a->tm_mon+1,a->tm_mday);
	
	sNowTime = buf;
	
	return 0;
}

string timeToDate(const time_t timer)
{
	struct tm *ltm=localtime(&timer);

	char buf[30];
	snprintf(buf,sizeof(buf),"%04d年%02d月%02d日",ltm->tm_year+1900,ltm->tm_mon+1,ltm->tm_mday);

	string s(buf);
	return s;
}

string timeToDateTm(const time_t timer)
{
	struct tm *ltm=localtime(&timer);

	char buf[25];
	snprintf(buf,sizeof(buf),"%04d-%02d-%02d %02d:%02d",ltm->tm_year+1900,ltm->tm_mon+1,ltm->tm_mday,ltm->tm_hour,ltm->tm_min);

	string s(buf);
	return s;
}

int offset_day(time_t &t,int iType,int iOffset)
{
	if(iType == 0)
		t = time(NULL) + iOffset*24*3600;
	else
		t = time(NULL) - iOffset*24*3600;
	
	return 0;
}

int getOffsetTm(string& sNowTime,int iType,int iOffset)
{
	time_t t; 
	struct tm * a;
	char buf[30];
	
	offset_day(t,iType,iOffset);
	
	a=localtime(&t); 
	
	sprintf(buf,"%4d-%02d-%02d_%02d:%02d:%02d",1900+a->tm_year,1+a->tm_mon,a->tm_mday,a->tm_hour,a->tm_min,a->tm_sec); 
	
	sNowTime = buf;
	
	return 0;
}

int getOffsettmYMD(string& sNowTime,int iType,int iOffset)
{
	time_t t; 
	struct tm * a;
	char buf[30];
	
	offset_day(t,iType,iOffset);

	a=localtime(&t); 
	
	snprintf(buf,sizeof(buf),"%04d年%02d月%02d日",a->tm_year+1900,a->tm_mon+1,a->tm_mday);
	
	sNowTime = buf;
	
	return 0;
}
