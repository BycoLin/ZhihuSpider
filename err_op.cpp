#include <iostream>
#include <fstream>
#include <sstream>
#include "err_op.h"
#include "file_str_op.h"

int getErrFowweerFileInfo(const string& sfilepath,map<string,string> &g_mErrFowwInfo)
{
	fstream FileStream;
	FileStream.open(sfilepath.c_str(),ios::in); 
	istreambuf_iterator<char> fileBeg(FileStream),fileEnd;
	string sInfo(fileBeg, fileEnd);
	FileStream.close();
	
	if(sInfo.empty())
	{
		cout<<"no err file"<<endl;
		return 0;
	}
	
	string::size_type  pos(0);  
	string sTmep = "";
	string::size_type  pos2(0);  
	string sTmepFist = "";
	string sFist = "";
	string sTmepSecond = "";
	string sFirstSign = "|||";
	int iFirstSignLen = sFirstSign.length();
	string sSecondSign = "###";
	int iSecondSignLen = sSecondSign.length();
	
	while(1)
	{
		if(sInfo.empty())
			break;
	
		if((pos=sInfo.find(sFirstSign))!=string::npos)     
		{
			sTmep = sInfo.substr(0,pos);
			
			if(sTmep.empty())
				break;
		
			if((pos2=sTmep.find(sSecondSign))!=string::npos)     
			{
				sTmepFist = sTmep.substr(0,pos2);
				
				sFist = trim(sTmepFist);
				sFist = replace_all(sFist,"\n","");
				
				if(pos2+iSecondSignLen <= sTmep.length())
					sTmepSecond = sTmep.substr(pos2+iSecondSignLen,sTmep.length());
				else
					sTmepSecond = "";
				
				if(!sFist.empty() && !sTmepSecond.empty())
					g_mErrFowwInfo.insert(make_pair<string,string>(sFist,sTmepSecond));
			}
		
			if(pos+iFirstSignLen <= sInfo.length())
				sInfo = sInfo.substr(pos+iFirstSignLen,sInfo.length());
		}		
		else
			break;
	}
	
	return 0;
}