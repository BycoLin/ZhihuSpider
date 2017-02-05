#ifndef _PULL_OP_H
#define _PULL_OP_H

#include <string>
#include <iostream>
using namespace std;

int pullURL(const string& srcURL,const string& dstFile);
int getLacation(const string& sDownloadBaseUrl,string& sGetDownloadUrl);
int getCurlHtmlInfo(const string& sDownloadBaseUrl,string& sGetDownloadInfo);
int getCurlHtmlInfo2(const string& sDownloadBaseUrl,string& sGetDownloadInfo);
int getCurlCookies(const string &sBaseUrl,const string &sPathName,const string &sData);

#endif