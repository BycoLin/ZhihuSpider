#include "pull_op.h"
#include <dirent.h>
#include <fstream>
#include <string.h>

int pullURL(const string& srcURL,const string& dstFile)
{
	FILE *fstream = NULL;  
    char buff[1000];  
    memset (buff ,'\0', sizeof(buff));  
	ofstream in;
	bool bCreate = false;
	
	//--connect-timeout <seconds> 设置最大请求时间
	//-y/--speed-time 放弃限速所要的时间。默认为30
	//-Y/--speed-limit 停止传输速度的限制，速度时间'秒
	//-m/--max-time <seconds> 设置最大传输时间
   
	//string cmd = "curl -o " + dstFile + " "+ srcURL +" "+"--limit-rate 50k --speed-time 10 --speed-limit 3 --connect-timeout 30 -m 20";   
	string cmd = "curl -o " + dstFile + " "+ srcURL +" "+"--limit-rate 50k --speed-time 10 --speed-limit 3 --connect-timeout 300 -m 200";  
	
	cout<<cmd<<endl;
	
    if(NULL == (fstream = popen(cmd.c_str(), "r"))) {  
		cout<<"popen error:"<<cmd<<endl;
        return -1;  
    }  

	while (fgets(buff, sizeof(buff), fstream) != NULL)
	{
		in.open(dstFile.c_str(),ios::trunc);
		if(in)
		{
			in<<buff;
			bCreate = true;
		}
    }
	
	pclose(fstream);
	
	if(bCreate)
		in.close();
	
	return 0; 
}

int getLacation(const string& sDownloadBaseUrl,string& sGetDownloadUrl)
{
	FILE *fstream = NULL;  
	char buff[1000];  
	memset (buff ,'\0', sizeof(buff));  
	string sLocation = "";
	string sTempLocation = "Location: ";
	int iLocaLen = sTempLocation.length();
	string::size_type pos(0);  

	string cmd = "curl -s -I " + sDownloadBaseUrl +" --limit-rate 50k --speed-time 10 --speed-limit 3 --connect-timeout 30 -m 20 "+"|grep Location";   

	if(NULL == (fstream = popen(cmd.c_str(), "r")))
	{  
		cout<<"popen error:"<<cmd<<endl;
		return -1;  
	}  

	while (fgets(buff, sizeof(buff), fstream) != NULL)
	{
		if (buff[strlen(buff) - 1] == '\n')
		{
			buff[strlen(buff) - 1] = '\0'; //去除换行符
		}

		buff[strlen(buff) - 1] = '\0';
		
		sLocation = buff;
	}
	
	if(!sLocation.empty())
	{
		if((pos=sLocation.find(sTempLocation)) != string::npos)
		{
			if(pos+iLocaLen <= sLocation.length())
			{
				sLocation = sLocation.substr(pos+iLocaLen,sLocation.length());
				cout<<"Location:"<<sLocation<<endl;
				sGetDownloadUrl = sLocation;
			}
		}
	}
	else
	{
		sGetDownloadUrl = "";
	}
	
	pclose(fstream);
	
	return 0;
}

int getCurlHtmlInfo(const string& sDownloadBaseUrl,string& sGetDownloadInfo)
{
	FILE *fstream = NULL;  
	char buff[100000];  
	memset (buff ,'\0', sizeof(buff));  

	string cmd = "curl " + sDownloadBaseUrl +" "+"--limit-rate 50k --speed-time 10 --speed-limit 3 --connect-timeout 30 -m 20";

	if(NULL == (fstream = popen(cmd.c_str(), "r")))
	{  
		cout<<"popen error:"<<cmd<<endl;
		return -1;  
	}  
  
	while (fgets(buff, sizeof(buff), fstream) != NULL)
	{
		if (buff[strlen(buff) - 1] == '\n')
		{
			buff[strlen(buff) - 1] = '\0'; //去除换行符
		}

		buff[strlen(buff) - 1] = '\0';
		
		sGetDownloadInfo = sGetDownloadInfo+buff;
	}
	
	pclose(fstream);
	return 0;
}

int getCurlHtmlInfo2(const string& sDownloadBaseUrl,string& sGetDownloadInfo)
{
	FILE *fstream = NULL;  
	char buff[100000];  
	memset (buff ,'\0', sizeof(buff));  

	string cmd = "curl " + sDownloadBaseUrl +" "+"--limit-rate 50k --speed-time 10 --speed-limit 3 --connect-timeout 30 -m 20";

	if(NULL == (fstream = popen(cmd.c_str(), "r")))
	{  
		cout<<"popen error:"<<cmd<<endl;
		return -1;  
	}  
  
	while (fgets(buff, sizeof(buff), fstream) != NULL)
	{
		buff[strlen(buff) - 1] = '\0';
		
		sGetDownloadInfo = sGetDownloadInfo+buff;
	}
	
	pclose(fstream);
	return 0;
}

int getCurlCookies(const string &sBaseUrl,const string &sPathName,const string &sData)
{
	FILE *fstream = NULL;  
	char buff[100000];  
	memset (buff ,'\0', sizeof(buff));  
	
	string cmd = "curl " + sBaseUrl +" -c "+ sPathName+" --data "+sData+" --speed-time 10 --speed-limit 3 --connect-timeout 30 -m 20";

	cout<<cmd<<endl;
	
	if(NULL == (fstream = popen(cmd.c_str(), "r")))
	{  
		cout<<"popen error:"<<cmd<<endl;
		return -1;  
	}  
	
	while (fgets(buff, sizeof(buff), fstream) != NULL)
	{
		buff[strlen(buff) - 1] = '\0';
	}
	
	cout<<buff<<endl;
  
	pclose(fstream);
	return 0;
}
