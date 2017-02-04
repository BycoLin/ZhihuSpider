#include "file_str_op.h"
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <sstream>
#include <unistd.h>
#include <dirent.h>

int get_file_stream(const string& strTmPath,ofstream &in)
{
	ifstream readfile;
	readfile.open(strTmPath.c_str(),ios::in);
	if(!readfile)
	{
		in.open(strTmPath.c_str(),ios::trunc);
		if(!in)
		{
			cout<<"create spend log err"<<endl;
			return -1;
		}
	}
	else
	{
		in.open(strTmPath.c_str(),ios::app);
		if(!in)
		{
			cout<<"add to spend log err"<<endl;
			readfile.close();
			return -1;
		}
	}
	
	readfile.close();
	 
	return 0;
}

unsigned extract_all_number(const string& str)     
{ 
	unsigned int ret = 0;
	string str2 = "";   
	
	for(size_t i=0; i<str.size(); i++)
	{
	  if((str[i]>='0')&&(str[i]<='9'))
	  {
		  str2 += str[i];
	  }
	  else
	  {
		  return ret;
	  }
	}
	
	if(!str2.empty())
	{
		ret = atoi(str2.c_str());
	}
	
	return ret;
}

string& trim(std::string &s) 
{	
    if (s.empty()) 
    {
        return s;
    }
	
    s.erase(0,s.find_first_not_of(" "));	
    s.erase(s.find_last_not_of(" ") + 1);

    return s;
}

unsigned extract_number(const string& str)     
{ 
	unsigned int ret = 0;
	string str2 = "";   
	bool bBreak = false;
	
	for(size_t i=0; i<str.size(); i++)
	{
	  if((str[i]>='0')&&(str[i]<='9'))
	  {
		  str2 += str[i];
		  bBreak = true;
	  }
	  else
	  {
		  if(bBreak)
			  break;
	  }
	}
	
	if(!str2.empty())
	{
		ret = atoi(str2.c_str());
	}
	
	return ret;
}

string& replace_all(string& str,const string& old_value,const string& new_value)     
{     
	if(str.empty())
		return str;
	
	string::size_type  pos(0);     
	
    while(true)   
	{     
        if((pos=str.find(old_value))!=string::npos)     
            str.replace(pos,old_value.length(),new_value);     
        else   break;     
    }     
    return str;     
}   

bool if_digit(const string& sStr)
{
	if(sStr.empty())
		return false;
	
	for(size_t i=0; i<sStr.size(); i++)
	{
		if(isspace(sStr[i]) || ispunct(sStr[i]))
		{
			continue;
		}
		
		if(!isdigit(sStr[i]))
		{
			return false;
		}
	}

	return true;
}

bool if_english(const string& sStr)
{
	if(sStr.empty())
		return false;
	
	for(size_t i=0; i<sStr.size(); i++)
	{
		if(isspace(sStr[i]) || ispunct(sStr[i]))
		{
			continue;
		}
		
		if(!isalpha(sStr[i]))
		{
			return false;
		}
	}

	return true;
}

bool if_english_num(const string& sStr)
{
	if(sStr.empty())
		return false;
	
	for(size_t i=0; i<sStr.size(); i++)
	{
		if(isspace(sStr[i]) || ispunct(sStr[i]) || isdigit(sStr[i]))
		{
			continue;
		}
		
		if(!isalpha(sStr[i]))
		{
			return false;
		}
	}

	return true;
}

int english_to_upper(string& sStr)
{
	if(sStr.empty())
		return 0;
	
	for(size_t i=0; i<sStr.size(); i++)
	{
		if(isspace(sStr[i]) || ispunct(sStr[i]))
			continue;
		
		if(isalpha(sStr[i]))
		{
			sStr[i] = toupper(sStr[i]);
		}
	}

	return 0;
}

bool if_no_english(const string& sStr)
{
	if(sStr.empty())
		return false;
	
	for(size_t i=0; i<sStr.size(); i++)
	{
		if(isspace(sStr[i]) || ispunct(sStr[i]))
			continue;
		
		if(isalpha(sStr[i]))
			return false;
	}

	return true;
}

bool if_part_english_same(const string& sStrA,const string& sStrB)
{
	if(sStrA.empty())
		return false;

	string sTmep = "";
	int iCount = 0;
	
	for(size_t i=0; i<sStrA.size(); i++)
	{
		if(ispunct(sStrA[i]))
			continue;
		
		if(isspace(sStrA[i]))
		{
			if(iCount > 0)
			{
				sTmep = sTmep+sStrA[i];	
			}
			
			continue;
		}
		
		if(isalpha(sStrA[i]))
		{
			sTmep = sTmep+sStrA[i];
			iCount++;
		}
	}
	
	if(iCount > 0)
	{
		if(sStrB == sTmep)
			return true;
	}

	return false;
}

int rm_space_between_en_and_non_english(string& sStrPhrase)
{
	string sTmepPhrase = sStrPhrase;
		
	if(sTmepPhrase.empty())
		return 0;
	
	if(if_english(sTmepPhrase))
		return 0;
	
	if(if_no_english(sTmepPhrase))
		return 0;
	
	bool bEnglish = false;
	bool bNonEnglish = false;
	string sTmep = "";
	string sTmepLast = "";
	string sTmepFront = "";
	
	for(size_t i=0; i<sTmepPhrase.size(); i++)
	{
		sTmepFront = sTmep;
		sTmep = sTmep+sTmepPhrase[i];
		
		if(isspace(sTmepPhrase[i]) || ispunct(sTmepPhrase[i]) || isdigit(sTmepPhrase[i]))
		{
			continue;
		}
		
		if(isalpha(sTmepPhrase[i]))
		{
			bEnglish = true;

			if(bNonEnglish)
			{
				if(i-1 >= 0)
				{
					if(isspace(sTmepPhrase[i-1]))
					{
						sTmep = sTmep.substr(0,i-1);
					}
					else
					{
						sTmep = sTmepFront;
					}
				}
				
				sTmepLast = sStrPhrase.substr(i,sStrPhrase.length());
			
				sStrPhrase = sTmep+sTmepLast;
				
				return 1;
			}
		}
		else
		{
			bNonEnglish = true;
			
			if(bEnglish)
			{
				if(i-1 >= 0)
				{
					if(isspace(sTmepPhrase[i-1]))
					{
						sTmep = sTmep.substr(0,i-1);
					}
					else
					{
						sTmep = sTmepFront;
					}
				}
				
				sTmepLast = sStrPhrase.substr(i,sStrPhrase.length());
			
				sStrPhrase = sTmep+sTmepLast;
			
				return 1;
			}
		}
	}

	return 0;
}

int string_find_count(const string &str,const string &sValue)
{
	string sTemp = str;
	
	if(sTemp.empty())
		return 0;
	
	string::size_type  pos(0);  
	int iCount = 0;
	int iValue = sValue.length();
	 
    while(true)   
	{     
        if((pos=sTemp.find(sValue))!=string::npos)    
		{
			if(pos+iValue <= sTemp.length())
			{
				iCount++;
				sTemp = sTemp.substr(pos+iValue,sTemp.length());  
			}				
			else
				break;
		}
        else   
			break;     
    }     
	
    return iCount;     
}

int string_find_count_info(const string &str,const string &sValue,string *pSvalue,int iMax)
{
	string sTemp = str;
	int retNum = 0;
	
	if((retNum = string_find_count(sTemp,sValue)) <= 0)
		return -1;

	string::size_type  pos(0);  
	int iCount = 0;
	int i;
	string sInfo = "";
	int iValue = sValue.length();
	
	for(i = 0;i<=retNum;i++)
	{
		if((pos=sTemp.find(sValue))!=string::npos)    
		{
			sInfo = sTemp.substr(0,pos); 
			
			if(iCount < iMax)
				pSvalue[iCount++] = sInfo;
				
			if(pos+iValue <= sTemp.length())
			{
				sTemp = sTemp.substr(pos+iValue,sTemp.length()); 
			}				
		}
		else
		{
			if(iCount < iMax)
				pSvalue[iCount++] = sTemp;
		}
	}
	
    return iCount;     
}

wstring s2ws(const string& s)
{
	setlocale(LC_ALL, "chs");
	const char* _Source = s.c_str();
	size_t _Dsize = s.size() + 1;
	cout<<_Dsize<<endl;
	wchar_t *_Dest = new wchar_t[_Dsize];
	wmemset(_Dest, 0, _Dsize);
	mbstowcs(_Dest,_Source,_Dsize);
	cout<<"s:"<<_Source<<endl;
	std::wstring result = _Dest;
	wcout<<"w:"<<result<<endl;
	delete []_Dest;
	setlocale(LC_ALL, "C");
	return result;
}

int digit_rail_digit_dispose(string& sFullName,const string& sRail)
{
	int iRetNum = 0;
	string aValue[10] = "";
	int m = 0;
	string sDuan = "";
	bool bPreDigit = false;
	
	if(sFullName.empty() || sRail.empty())
		return 0;
		
	iRetNum = string_find_count_info(sFullName,sRail,aValue,10);
	
	if(iRetNum > 0)
	{
		for(m = 0;m < iRetNum;m++)
		{
			sDuan = aValue[m];
			//cout<<"duan:"<<sDuan<<endl;
		
			if(sDuan.empty() || sDuan.size() < 1)
				continue;

			if(bPreDigit)
			{
				if(isdigit(sDuan[0]))
					return 1;
				else
					bPreDigit = false;
			}
					
			if(!isdigit(sDuan[sDuan.size()-1]))
				bPreDigit = false;
			else
				bPreDigit = true;
		}
	}
	
	return 0;
}

int del_symbol_pair_info(string &sFullName,const string& sLeftSymbol,const string &sRightSymbol)
{
	bool bKuoHao = false;
	string sTmepFist = "";
	string sTmepSecond = "";
	string::size_type pos(0); 
	string::size_type pos2(0);
	
	if(sFullName.empty() || sLeftSymbol.empty() || sRightSymbol.empty())
		return 0;
	
	int iLeftLen = sLeftSymbol.length();
	int iRightLen = sRightSymbol.length();
	
	if((pos=sFullName.find(sLeftSymbol))!=string::npos)   
	{			
		sTmepFist = sFullName.substr(0,pos);
		
		if(pos+iLeftLen <= sFullName.length())
		{
			sTmepSecond = sFullName.substr(pos+iLeftLen,sFullName.length());
			
			if((pos2=sTmepSecond.find(sRightSymbol))!=string::npos)
			{
				if(pos2+iRightLen <= sTmepSecond.length())
				{
					sTmepSecond = sTmepSecond.substr(pos2+iRightLen,sTmepSecond.length());
					
					sFullName = sTmepFist+sTmepSecond;
					
					bKuoHao = true;
				}
			}
		}
		
		if(!bKuoHao)
		{
			sFullName = sFullName.substr(0,pos);
		}
	}
	
	return 0;
}

string UrlEncode(const std::string& szToEncode)  
{ 
	string src = szToEncode;  

	char hex[] = "0123456789ABCDEF";  
	string dst;  
	unsigned char cc;
	unsigned char c;
	 
	for (size_t i = 0; i < src.size(); ++i)
	{  
		cc = src[i];  
		if(isascii(cc))  
		{  
			if (cc == ' ')  
			{  
				dst += "%20"; 
			}  
			else 
			{
				dst += cc;  
			}  
		}
		else 
		{
			c = static_cast<unsigned char>(src[i]);
			dst += '%';  
			dst += hex[c / 16];  
			dst += hex[c % 16];
		}
	}  
	
	return dst;  
}  

int copy_str_array(const string *strOrg,const int iOrgNum,string *strGoal,int *iGoalNum)
{
	if(!strOrg || !strGoal || iOrgNum <= 0)
		return 0;
	
	int i;
	
	for(i = 0;i<iOrgNum;i++)
	{
		strGoal[i] = strOrg[i];
	}
	
	if(iGoalNum)
		*iGoalNum = iOrgNum;
	
	return 0;
}

bool fGetFileDir(string& strFileDir)  
{  
    char szWorkDir[PATH_MAX] = {0};  

    if(!getcwd(szWorkDir, PATH_MAX))  
    {  
        return false;  
    }  
  
	strFileDir.clear() ;  
    strFileDir = szWorkDir ;  
    strFileDir.append("/") ;  
   
    return true ;  
}  

string unicode2chinese(string& sInfo)
{
	if(sInfo.empty())
		return sInfo;
	
	string output = "";
	string::iterator it = sInfo.begin();
	wchar_t wide;
	char ansi[4];

	for(;it != sInfo.end();it++) 
	{
		if (*it != '\\')
		{
			output.push_back(*it);
		}
		else
		{
			if (*(it+1) == 'u' || *(it+1) == 'U')
			{
				string tmp(it + 2,it + 6);
				wide = strtol(tmp.c_str(), NULL, 16);
				(void)setlocale(LC_ALL, "");
				ansi[3] = '\0';
				wctomb(ansi,wide);
				output.append(ansi);
				it += 5;
			}
			else
			{
				output.push_back(*it);
			}
		}
	} 
	
	return output;
}

string filterSpeStr(string& sInfo)
{
	if(sInfo.empty())
		return sInfo;
	
	string output = "";
	string::iterator it = sInfo.begin();
	
	for(;it != sInfo.end();it++) 
	{
		if (*it != '\\')
		{
			output.push_back(*it);
		}
		else
		{
			if (*(it+1) == 'u' || *(it+1) == 'U')
			{
				if (*(it+2) == '0' && *(it+3) == '0')
				{
					if (*(it+4) == 'e' || *(it+4) == 'f')
					{
						output.push_back('?');
						it += 5;
					}
					else
					{
						output.push_back(*it);
					}
				}
				else
				{
					output.push_back(*it);
				}
			}
			else
			{
				output.push_back(*it);
			}
		}
	} 
	
	return output;
}

int filterEmojiStr(const string &source,string &sFilterStr) 
{		
	if(source.empty())
		return -1;
	
	const char* pInfo = source.c_str();
	bool bSelect = false;
	string sGetInfo = "";
	unsigned int uiCha = 0;
	
	for(size_t i=0; i<source.size(); i++)
	{	
		uiCha = pInfo[i];
		
		bSelect = false;

		//一个表情符号utf8解码后是4个char
		
		//开头为0xFO的表情符号
		//0xF0FFFFFF
		if (uiCha == 4294967280) 
		{
			i = i+3;
			bSelect = true;
		}
		//开头为0XE2，0XE3的三个字符的表情
		else if(uiCha == 14876671 || uiCha == 14942207)
		{
			i = i+2;
			bSelect = true;
		}
		else 
		{
			//0x30FFFFFF,0x31FFFFFF,0x32FFFFFF,0x33FFFFFF,0x34FFFFFF,0x35FFFFFF,0x36FFFFFF,0x37FFFFFF,0x38FFFFFF,0x39FFFFFF,0x23FFFFFF
			
			if(uiCha == 822083583 || uiCha == 838860799 || uiCha == 855638015 || uiCha == 872415231 ||
			uiCha == 889192447 || uiCha == 905969663 || uiCha == 922746879 || uiCha == 939524095 ||
			uiCha == 956301311 || uiCha == 973078527 || uiCha == 603979775)
			{
				i = i+3;
				bSelect = true;
			}
		}

        if (!bSelect) 
		{
			sGetInfo.append(1,pInfo[i]);
        }
	}
	
	sFilterStr = trim(sGetInfo);
	sFilterStr = replace_all(sFilterStr,"®","");

	return 0;
}

int filterNoUTF8Str(const string &source,string &sFilterStr) 
{		
	if(source.empty())
		return -1;
	
	const char* pInfo = source.c_str();
	bool bSelect = false;
	string sGetInfo = "";
	unsigned int uiCha = 0;
	
	for(size_t i=0; i<source.size(); i++)
	{	
		uiCha = pInfo[i];
		
		bSelect = false;
		
		if(uiCha < 0x80)
		{
			
		}
		else if(uiCha < 0xC0)
		{
			
		}
		else if(uiCha < 0xE0)
		{
			
		}
		else if(uiCha < 0xF0)
		{
			
		}
		else if(uiCha < 0xF8)
		{
			i = i+4;
			bSelect = true;
		}
		else if(uiCha < 0xFC)
		{
			i = i+5;
			bSelect = true;
		}
		else if(uiCha < 0xFE)
		{
			i = i+6;
			bSelect = true;
		}
		else
		{
			
		}
		
        if (!bSelect) 
		{
			sGetInfo.append(1,pInfo[i]);
        }
	}
	
	sFilterStr = trim(sGetInfo);
	sFilterStr = replace_all(sFilterStr,"®","");
	
	cout<<"filter:"<<sFilterStr<<endl;

	return 0;
}

int get_baidu_docid(const string &sRetUrl,string &sDocID)
{
	string::size_type pos(0);   
	string sDocTemp = BAIDU_DOCID;
	int iDocIDLen = sDocTemp.length();
	
	if(sRetUrl.empty())
		return -1;
	
	string sTmpUrl = sRetUrl;
	
	if((pos=sTmpUrl.find(BAIDU_DOCID))!=string::npos)     
	{
		if(pos+iDocIDLen <= sTmpUrl.length())
		{
			sDocTemp = sTmpUrl.substr(pos+iDocIDLen,sTmpUrl.length());
			
			if((pos=sDocTemp.find(BAIDU_AND))!=string::npos)
			{
				sDocID = sDocTemp.substr(0,pos);
				
				return 0;
			}
		}
	}	
	
	return -1;
}

void IntToString (string& out, const int value)  
{  
	out = "";
    char buf[32];  
    snprintf (buf, sizeof (buf), "%d", value); 
    out.append (buf);  
}   

void insert_spe_sign_dispose(string &sOrgStr)
{
	sOrgStr = replace_all(sOrgStr,"\""," ");
	sOrgStr = replace_all(sOrgStr,"\\"," ");
}
