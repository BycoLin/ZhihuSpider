#include "pull_zhihu.h"
#include "CharsetConverter.h"
#include "Uri.h"
#include "ParserDom.h"
#include "utils.h"
#include "redis_op.h"
#include <math.h>
#include "time_op.h"
#include "dispose_zhihu.h"

string g_strFileDir = "";
string g_strFowUrlBak = "";

int iFowEname = 0;
string saFowEname[FOW_TIMES] = "";

string aZhihuHelp[DISPOSE_LAST] = 
{
	{"首页"},
	{"今日最热"},
	{"本月最热"},
	{"话题"},
	{"关注"},
	{"提问"},
	{"回答"},
	{"文章"},
	{"收藏"},
	{"关注的少量专栏"},
	{"关注的专栏"},
	{"关注的话题"},
	{"测试关注者"},
	{"测试所有关注者"},
	{"发表文章"},
	{"今日最热三十条"},
	{"发表今日最热三十条文章"},
	{"从种子用户开始获取所有用户"},
	{"从准备队列中开始获取所有用户"},
	{"专用测试"},
	{"子话题"},
	{"关注错误重试"},
	{"更新用户信息"},
	{"拉取子话题的精华回答"},
	{"更新子话题的精华回答"},
	{"精华话题json输出"},
	{"风云人物json输出"},
	{"统计数据json输出"},
	{"登录知乎并获取cookies"},
	{"随机发表精华话题到鸭哥推荐"},
	{"更新单个用户信息"},
};

int help()
{
	for(int i=0;i<DISPOSE_LAST;i++)
	{
		cout<<i<<" "<<aZhihuHelp[i]<<endl;
	}
	
	return 0;
}

void getCurlZhiHuUrl(const string& sOrgUrl,string &sUrl,const string &strFileDir)
{
	string strPullPath = "";
	
	if(get_path_file_name(20,strFileDir,strPullPath) < 0)
		return;
	
	getCurlZhiHuUrlCookies(sOrgUrl,sUrl,strPullPath);
}

void getCurlZhiHuUrlCookies(const string& sOrgUrl,string &sUrl,string &sCookiesFileName)
{
	sUrl = sOrgUrl+ZHIHU_URL_HOST+ZHIHU_URL_USER_AGENT+ZHIHU_URL_ACCEPT+ZHIHU_URL_ACCEPT_LANG+ZHIHU_URL_ACCEPT_ENCODING+ZHIHU_URL_CONNECTION+ZHIHU_URL_CACHE_CONTROL+ZHIHU_URL_B+sCookiesFileName;
}

void getCurlZhiHuSubTopicsUrl2(const string& sOrgUrl,string &sUrl,string &sData)
{
	sUrl = sOrgUrl+ZHIHU_URL_HOST+ZHIHU_URL_USER_AGENT+ZHIHU_URL_ACCEPT+ZHIHU_URL_ACCEPT_LANG+ZHIHU_URL_ACCEPT_ENCODING+ZHIHU_URL_CONNECTION+ZHIHU_URL_CACHE_CONTROL+ZHIHU_URL_DATA+sData;
}

void getCurlZhiHuSubTopicsUrl(const string& sOrgUrl,string &sUrl,string &sData)
{
	sUrl = sOrgUrl+ZHIHU_URL_HOST+ZHIHU_URL_USER_AGENT+ZHIHU_URL_ACCEPT+ZHIHU_URL_ACCEPT_LANG+ZHIHU_URL_ACCEPT_ENCODING+ZHIHU_URL_COOKIE2+ZHIHU_URL_CONNECTION+ZHIHU_URL_CACHE_CONTROL+ZHIHU_URL_DATA+sData;
}

void get_daily_hot_url(const string &sOffset,string &sUrl)
{
	sUrl = ZHIHU_HOT_PRE;
	
	sUrl = sUrl+sOffset+ZHIHU_HOT_MID+ZHIHU_DAY+ZHIHU_HOT_BAK;	
}

void get_month_hot_url(const string &sOffset,string &sUrl)
{
	sUrl = ZHIHU_HOT_PRE;
	
	sUrl = sUrl+sOffset+ZHIHU_HOT_MID+ZHIHU_MONTH+ZHIHU_HOT_BAK;	
}

int get_file_info(const string& strPullPath,string &sInfo)
{
	if(strPullPath.empty())
	{
		cout<<"strPullPath empty"<<endl;
		return -1;
	}
	
	fstream FileStream;
	FileStream.open(strPullPath.c_str(),ios::in); 
	istreambuf_iterator<char> fileBeg(FileStream),fileEnd;
	string sTempInfo(fileBeg, fileEnd);
	FileStream.close(); 
				
	if(sTempInfo.empty())
	{
		cout<<"info empty"<<endl;
		return -1;
	}
	
	sInfo = sTempInfo;
	
	return 0;
}

int html_dispose_get_attr_next_text(tree<HTML::Node>::iterator &it,const string& sTagName,const string& sAttrName,const string& sSecondName,string& sGetTextName)
{
	if(sTagName.empty() || sAttrName.empty() || sSecondName.empty())
		return -1;
	
	std::pair<bool, string> pAttr;
	tree<HTML::Node>::iterator itTemp;
	tree<HTML::Node> dom;
	
	if (it->isTag() && !strcasecmp(it->tagName().c_str(),sTagName.c_str()))
	{
		it->parseAttributes();
		pAttr = it->attribute(sAttrName.c_str());
		if(pAttr.first && !strcasecmp(pAttr.second.c_str(),sSecondName.c_str()))
		{		
			itTemp= dom.begin(it);
			sGetTextName = itTemp->text();
			return 1;
		}
	}
	
	return 0;
}

int html_dispose_get_attr_second(tree<HTML::Node>::iterator &it,const string& sTagName,const string& sAttrName,string& sGetSecondName)
{
	if(sTagName.empty() || sAttrName.empty())
		return -1;
	
	std::pair<bool, string> pAttr;
	
	if (it->isTag() && !strcasecmp(it->tagName().c_str(),sTagName.c_str()))
	{
		it->parseAttributes();
		pAttr = it->attribute(sAttrName.c_str());
		if(pAttr.first)
		{		
			sGetSecondName = pAttr.second;
			return 1;
		}
	}
	
	return 0;
}

int html_dispose_get_other_attr_second(tree<HTML::Node>::iterator &it,const string& sTagName,const string& sAttrName,const string& sSecondName,const string& sOtherAttrName,string &sGetOtherSecondName)
{
	if(sTagName.empty() || sAttrName.empty() || sSecondName.empty() || sOtherAttrName.empty())
		return -1;
	
	std::pair<bool, string> pAttr;
	
	if (it->isTag() && !strcasecmp(it->tagName().c_str(),sTagName.c_str()))
	{	
		it->parseAttributes();
		pAttr = it->attribute(sAttrName.c_str());
		if(pAttr.first && !strcasecmp(pAttr.second.c_str(),sSecondName.c_str()))
		{				
			pAttr = it->attribute(sOtherAttrName.c_str());
			if(pAttr.first)
			{	
				sGetOtherSecondName = pAttr.second;
				return 1;
			}		
		}
	}
	
	return 0;
}

int disposeAjax(const string& sInfo,const string& sName,string* asName,unsigned &uiNum,unsigned uiMaxNum,int subLen,string subType)
{
	if(sInfo.empty() || sName.empty())
		return -1;
	
	string strTemp="";
	string strTemp2 = "";
	bool bInsert = false;
	unsigned uiPkgNameLen = sName.length();
	string::size_type  pos(0);   
	
	strTemp2 = strTemp = sInfo;
	
	while(!bInsert)
	{
		if((pos=strTemp2.find(sName))!= string::npos)   
		{							
			if(pos+uiPkgNameLen+subLen <= strTemp2.length())
				strTemp2 = strTemp2.substr(pos+uiPkgNameLen+subLen,strTemp2.length());
			
			strTemp = strTemp2;
			
			if((pos=strTemp.find(subType))!= string::npos)   
			{							
				strTemp = strTemp.substr(0,pos);
			
				if(uiNum >= uiMaxNum)
					break;
				
				asName[uiNum++] = strTemp;
			}
			else
			{
				bInsert = true;
			}
		}
		else
		{
			bInsert = true;
		}
	}
	
	return 0;
}

int dispose_zhihu_html(const string& strPullPath)
{
	string sUrl = "";
	string sInfo = "";
	string sTempInfo = "";
	
	cout<<"1:"<<endl;
	
	if(get_file_info(strPullPath,sInfo) < 0)
		return -1;
	
	cout<<"2:"<<endl;
	
	if(sInfo.find("</html>")==string::npos)  
	{		
		cout<<"data only part"<<endl;
		return -1;
	}
	
	cout<<"3:"<<endl;
	
	HTML::ParserDom parser;
	tree<HTML::Node>::iterator it;
	tree<HTML::Node>::iterator end;
	tree<HTML::Node>::iterator itTemp;
	tree<HTML::Node> dom;

	dom = parser.parseTree(sInfo);
	
	it= dom.begin();
	end= dom.end();
	
	for(; it != end; ++it)
	{				
		if(html_dispose_get_other_attr_second(it,"a","class","question_link","href",sUrl) == 1)
		{
			sUrl = ZHIHU_URL+sUrl;
			
			cout<<"url:"<<sUrl<<endl;
			
			itTemp= dom.begin(it);
				
			cout<<"question:"<<itTemp->text()<<endl;
		}
		
		if(html_dispose_get_attr_second(it,"a","data-votecount",sTempInfo) == 1)
		{
			cout<<"suppor:"<<sTempInfo<<endl;
		}
	}
	
	return 0;
}

int dispose_zhihu_daily_hot_html(const string& strPullPath,vector<TDAILYHOTDATA> &v_stDailyHotData)
{
	string sUrl = "";
	string sInfo = "";
	string sTempInfo = "";
	
	if(get_file_info(strPullPath,sInfo) < 0)
		return -1;
	
	HTML::ParserDom parser;
	tree<HTML::Node>::iterator it;
	tree<HTML::Node>::iterator end;
	tree<HTML::Node>::iterator itTemp;
	tree<HTML::Node>::iterator endTemp;
	tree<HTML::Node> dom;
	
	std::pair<bool, string> pAttr;

	dom = parser.parseTree(sInfo);
	
	it= dom.begin();
	end= dom.end();
	
	string sAskInfo[DAILY_HOT_PAGE_NUM]= "";
	int iAskNum = 0;
	
	TDAILYHOTDATA stData;

	for(; it != end; ++it)
	{			
		if(html_dispose_get_other_attr_second(it,"a","class","question_link","href",sUrl) == 1)
		{
			sUrl = ZHIHU_COM_URL+sUrl;
			
			stData.sQuestionHref = sUrl;
			
			cout<<"url:"<<sUrl<<endl;
			
			itTemp= dom.begin(it);
			
			sTempInfo = itTemp->text();
			
			sTempInfo = replace_all(sTempInfo,"\n","");
			sTempInfo = trim(sTempInfo);
			
			stData.sQuestionInfo = sTempInfo;
				
			cout<<"question:"<<sTempInfo<<endl;
		}
		
		if(html_dispose_get_other_attr_second(it,"a","class","author-link","href",sUrl) == 1)
		{
			sUrl = ZHIHU_COM_URL+sUrl;
			
			stData.sPeopleHref = sUrl;
			
			cout<<"author url:"<<sUrl<<endl;
			
			itTemp= dom.begin(it);
			
			stData.sPeopleName = itTemp->text();
				
			cout<<"author:"<<itTemp->text()<<endl;
		}
		else
		{
			if(html_dispose_get_attr_next_text(it,"span","class","name",sTempInfo) == 1)
			{
				stData.sPeopleName = sTempInfo;
				
				stData.sPeopleHref = "";
				
				cout<<"author:"<<stData.sPeopleName<<endl;
			}
		}
		
		if(html_dispose_get_attr_second(it,"div","data-votecount",sTempInfo) == 1)
		{
			cout<<"suppor:"<<sTempInfo<<endl;
			
			stData.sAgreeNum = sTempInfo;
		}
		
		if(html_dispose_get_attr_next_text(it,"i","class","z-icon-comment",sTempInfo) == 1)
		{
			cout<<"comment:"<<sTempInfo<<endl;
		}
		
		if (it->isTag() && !strcasecmp(it->tagName().c_str(),"div"))
		{
			it->parseAttributes();
			pAttr = it->attribute("class");
			
			sTempInfo = "";
			
			if(pAttr.first && !strcasecmp(pAttr.second.c_str(),"zh-summary summary clearfix"))
			{
				itTemp= dom.begin(it);
				endTemp= dom.end(it);
				
				for(; itTemp != endTemp; ++itTemp)
				{		
					if (itTemp->isTag() && !strcasecmp(itTemp->tagName().c_str(),"img"))
					{
						continue;
					}
					else if (itTemp->isTag() && !strcasecmp(itTemp->tagName().c_str(),"a"))
					{
						continue;
					}
					
					sTempInfo = sTempInfo + itTemp->text();
					
					if(sTempInfo == "" || sTempInfo == "\r\n" || sTempInfo == "\r" || sTempInfo == "\n")
						continue;
					
					sTempInfo = trim(sTempInfo);
					sTempInfo = replace_all(sTempInfo,"\r\n","");
					sTempInfo = replace_all(sTempInfo,"\r","");
					sTempInfo = replace_all(sTempInfo,"\n","");
					sTempInfo = replace_all(sTempInfo,"显示全部","");
					sTempInfo = trim(sTempInfo);
				}
				
				if(iAskNum < DAILY_HOT_PAGE_NUM && !sTempInfo.empty())
				{
					while(sTempInfo.find("<") != string::npos && sTempInfo.find(">") != string::npos)
						del_symbol_pair_info(sTempInfo,"<",">");
					
					sAskInfo[iAskNum++] = sTempInfo;
					stData.sAnswerInfo = sTempInfo;
					
					v_stDailyHotData.push_back(stData);
				}
			}
		}
	}
	
	for(int i=0;i<iAskNum;i++)
		cout<<i<<":"<<sAskInfo[i]<<endl;
	
	return 0;
}

int dispose_zhihu_topic_html(const string& strPullPath,map<int,TTOPICS> &m_stTopics)
{
	string sTempInfo = "";
	string sInfo = "";
	TTOPICS stData;
	int iTopicID = 0;
	string sTempName = "";
	
	if(get_file_info(strPullPath,sInfo) < 0)
		return -1;
			
	if(sInfo.find("</html>")==string::npos)  
	{		
		cout<<"data only part"<<endl;
		return -1;
	}
	
	HTML::ParserDom parser;
	tree<HTML::Node>::iterator it;
	tree<HTML::Node>::iterator end;
	tree<HTML::Node>::iterator itTemp;
	tree<HTML::Node> dom;

	dom = parser.parseTree(sInfo);
	
	it= dom.begin();
	end= dom.end();
	
	for(; it != end; ++it)
	{			
		if(html_dispose_get_attr_second(it,"li","data-id",sTempInfo) == 1)
		{
			cout<<"topic_id:"<<sTempInfo<<endl;
		
			if(!sTempInfo.empty())
			{
				iTopicID = extract_all_number(sTempInfo);
				
				if(iTopicID > 0)
					stData.iTopicID = iTopicID;
			}
			
			itTemp = dom.begin(it);
			
			if(html_dispose_get_attr_second(itTemp,"a","href",sTempInfo) == 1)
			{
				sTempName = sTempInfo;
				
				stData.sTopicName = replace_all(sTempName,"#","");
				cout<<stData.sTopicName<<endl;
				
				sTempInfo = ZHIHU_TOPIC+sTempInfo;
				cout<<sTempInfo<<endl;
				
				stData.sTopicUrl = sTempInfo;
				
				if(stData.iTopicID>0)
					m_stTopics.insert(make_pair<int,TTOPICS>(stData.iTopicID,stData));
			}
		}
	}
	
	return 0;
}

int dispose_zhihu_sub_topic_json(int iParentTopicID,const string& strPullPath,vector<TSUBTOPIC> &v_stSubTopic)
{
	TSUBTOPIC stData;
	string sUrl = "\/topic";
	string asUrl[PAGE_NUMS] = "";
	unsigned int uiUrlNum = 0;
	string sName = "alt";
	string asName[PAGE_NUMS] = ""; 
	unsigned int uiNum = 0;
	string sOrgName = "";
	string sTemp = "";
	string sTName = "";
	string sInfo = "";
	int iSubTopicID = 0;
	int i = 0;
	
	if(get_file_info(strPullPath,sInfo) < 0)
		return -1;

	if(sInfo.find("msg") == string::npos)
		return -1;
	
	disposeAjax(sInfo,sUrl,asUrl,uiUrlNum,PAGE_NUMS,2,"\\");
	disposeAjax(sInfo,sName,asName,uiNum,PAGE_NUMS,3,"\"");
	
	if(uiUrlNum != uiNum)
		return -1;
	
	if(uiUrlNum == 0 || uiNum == 0)
		return -1;
	
	for(i = 0;i<uiUrlNum;i++)
	{
		sTemp = asUrl[i];
		
		iSubTopicID = extract_all_number(sTemp);
		
		cout<<iSubTopicID<<endl;
		
		sTemp = ZHIHU_SUB_TOPIC + sTemp;
		
		cout<<sTemp<<endl;
		
		sTName = asName[i];
		
		sTName = replace_all(sTName,"\\u00b7","·");
		
		sTName = filterSpeStr(sTName);
		
		cout<<sTName<<endl;
		
		sOrgName = unicode2chinese(sTName);

		sOrgName = replace_all(sOrgName,"\\","");
		
		cout<<sOrgName<<endl;
		
		stData.iTopicID = iParentTopicID;
		stData.iSubID = iSubTopicID;
		stData.sSubName = sOrgName;
		stData.sSubUrl = sTemp;
		
		v_stSubTopic.push_back(stData);
	}
	
	return 0;
}

int dispose_zhihu_follow_html(const string& strPullPath,int iType)
{
	string sUrl = "";
	string sTemp = "";
	string sInfo = "";
	string sDataID = "";
	string sXsrf = "";

	if(get_file_info(strPullPath,sInfo) < 0)
		return -1;
			
	if(iType == 0)
	{
		if(sInfo.find("</html>")==string::npos)  
		{		
			cout<<"data only part"<<endl;
			return -1;
		}
	}
	
	HTML::ParserDom parser;
	tree<HTML::Node>::iterator it;
	tree<HTML::Node>::iterator end;
	tree<HTML::Node>::iterator itTemp;
	tree<HTML::Node>::iterator endTemp;
	tree<HTML::Node> dom;
	std::pair<bool, string> pAttr;

	dom = parser.parseTree(sInfo);
	
	it= dom.begin();
	end= dom.end();
	
	for(; it != end; ++it)
	{			
		if (it->isTag() && !strcasecmp(it->tagName().c_str(),"div"))
		{
			it->parseAttributes();
			pAttr = it->attribute("class");
			
			if(pAttr.first && !strcasecmp(pAttr.second.c_str(),"zm-list-content-medium"))
			{
				itTemp= dom.begin(it);
				endTemp= dom.end(it);
	
				for(; itTemp != endTemp; ++itTemp)
				{		
					if(html_dispose_get_other_attr_second(itTemp,"a","class","zg-link","href",sUrl) == 1)
					{
						//cout<<"followee url:"<<sUrl<<endl;
						
						sTemp = replace_all(sUrl,ZHIHU_PEOPLE,"");
						
						//cout<<"followee e_name:"<<sTemp<<endl;
						
						if(iFowEname < FOW_TIMES)
							saFowEname[iFowEname++] = sTemp;
					}
					
					if(html_dispose_get_other_attr_second(itTemp,"a","class","zg-link","title",sTemp) == 1)
					{
						cout<<"followee name:"<<sTemp<<endl;
					}
					
					if(html_dispose_get_attr_next_text(itTemp,"a","class","zg-link-gray-normal",sTemp) == 1)
					{
						//cout<<"followee join:"<<sTemp<<endl;
					}
					
					if(html_dispose_get_attr_next_text(itTemp,"div","class","zg-big-gray",sTemp) == 1)
					{
						//cout<<"followee con:"<<sTemp<<endl;
					}
				}
			}
			else if(pAttr.first && !strcasecmp(pAttr.second.c_str(),"zm-profile-header-op-btns clearfix"))
			{
				itTemp= dom.begin(it);
				endTemp= dom.end(it);
	
				for(; itTemp != endTemp; ++itTemp)
				{		
					if(html_dispose_get_attr_second(itTemp,"button","data-id",sDataID) == 1)
					{
						//cout<<"followee data-id:"<<sDataID<<endl;
					}
				}
			}
		}
		
		/*
		if(html_dispose_get_other_attr_second(it,"input","name","_xsrf","value",sXsrf) == 1)
		{
			//cout<<"followee _xsrf:"<<sXsrf<<endl;
		}
		*/
	}
	
	if(!sDataID.empty())
	{
		g_strFowUrlBak = ZHIHU_NODE_2 + sDataID;
	}
	
	return 0;
}

int dispose_zhihu_get_follow_brief_html(const string& strPullPath,vector<SUSERINFODATA> &v_stUserData)
{
	string sUrl = "";
	string sTemp = "";
	string sInfo = "";
	string sDataID = "";
	SUSERINFODATA stData;
	bool bData = false;
	
	if(get_file_info(strPullPath,sInfo) < 0)
		return -1;
	
	HTML::ParserDom parser;
	tree<HTML::Node>::iterator it;
	tree<HTML::Node>::iterator end;
	tree<HTML::Node>::iterator itTemp;
	tree<HTML::Node>::iterator endTemp;
	tree<HTML::Node> dom;
	std::pair<bool, string> pAttr;

	dom = parser.parseTree(sInfo);
	
	it= dom.begin();
	end= dom.end();
	
	for(; it != end; ++it)
	{			
		if (it->isTag() && !strcasecmp(it->tagName().c_str(),"div"))
		{
			it->parseAttributes();
			pAttr = it->attribute("class");
			
			if(pAttr.first && !strcasecmp(pAttr.second.c_str(),"zm-list-content-medium"))
			{
				itTemp= dom.begin(it);
				endTemp= dom.end(it);
	
				for(; itTemp != endTemp; ++itTemp)
				{		
					if(html_dispose_get_other_attr_second(itTemp,"a","class","zg-link author-link","href",sUrl) == 1)
					{						
						sTemp = replace_all(sUrl,ZHIHU_PEOPLE,"");
						
						cout<<"followee e_name:"<<sTemp<<endl;
						
						stData.sEName = sTemp;
					}
					
					if(html_dispose_get_other_attr_second(itTemp,"a","class","zg-link author-link","title",sTemp) == 1)
					{	
						cout<<"followee z_name:"<<sTemp<<endl;
						
						stData.sZHname = sTemp;
						
						v_stUserData.push_back(stData);
						
						bData = true;
					}
				}
			}
		}
		
		if(html_dispose_get_attr_second(it,"button","data-id",sTemp) == 1)
		{
			cout<<"data-id:"<<sTemp<<endl;
			
			stData.sHid = sTemp;
		}
	}
	
	if(!bData)
		return -1;
	
	return 0;
}

int dispose_zhihu_get_people_index_html(const string& strPullPath,SUSERINFODATA &stData)
{
	string sUrl = "";
	string sTemp = "";
	string sInfo = "";
	string sDataID = "";
	string sXsrf = "";
	bool bFirst = false;
	string sNum = "";
	unsigned uiNum = 0;
	bool bItem = false;

	if(get_file_info(strPullPath,sInfo) < 0)
		return -1;
	
	if(sInfo.find("</html>")==string::npos)  
	{		
		cout<<"data only part"<<endl;
		return -1;
	}
	
	HTML::ParserDom parser;
	tree<HTML::Node>::iterator it;
	tree<HTML::Node>::iterator end;
	tree<HTML::Node>::iterator itTemp;
	tree<HTML::Node>::iterator endTemp;
	tree<HTML::Node>::iterator tempTemp;
	tree<HTML::Node> dom;
	std::pair<bool, string> pAttr;

	dom = parser.parseTree(sInfo);
	
	it= dom.begin();
	end= dom.end();
	
	for(; it != end; ++it)
	{			
		if (it->isTag() && !strcasecmp(it->tagName().c_str(),"div"))
		{
			it->parseAttributes();
			pAttr = it->attribute("class");
			
			if(pAttr.first && !strcasecmp(pAttr.second.c_str(),"zm-profile-side-following zg-clear"))
			{
				itTemp= dom.begin(it);
				endTemp= dom.end(it);
	
				for(; itTemp != endTemp; ++itTemp)
				{		
					if (itTemp->isTag() && !strcasecmp(itTemp->tagName().c_str(),"strong"))
					{
						tempTemp = dom.begin(itTemp);
						cout<<"guanzhu:"<<tempTemp->text()<<endl;
						
						sNum = tempTemp->text();
						
						if(!sNum.empty())
							uiNum = atoi(sNum.c_str());
						else
							uiNum = 0;
						
						if(!bFirst)
						{
							stData.iFowwees = uiNum;
							bFirst = true;
						}
						else
						{
							stData.iFowwers = uiNum;
						}
					}
					
					if(html_dispose_get_other_attr_second(itTemp,"a","class","item","href",sTemp) == 1)
					{
						if(!bItem)
						{
							sTemp = replace_all(sTemp,"/people/","");
							sTemp = replace_all(sTemp,"/followees","");
							
							cout<<"ename:"<<sTemp<<endl;
							
							stData.sEName = sTemp;
							
							bItem = true;
						}
					}
				}
			}
		}
		
		if(html_dispose_get_attr_second(it,"button","data-id",sTemp) == 1)
		{
			cout<<"data-id:"<<sTemp<<endl;
			stData.sHid = sTemp;
		}
	}
	
	return 0;
}

int dispose_zhihu_get_people_about_html(const string& strPullPath,SUSERINFODATA &stData)
{
	string sUrl = "";
	string sTemp = "";
	string sInfo = "";
	string sDataID = "";
	string sXsrf = "";
	bool bFirst = false;
	string sNum = "";
	unsigned uiNum = 0;
	bool bItem = false;
	bool bAvatar = false;
	int iItem = 0;
	int iGrcjItem = 0;

	if(get_file_info(strPullPath,sInfo) < 0)
		return -1;
	
	if(sInfo.find("</html>")==string::npos)  
	{		
		cout<<"data only part"<<endl;
		return -1;
	}
	
	HTML::ParserDom parser;
	tree<HTML::Node>::iterator it;
	tree<HTML::Node>::iterator end;
	tree<HTML::Node>::iterator itTemp;
	tree<HTML::Node>::iterator endTemp;
	tree<HTML::Node>::iterator tempTemp;
	tree<HTML::Node> dom;
	std::pair<bool, string> pAttr;

	dom = parser.parseTree(sInfo);
	
	it= dom.begin();
	end= dom.end();
	
	for(; it != end; ++it)
	{			
		if (it->isTag() && !strcasecmp(it->tagName().c_str(),"div"))
		{
			it->parseAttributes();
			pAttr = it->attribute("class");
			
			if(pAttr.first && !strcasecmp(pAttr.second.c_str(),"zm-profile-side-following zg-clear"))
			{
				itTemp= dom.begin(it);
				endTemp= dom.end(it);
	
				for(; itTemp != endTemp; ++itTemp)
				{		
					if (itTemp->isTag() && !strcasecmp(itTemp->tagName().c_str(),"strong"))
					{
						tempTemp = dom.begin(itTemp);
						cout<<"guanzhu:"<<tempTemp->text()<<endl;
						
						sNum = tempTemp->text();
						
						if(!sNum.empty())
							uiNum = atoi(sNum.c_str());
						else
							uiNum = 0;
						
						if(!bFirst)
						{
							stData.iFowwees = uiNum;
							bFirst = true;
						}
						else
						{
							stData.iFowwers = uiNum;
						}
					}
					
					if(html_dispose_get_other_attr_second(itTemp,"a","class","item","href",sTemp) == 1)
					{
						if(!bItem)
						{
							sTemp = replace_all(sTemp,"/people/","");
							sTemp = replace_all(sTemp,"/followees","");
							
							cout<<"ename:"<<sTemp<<endl;
							
							stData.sEName = sTemp;
							
							bItem = true;
						}
					}
				}
			}
			else if(pAttr.first && !strcasecmp(pAttr.second.c_str(),"profile-navbar clearfix"))
			{
				itTemp= dom.begin(it);
				endTemp= dom.end(it);
	
				for(; itTemp != endTemp; ++itTemp)
				{		
					if(html_dispose_get_attr_next_text(itTemp,"span","class","num",sTemp) == 1)
					{
						cout<<"num:"<<sTemp<<endl;
						
						if(!sTemp.empty())
							uiNum = atoi(sTemp.c_str());
						else
							uiNum = 0;
	
						if(iItem == 0)
						{
							stData.iAsksNum = uiNum;
						}
						else if(iItem == 1)
						{
							stData.iAnswersNum = uiNum;
						}
						else if(iItem == 2)
						{
							stData.iPostsNum = uiNum;
						}
						else if(iItem == 3)
						{
							stData.iCollectionsNum = uiNum;
						}
						else if(iItem == 4)
						{
							stData.iPublicEditorNum = uiNum;
						}
						
						iItem++;
					}
				}
			}
			else if(pAttr.first && !strcasecmp(pAttr.second.c_str(),"zm-profile-module zm-profile-details-reputation"))
			{
				itTemp= dom.begin(it);
				endTemp= dom.end(it);
	
				for(; itTemp != endTemp; ++itTemp)
				{		
					if (itTemp->isTag() && !strcasecmp(itTemp->tagName().c_str(),"strong"))
					{
						tempTemp = dom.begin(itTemp);
						cout<<"grcj:"<<tempTemp->text()<<endl;
						
						sNum = tempTemp->text();
						
						if(!sNum.empty())
							uiNum = atoi(sNum.c_str());
						else
							uiNum = 0;
	
						if(iGrcjItem == 0)
						{
							stData.iAgreeNum = uiNum;
						}
						else if(iGrcjItem == 1)
						{
							stData.iThankNum = uiNum;
						}
						else if(iGrcjItem == 2)
						{
							stData.iAchCollectNum = uiNum;
						}
						else if(iGrcjItem == 3)
						{
							stData.iShareNum = uiNum;
						}
						
						iGrcjItem++;
					}
				}
			}
		}
		else if (it->isTag() && !strcasecmp(it->tagName().c_str(),"span"))
		{
			it->parseAttributes();
			pAttr = it->attribute("class");
			
			if(pAttr.first && !strcasecmp(pAttr.second.c_str(),"item gender"))
			{
				itTemp= dom.begin(it);
				endTemp= dom.end(it);
	
				for(; itTemp != endTemp; ++itTemp)
				{		
					if(html_dispose_get_attr_second(itTemp,"i","class",sTemp) == 1)
					{
						if(sTemp == "icon icon-profile-male")
						{
							cout<<"man"<<endl;
							stData.sSex = "0";
						}
						else
						{
							cout<<"women"<<endl;
							stData.sSex = "1";
						}
					}
				}
			}
		}
		
		if(html_dispose_get_attr_second(it,"button","data-id",sTemp) == 1)
		{
			cout<<"data-id:"<<sTemp<<endl;
			stData.sHid = sTemp;
		}
	
		if(html_dispose_get_other_attr_second(it,"span","class","location item","title",sTemp) == 1)
		{
			cout<<"location:"<<sTemp<<endl;
			stData.sPlace = sTemp;
		}
		
		if(html_dispose_get_other_attr_second(it,"span","class","education item","title",sTemp) == 1)
		{
			cout<<"education:"<<sTemp<<endl;
			stData.sSchool = sTemp;
		}
		
		if(html_dispose_get_other_attr_second(it,"span","class","education-extra item","title",sTemp) == 1)
		{
			cout<<"Major:"<<sTemp<<endl;
			stData.sMajor = sTemp;
		}
		
		if(html_dispose_get_other_attr_second(it,"span","class","position item","title",sTemp) == 1)
		{
			cout<<"position:"<<sTemp<<endl;
			stData.sVocation = sTemp;
		}
		
		if(html_dispose_get_other_attr_second(it,"span","class","business item","title",sTemp) == 1)
		{
			cout<<"Skill:"<<sTemp<<endl;
			stData.sSkill = sTemp;
		}
		
		if(html_dispose_get_attr_next_text(it,"textarea","name","description",sTemp) == 1)
		{
			cout<<"description:"<<sTemp<<endl;
			stData.sIntroduce = sTemp;
		}
		
		if(!bAvatar)
		{
			if(html_dispose_get_other_attr_second(it,"img","class","Avatar Avatar--l","src",sTemp) == 1)
			{
				cout<<"icon_url:"<<sTemp<<endl;
				
				bAvatar = true;
				
				stData.sIconUrl = sTemp;
			}
			
			if(html_dispose_get_other_attr_second(it,"img","class","Avatar Avatar--l","alt",sTemp) == 1)
			{
				cout<<"zname:"<<sTemp<<endl;
				
				bAvatar = true;
				
				stData.sZHname = sTemp;
			}
		}
	}
	
	return 0;
}

int dispose_zhihu_asks_html(const string& strPullPath)
{
	string sUrl = "";
	string sTemp = "";
	string sInfo = "";

	if(get_file_info(strPullPath,sInfo) < 0)
		return -1; 
			
	if(sInfo.find("</html>")==string::npos)  
	{		
		cout<<"data only part"<<endl;
		return -1;
	}
	
	HTML::ParserDom parser;
	tree<HTML::Node>::iterator it;
	tree<HTML::Node>::iterator end;
	tree<HTML::Node>::iterator itTemp;
	tree<HTML::Node>::iterator endTemp;
	tree<HTML::Node>::iterator nextNode;
	tree<HTML::Node> dom;
	std::pair<bool, string> pAttr;

	dom = parser.parseTree(sInfo);
	
	it= dom.begin();
	end= dom.end();
	
	for(; it != end; ++it)
	{		
		if(html_dispose_get_other_attr_second(it,"a","class","question_link","href",sUrl) == 1)
		{
			sUrl = ZHIHU_URL+sUrl;
			
			cout<<"ask url:"<<sUrl<<endl;
			
			itTemp = dom.begin(it);
			
			sTemp = itTemp->text();
			
			sTemp = replace_all(sTemp,"\n","");
			sTemp = trim(sTemp);
				
			cout<<"ask question:"<<sTemp<<endl;
		}
		
		if(html_dispose_get_attr_next_text(it,"div","class","zm-profile-vote-num",sTemp) == 1)
		{
			cout<<"ask look:"<<sTemp<<endl;
		}
		
		if (it->isTag() && !strcasecmp(it->tagName().c_str(),"div"))
		{
			it->parseAttributes();
			pAttr = it->attribute("class");
			if(pAttr.first && !strcasecmp(pAttr.second.c_str(),"meta zg-gray"))
			{
				itTemp= dom.begin(it);
				endTemp= dom.end(it);
	
				for(; itTemp != endTemp; ++itTemp)
				{		
					if (itTemp->isTag() && !strcasecmp(itTemp->tagName().c_str(),"span"))
					{
						itTemp->parseAttributes();
						pAttr = itTemp->attribute("class");
						
						if(pAttr.first && !strcasecmp(pAttr.second.c_str(),"zg-bull"))
						{
							nextNode = dom.begin(itTemp);
							nextNode = dom.begin(nextNode);
					
							sTemp = nextNode->text();     
							sTemp = trim(sTemp);
							
							sTemp = replace_all(sTemp,"\n","");
							sTemp = replace_all(sTemp," ","");
							
							cout<<"ask huida:"<<sTemp<<endl;
						}
					}
				}
			}
		}
	}
					
	return 0;
}

int dispose_zhihu_answers_html(const string& strPullPath)
{
	string sUrl = "";
	string sTemp = "";
	string sInfo = "";

	if(get_file_info(strPullPath,sInfo) < 0)
		return -1;
	
	if(sInfo.find("</html>")==string::npos)  
	{		
		cout<<"data only part"<<endl;
		return -1;
	}
	
	HTML::ParserDom parser;
	tree<HTML::Node>::iterator it;
	tree<HTML::Node>::iterator end;
	tree<HTML::Node>::iterator itTemp;
	tree<HTML::Node> dom;

	dom = parser.parseTree(sInfo);
	
	it= dom.begin();
	end= dom.end();
	
	for(; it != end; ++it)
	{		
		if(html_dispose_get_other_attr_second(it,"a","class","question_link","href",sUrl) == 1)
		{
			sUrl = ZHIHU_URL+sUrl;
			
			cout<<"ask url:"<<sUrl<<endl;
			
			itTemp = dom.begin(it);
				
			cout<<"ask question:"<<itTemp->text()<<endl;
		}
		
		if(html_dispose_get_attr_next_text(it,"span","class","count",sTemp) == 1)
		{
			cout<<"answers up:"<<sTemp<<endl;
		}
	}
	
	return 0;
}

int dispose_zhihu_posts_html(const string& strPullPath)
{
	string sUrl = "";
	string sTemp = "";
	string sInfo = "";

	if(get_file_info(strPullPath,sInfo) < 0)
		return -1;
	
	if(sInfo.find("</html>")==string::npos)  
	{		
		cout<<"data only part"<<endl;
		return -1;
	}
	
	HTML::ParserDom parser;
	tree<HTML::Node>::iterator it;
	tree<HTML::Node>::iterator end;
	tree<HTML::Node>::iterator itTemp;
	tree<HTML::Node> dom;

	dom = parser.parseTree(sInfo);
	
	it= dom.begin();
	end= dom.end();
	
	for(; it != end; ++it)
	{			
		if(html_dispose_get_other_attr_second(it,"a","class","post-link","href",sUrl) == 1)
		{
			cout<<"posts url:"<<sUrl<<endl;
			
			itTemp = dom.begin(it);
				
			cout<<"posts question:"<<itTemp->text()<<endl;
		}
		
		if(html_dispose_get_attr_next_text(it,"span","class","count",sTemp) == 1)
		{
			cout<<"posts up:"<<sTemp<<endl;
		}
	}
	
	return 0;
}

int dispose_zhihu_collections_html(const string& strPullPath)
{
	string sUrl = "";
	string sTemp = "";
	string sInfo = "";

	if(get_file_info(strPullPath,sInfo) < 0)
		return -1;
	
	if(sInfo.find("</html>")==string::npos)  
	{		
		cout<<"data only part"<<endl;
		return -1;
	}
	
	HTML::ParserDom parser;
	tree<HTML::Node>::iterator it;
	tree<HTML::Node>::iterator end;
	tree<HTML::Node>::iterator itTemp;
	tree<HTML::Node>::iterator endTemp;
	tree<HTML::Node>::iterator nextNode;
	tree<HTML::Node> dom;
	std::pair<bool, string> pAttr;

	dom = parser.parseTree(sInfo);
	
	it= dom.begin();
	end= dom.end();
	
	for(; it != end; ++it)
	{	
		if(html_dispose_get_other_attr_second(it,"a","class","zm-profile-fav-item-title","href",sUrl) == 1)
		{
			sUrl = ZHIHU_URL+sUrl;
			
			cout<<"collections url:"<<sUrl<<endl;
			
			itTemp = dom.begin(it);
				
			cout<<"collections title:"<<itTemp->text()<<endl;
		}
		
		if (it->isTag() && !strcasecmp(it->tagName().c_str(),"div"))
		{
			it->parseAttributes();
			pAttr = it->attribute("class");
			
			if(pAttr.first && !strcasecmp(pAttr.second.c_str(),"zm-profile-fav-question-item"))
			{
				itTemp= dom.begin(it);
				endTemp= dom.end(it);
				
				for(; itTemp != endTemp; ++itTemp)
				{		
					if(html_dispose_get_other_attr_second(itTemp,"a","class","zg-link-litblue","href",sUrl) == 1)
					{
						sUrl = ZHIHU_URL+sUrl;
						
						cout<<"collections item url:"<<sUrl<<endl;
						
						nextNode = dom.begin(itTemp);
							
						cout<<"collections item title:"<<nextNode->text()<<endl;
					}
				}
			}
		}
	}
	
	return 0;
}

int dispose_zhihu_zhuanlan_small_html(const string& strPullPath)
{
	string sUrl = "";
	string sTemp = "";
	string sInfo = "";

	if(get_file_info(strPullPath,sInfo) < 0)
		return -1;
	
	if(sInfo.find("</html>")==string::npos)  
	{		
		cout<<"data only part"<<endl;
		return -1;
	}
	
	HTML::ParserDom parser;
	tree<HTML::Node>::iterator it;
	tree<HTML::Node>::iterator end;
	tree<HTML::Node>::iterator itTemp;
	tree<HTML::Node>::iterator endTemp;
	tree<HTML::Node> dom;
	std::pair<bool, string> pAttr;

	dom = parser.parseTree(sInfo);
	
	it= dom.begin();
	end= dom.end();
	
	for(; it != end; ++it)
	{	
		if (it->isTag() && !strcasecmp(it->tagName().c_str(),"div"))
		{
			it->parseAttributes();
			pAttr = it->attribute("class");
			
			if(pAttr.first && !strcasecmp(pAttr.second.c_str(),"zm-profile-side-columns"))
			{
				itTemp= dom.begin(it);
				endTemp= dom.end(it);
				
				for(; itTemp != endTemp; ++itTemp)
				{		
					if(html_dispose_get_attr_second(itTemp,"a","href",sTemp) == 1)
					{
						cout<<"zhuanglan url:"<<sTemp<<endl;
					}
					
					if(html_dispose_get_attr_second(itTemp,"img","src",sTemp) == 1)
					{
						cout<<"zhuanglan img:"<<sTemp<<endl;
					}
					
					if(html_dispose_get_attr_second(itTemp,"img","alt",sTemp) == 1)
					{
						cout<<"zhuanglan title:"<<sTemp<<endl;
					}
				}
			}
			else if(pAttr.first && !strcasecmp(pAttr.second.c_str(),"zm-profile-side-topics"))
			{
				itTemp= dom.begin(it);
				endTemp= dom.end(it);
				
				for(; itTemp != endTemp; ++itTemp)
				{		
					if(html_dispose_get_attr_second(itTemp,"a","href",sTemp) == 1)
					{
						sTemp = ZHIHU_URL+sTemp;
						cout<<"topic url:"<<sTemp<<endl;
					}
					
					if(html_dispose_get_attr_second(itTemp,"img","src",sTemp) == 1)
					{
						cout<<"topic img:"<<sTemp<<endl;
					}
					
					if(html_dispose_get_attr_second(itTemp,"img","alt",sTemp) == 1)
					{
						cout<<"topic title:"<<sTemp<<endl;
					}
				}
			}
		}
	}
	
	return 0;
}

int dispose_zhihu_zhuanlan_html(const string& strPullPath)
{
	string sUrl = "";
	string sTemp = "";
	string sInfo = "";

	if(get_file_info(strPullPath,sInfo) < 0)
		return -1;
	
	if(sInfo.find("</html>")==string::npos)  
	{		
		cout<<"data only part"<<endl;
		return -1;
	}
	
	HTML::ParserDom parser;
	tree<HTML::Node>::iterator it;
	tree<HTML::Node>::iterator end;
	tree<HTML::Node>::iterator itTemp;
	tree<HTML::Node>::iterator endTemp;
	tree<HTML::Node>::iterator nextNode;
	tree<HTML::Node> dom;
	std::pair<bool, string> pAttr;

	dom = parser.parseTree(sInfo);
	
	it= dom.begin();
	end= dom.end();
	
	for(; it != end; ++it)
	{	
		if (it->isTag() && !strcasecmp(it->tagName().c_str(),"div"))
		{
			it->parseAttributes();
			pAttr = it->attribute("class");
			
			if(pAttr.first && !strcasecmp(pAttr.second.c_str(),"zm-profile-section-item zg-clear"))
			{
				itTemp= dom.begin(it);
				endTemp= dom.end(it);
				
				for(; itTemp != endTemp; ++itTemp)
				{		
					if(html_dispose_get_attr_next_text(itTemp,"div","class","description",sTemp) == 1)
					{
						cout<<"zhuanglan description:"<<sTemp<<endl;
					}
					
					if(html_dispose_get_attr_second(itTemp,"img","src",sTemp) == 1)
					{
						cout<<"zhuanglan img:"<<sTemp<<endl;
					}
					
					if(html_dispose_get_attr_next_text(itTemp,"span","class","zg-gray",sTemp) == 1)
					{
						cout<<"zhuanglan counts:"<<sTemp<<endl;
					}
		
					 if (itTemp->isTag() && !strcasecmp(itTemp->tagName().c_str(),"a"))
					{
						itTemp->parseAttributes();
						
						pAttr = itTemp->attribute("class");
						if(pAttr.first && !strcasecmp(pAttr.second.c_str(),"zm-list-avatar-link"))
						{
							continue;
						}
						
						pAttr = itTemp->attribute("href");
				
						if(pAttr.first)
						{
							sTemp = pAttr.second;
							
							cout<<"zhuanglan url:"<<sTemp<<endl;
						}
						
						nextNode = dom.begin(itTemp);
						nextNode = dom.begin(nextNode);
						
						cout<<"zhuanglan name:"<<nextNode->text()<<endl;
					}
				}
			}
		}
	}
	
	return 0;
}

int dispose_zhihu_topics_html(const string& strPullPath)
{
	string sUrl = "";
	string sTemp = "";
	string sInfo = "";

	if(get_file_info(strPullPath,sInfo) < 0)
		return -1;
	
	if(sInfo.find("</html>")==string::npos)  
	{		
		cout<<"data only part"<<endl;
		return -1;
	}
	
	HTML::ParserDom parser;
	tree<HTML::Node>::iterator it;
	tree<HTML::Node>::iterator end;
	tree<HTML::Node>::iterator itTemp;
	tree<HTML::Node>::iterator endTemp;
	tree<HTML::Node>::iterator nextNode;
	tree<HTML::Node> dom;
	std::pair<bool, string> pAttr;

	dom = parser.parseTree(sInfo);
	
	it= dom.begin();
	end= dom.end();
	
	for(; it != end; ++it)
	{	
		if (it->isTag() && !strcasecmp(it->tagName().c_str(),"div"))
		{
			it->parseAttributes();
			pAttr = it->attribute("class");
			
			if(pAttr.first && !strcasecmp(pAttr.second.c_str(),"zm-profile-section-main"))
			{
				itTemp= dom.begin(it);
				endTemp= dom.end(it);
				
				for(; itTemp != endTemp; ++itTemp)
				{		
					if (itTemp->isTag() && !strcasecmp(itTemp->tagName().c_str(),"a"))
					{
						itTemp->parseAttributes();
						pAttr = itTemp->attribute("class");
				
						if(pAttr.first && !strcasecmp(pAttr.second.c_str(),"zg-link-gray"))
						{
							continue;
						}
				
						pAttr = itTemp->attribute("href");
						if(pAttr.first)
						{
							if(!strcasecmp(pAttr.second.c_str(),"javascript:;"))
								continue;
							
							sTemp = ZHIHU_URL;
							sTemp = sTemp+pAttr.second;
							
							cout<<"topics url:"<<sTemp<<endl;
						
							nextNode = dom.begin(itTemp);
							nextNode = dom.begin(nextNode);
							
							cout<<"topics name:"<<nextNode->text()<<endl;
						}
					}
				}
			}
		}
	}
	
	return 0;
}

int dispose_zhihu_monthly_hot_html(const string& strPullPath)
{
	string sUrl = "";
	string sInfo = "";
	string sTempInfo = "";

	if(get_file_info(strPullPath,sInfo) < 0)
		return -1;
	
	HTML::ParserDom parser;
	tree<HTML::Node>::iterator it;
	tree<HTML::Node>::iterator end;
	tree<HTML::Node>::iterator itTemp;
	tree<HTML::Node> dom;

	dom = parser.parseTree(sInfo);
	
	it= dom.begin();
	end= dom.end();
	
	for(; it != end; ++it)
	{		
		if(html_dispose_get_other_attr_second(it,"a","class","question_link","href",sUrl) == 1)
		{
			sUrl = ZHIHU_URL+sUrl;
			
			cout<<"url:"<<sUrl<<endl;
			
			itTemp= dom.begin(it);
				
			cout<<"question:"<<itTemp->text()<<endl;
		}
		
		if(html_dispose_get_attr_second(it,"a","data-votecount",sTempInfo) == 1)
		{
			cout<<"suppor:"<<sTempInfo<<endl;
		}
		
		if(html_dispose_get_attr_next_text(it,"i","class","z-icon-comment",sTempInfo) == 1)
		{
			cout<<"comment:"<<sTempInfo<<endl;
		}
	}
	
	return 0;
}

int circle_dispose_e_name(const string &strFileDir)
{
	int i = 0;
	string sTName = "";
	string sTPeople = aZhihuPeople[0];
	string sPullPath = "";
	char cIdx[20];
	string sIdx = "";
	string sUrl = "";
	int iSize = iFowEname;
	
	for(i = 0;i<iSize;i++)
	{
		sTName = saFowEname[i];
		
		sTName = ZHIHU_PEOPLE+sTName+sTPeople;
		
		cout<<"nono:"<<sTName<<endl;
		
		getCurlZhiHuUrl(sTName,sUrl,strFileDir);
		
		sprintf(cIdx,"%d",i);
		sIdx = cIdx;
				
		sPullPath = g_strFileDir+PEOPLE_DIR+"zhihu_follow_"+sIdx+".txt";
					
		pullURL(sUrl,sPullPath);
		
		dispose_zhihu_follow_html(sPullPath,0);
	}
	
	return 0;
}

int print_e_name()
{
	int i = 0;
	string sTName = "";
	cout<<endl;
	cout<<endl;
	cout<<"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"<<endl;
	cout<<"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"<<endl;
	
	for(i = 0;i<iFowEname;i++)
	{
		sTName = saFowEname[i];
		
		cout<<"all name["<<i<<"]:"<<sTName<<endl;
	}
	
	return 0;
}

int dispose_zhihu_profileFolloweesList(const string& strPullPath)
{
	string sUrl = "";
	string sInfo = "";
	string sTempInfo = "";

	if(get_file_info(strPullPath,sInfo) < 0)
		return -1;
	
	HTML::ParserDom parser;
	tree<HTML::Node>::iterator it;
	tree<HTML::Node>::iterator end;
	tree<HTML::Node>::iterator itTemp;
	tree<HTML::Node> dom;

	dom = parser.parseTree(sInfo);
	
	it= dom.begin();
	end= dom.end();
	
	for(; it != end; ++it)
	{		
		if(html_dispose_get_other_attr_second(it,"a","class","question_link","href",sUrl) == 1)
		{
			sUrl = ZHIHU_URL+sUrl;
			
			cout<<"url:"<<sUrl<<endl;
			
			itTemp= dom.begin(it);
				
			cout<<"question:"<<itTemp->text()<<endl;
		}
		
		if(html_dispose_get_attr_second(it,"a","data-votecount",sTempInfo) == 1)
		{
			cout<<"suppor:"<<sTempInfo<<endl;
		}
		
		if(html_dispose_get_attr_next_text(it,"i","class","z-icon-comment",sTempInfo) == 1)
		{
			cout<<"comment:"<<sTempInfo<<endl;
		}
	}
	
	
	return 0;
}

int dispose_all_follow(const string &strFileDir)
{	
	int i = 0;
	int iOffset = 0;
	string sUrl = "";
	string strFowUrl = "";
	char cIdx[20];
	string sIdx = "";
	string sPullPath = "";
	
	for(i = 0;i<10;i++)
	{
		iOffset = i*PAGE_NUMS;
		
		sprintf(cIdx,"%d",iOffset);
		sIdx = cIdx;
		
		strFowUrl = ZHIHU_NODE_1 + sIdx + g_strFowUrlBak + ZHIHU_NODE_3;
		strFowUrl = "\'" + strFowUrl + "\'";
		
		getCurlZhiHuUrl(strFowUrl,sUrl,strFileDir);
				
		sPullPath = g_strFileDir+FOWWEE_DIR+"zhihu_follow_all_"+sIdx+".txt";
					
		pullURL(sUrl,sPullPath);
		
		dispose_zhihu_follow_html(sPullPath,1);
	}
}

int init_wp_posts(TWPPOSTS &szData,string &sContent,int iType,int iOffset)
{
	string sNowTime = "";
	string sNowYMD = "";
	string sGuid = "http://www.smimi.net/?page_id=";
	
	getOffsetTm(sNowTime,1,iOffset);
	getOffsettmYMD(sNowYMD,1,iOffset);
	
	string sTmpContent = sContent;
	
	sTmpContent = replace_all(sTmpContent,"\"","guaibyco");
	sTmpContent = replace_all(sTmpContent,"guaibyco","\\\"");
			
	cout<<sTmpContent<<endl;
	
	szData.uiPostAuthor = 1;
	szData.sPostDate = sNowTime;
	szData.sPostDateGmt = sNowTime;
	szData.sPostContent = sTmpContent;
	if(iType == 1)
		szData.sPostTitle = sNowYMD+" 今日推荐";
	else
		szData.sPostTitle = sNowYMD+" 今日最热";
	szData.sPostExcerpt = "";
	szData.sPostStatus = "publish";
	szData.sCommentStatus = "open";
	szData.sPingStatus = "open";
	szData.sPostPassWd = "";
	szData.sPostName = "byco";
	szData.sToPing= "";
	szData.sPinged="";
	szData.sPostModified = sNowTime;
	szData.sPostModifiedGmt= sNowTime;
	szData.sPostContFiltered = "";
	szData.uiPostParent = 0;
	szData.sGuid = sGuid+sNowTime;
	szData.iMenuOrder = 0;
	szData.sPostType = "post";
	szData.sPostMimeType = "";
	szData.uiCommentCount = 0;
	
	return 0;
}

int dispose_all_people_follow(MYSQL *pmysql,const string& strFileDir,const SUSERINFODATA &stData,int iType)
{	
	int i = 0;
	int iOffset = 0;
	string sUrl = "";
	string strFowUrl = "";
	string sIdx = "";
	string sPullPath = "";
	string sHid = stData.sHid;
	int iFollowCount = 0;
	string sNode1 = "";
	string sNode2 = "";
	string sNode3 = "";
	string sDir = "";
	vector<SUSERINFODATA> v_stUserData;
	string strErrPath = "";
	bool bErrFile = false;
	ofstream in;
	string sFileName = "";
	string sErrCmd = "";
	int iErrCount = 0;
	map<string,TUSERHASH> m_stUserHash;
	int iCount = 0;
	
	if(iType == 0)//关注了
	{
		iFollowCount = stData.iFowwees;
		sNode1 = ZHIHU_NODE_1;
		sNode2 = ZHIHU_NODE_2;
		sNode3 = ZHIHU_NODE_3;
		sDir = FOWWEE_DIR;
	}
	else//粉丝
	{
		iFollowCount = stData.iFowwers;
		sNode1 = ZHIHU_FOLLOWERS_NODE_1;
		sNode2 = ZHIHU_FOLLOWERS_NODE_2;
		sNode3 = ZHIHU_FOLLOWERS_NODE_3;
		sDir = FOWWER_DIR;
	}
	
	if(iFollowCount <= 0)
		return -1;
	
	if(get_zhihu_file_name(19,sFileName))
	{
		strErrPath = strFileDir+sFileName+TXT_SUFFIX;
		
		if(!strErrPath.empty())
		{
			if(!get_file_stream(strErrPath,in))
			{
				bErrFile = true;
			}
		}
	}
	
	int iPage = ceil(iFollowCount/PAGE_NUMS);

	string strFowUrlBak = sNode2 + sHid;
	
	for(i = 0;i<=iPage;i++)
	{
		iOffset = i*PAGE_NUMS;
		
		IntToString(sIdx,iOffset);
		
		strFowUrl = sNode1 + sIdx + strFowUrlBak + sNode3;
		strFowUrl = "\'" + strFowUrl + "\'";
		
		getCurlZhiHuUrl(strFowUrl,sUrl,strFileDir);
				
		sPullPath = g_strFileDir+sDir+"follow_"+sIdx+".txt";
					
		pullURL(sUrl,sPullPath);
		
		v_stUserData.clear();
		
		if(dispose_zhihu_get_follow_brief_html(sPullPath,v_stUserData) < 0)
		{
			iErrCount++;
			
			if(bErrFile)
			{
				sErrCmd = sUrl + "###" + sPullPath + "|||";
				
				in<<sErrCmd<<endl;
			}
			
			if(iErrCount >= 3)
				break;
		}
		else
		{
			insert_redis_ready_pull_queue(v_stUserData);
			
			userdata_pkg_userhash(v_stUserData,m_stUserHash);
		
			iCount++;
			
			if(iCount >= 25)
			{
				iCount = 0;
				
				mysql_insert_t_user_hash(pmysql,m_stUserHash);
				
				m_stUserHash.clear();
			}
		}
	}
	
	mysql_insert_t_user_hash(pmysql,m_stUserHash);
	
	if(bErrFile)
		in.close();
	
	return 0;
}

int userdata_pkg_userhash(vector<SUSERINFODATA> &v_stUserData,map<string,TUSERHASH> &m_stUserHash)
{
	if(v_stUserData.empty())
		return 0;
	
	SUSERINFODATA stTempUserData;
	TUSERHASH stHashData;

	vector<SUSERINFODATA>::iterator it = v_stUserData.begin();
	for(;it != v_stUserData.end();it++)
	{
		stTempUserData = (SUSERINFODATA)*it;
		
		stHashData.sID = stTempUserData.sHid;
		
		if(stHashData.sID.empty())
			continue;
		
		stHashData.sENname = stTempUserData.sEName;
		stHashData.sZHname = stTempUserData.sZHname;
		
		m_stUserHash.insert(make_pair<string,TUSERHASH>(stHashData.sID,stHashData));
	}
	
	return 0;
}

int userdata_pkg_userInfo(vector<SUSERINFODATA> &v_stUserData,map<string,TUSERINFO> &m_stUserInfo)
{
	if(v_stUserData.empty())
		return 0;
	
	SUSERINFODATA stTempUserData;
	TUSERINFO stInfoData;
	
	vector<SUSERINFODATA>::iterator it = v_stUserData.begin();
	for(;it != v_stUserData.end();it++)
	{
		stTempUserData = (SUSERINFODATA)*it;
		
		stInfoData.sID = stTempUserData.sHid;
		
		if(stInfoData.sID.empty())
			continue;
			
		stInfoData.sENname = stTempUserData.sEName;
		stInfoData.sZHname = stTempUserData.sZHname;
		stInfoData.sIconUrl = stTempUserData.sIconUrl;
		stInfoData.sIntroduce = stTempUserData.sIntroduce;
		stInfoData.sSex = stTempUserData.sSex;
		stInfoData.sPlace = stTempUserData.sPlace;
		stInfoData.sVocation = stTempUserData.sVocation;
		stInfoData.sSchool = stTempUserData.sSchool;
		stInfoData.sMajor = stTempUserData.sMajor;
		stInfoData.sSkill = stTempUserData.sSkill;	
		
		m_stUserInfo.insert(make_pair<string,TUSERINFO>(stInfoData.sID,stInfoData));
	}
	
	return 0;
}

int userdata_pkg_userJoin(vector<SUSERINFODATA> &v_stUserData,map<string,TUSERJOIN> &m_stUserJoin)
{
	if(v_stUserData.empty())
		return 0;
	
	SUSERINFODATA stTempUserData;
	TUSERJOIN stJoinData;
	
	vector<SUSERINFODATA>::iterator it = v_stUserData.begin();
	for(;it != v_stUserData.end();it++)
	{
		stTempUserData = (SUSERINFODATA)*it;
		
		stJoinData.sID = stTempUserData.sHid;
		
		if(stJoinData.sID.empty())
			continue;
		
		stJoinData.sENname = stTempUserData.sEName;
		stJoinData.iAgreeNum = stTempUserData.iAgreeNum;
		stJoinData.iThankNum = stTempUserData.iThankNum;
		stJoinData.iAttentionNum = stTempUserData.iFowwees;
		stJoinData.iFansNum = stTempUserData.iFowwers;
		stJoinData.iAsksNum = stTempUserData.iAsksNum;
		stJoinData.iAnswersNum = stTempUserData.iAnswersNum;
		stJoinData.iPostsNum = stTempUserData.iPostsNum;
		stJoinData.iCollectionsNum = stTempUserData.iCollectionsNum;
		stJoinData.iPublicEditorNum = stTempUserData.iPublicEditorNum;
		stJoinData.iShareNum = stTempUserData.iShareNum;
		stJoinData.iAchCollectNum = stTempUserData.iAchCollectNum;
		
		m_stUserJoin.insert(make_pair<string,TUSERJOIN>(stJoinData.sID,stJoinData));
	}
	
	return 0;
}

int get_redis_should_pull_queue_fowwer(const string& strFileDir,MYSQL *pmysql,map<string,SUSERINFODATA> &m_stPullUserData)
{
	if(m_stPullUserData.empty())
		return 0;
	
	SUSERINFODATA stTempUserData;
	vector<SUSERINFODATA> v_stSucUserData;
	string sPullPath = "";
	string sEName = "";
	string sUrl = "";
	string sIndexUrl = "";
	vector<SUSERINFODATA> v_stErrorUserData;
	map<string,TUSERHASH> m_stUserHash;
	map<string,TUSERINFO> m_stUserInfo;
	map<string,TUSERJOIN> m_stUserJoin;
	string sBout = aZhihuPeople[7];

	map<string,SUSERINFODATA>::iterator it = m_stPullUserData.begin();
	for(;it != m_stPullUserData.end();it++)
	{
		stTempUserData = (SUSERINFODATA)it->second;
		
		sEName = stTempUserData.sEName;
		
		sIndexUrl = ZHIHU_PEOPLE + sEName + sBout;
		
		sIndexUrl = "\'" + sIndexUrl + "\'";
		
		getCurlZhiHuUrl(sIndexUrl,sUrl,strFileDir);
				
		sPullPath = strFileDir+INDEX_PEOPLE_HTML+"zhihu_people_index_"+ sEName +".txt";
					
		pullURL(sUrl,sPullPath);
		
		SUSERINFODATA stData;
		
		if(dispose_zhihu_get_people_about_html(sPullPath,stData) == 0)
		{
			dispose_all_people_follow(pmysql,strFileDir,stData,0);//关注了
					
			dispose_all_people_follow(pmysql,strFileDir,stData,1);//粉丝
			
			v_stSucUserData.push_back(stData);
		}
		else
		{
			v_stErrorUserData.push_back(stTempUserData);
		}
	}
	
	insert_redis_already_pull_hash(v_stSucUserData);
	insert_redis_ready_pull_queue(v_stErrorUserData);
	
	userdata_pkg_userhash(v_stSucUserData,m_stUserHash);
	userdata_pkg_userInfo(v_stSucUserData,m_stUserInfo);
	userdata_pkg_userJoin(v_stSucUserData,m_stUserJoin);
	
	mysql_insert_t_user_hash(pmysql,m_stUserHash);
	mysql_insert_t_user_info(pmysql,m_stUserInfo);
	mysql_insert_t_user_join(pmysql,m_stUserJoin);
	
	return 0;
}

int get_zhihu_sub_topic_by_parent_topic(string sParentTopicID,vector<TSUBTOPIC> &v_stSubTopic)
{
	string sTmpUrl = "";
	string sDataUrl = "";
	int i = 0;
	int iOffset = 0;
	string sOffset = "";
	string sPullPath = "";
	string sUrl = "";
	int iParentTopicID = extract_all_number(sParentTopicID);
	
	for(i = 0;i < ZHIHU_SUB_TOPIC_MAX_PAGES;i++)
	{
		iOffset = i * PAGE_NUMS;
		
		IntToString(sOffset,iOffset); 
		
		sPullPath = g_strFileDir+SUB_TOPIC+"zhihu_sub_topic_"+sParentTopicID+"_"+sOffset+".txt";
		
		sTmpUrl = ZHIHU_SUB_TOPIC_URL;
		
		sDataUrl = ZHIHU_SUB_TOPIC_NODE_1;
		
		sDataUrl = sDataUrl + sParentTopicID + ZHIHU_SUB_TOPIC_NODE_2 + sOffset + ZHIHU_SUB_TOPIC_NODE_3 + ZHIHU_SUB_TOPIC_NODE_4;
		
		sTmpUrl = "\'" + sTmpUrl + "\'";
		sDataUrl = "\'" + sDataUrl + "\'";
		
		getCurlZhiHuSubTopicsUrl2(sTmpUrl,sUrl,sDataUrl);
		
		pullURL(sUrl,sPullPath);
	
		if(dispose_zhihu_sub_topic_json(iParentTopicID,sPullPath,v_stSubTopic) < 0)
			break;
	}
	
	return 0;
}

int get_all_sub_topic_info(vector<string> &v_stTopics,vector<TSUBTOPIC> &v_stSubTopic)
{
	string sTopicID = "";
	vector<string>::iterator it;
	
	if(!v_stTopics.empty())
	{
		it = v_stTopics.begin();
		for(;it!=v_stTopics.end();it++)
		{
			sTopicID = (string)*it;
			
			get_zhihu_sub_topic_by_parent_topic(sTopicID,v_stSubTopic);
		}
	}
	
	return 0;
}

int dispose_fowweer_info(MYSQL *pmysql,map<string,string> &g_mErrFowwInfo)
{
	if(g_mErrFowwInfo.empty())
		return 0;
	
	vector<SUSERINFODATA> v_stUserData;
	string sUrl = "";
	string sPullPath = "";
	map<string,TUSERHASH> m_stUserHash;
	int iCount = 0;
	
	map<string,string>::iterator it = g_mErrFowwInfo.begin();
	for(;it != g_mErrFowwInfo.end();it++)
	{
		sUrl = (string)it->first;
		sPullPath = (string)it->second;
		
		if(sUrl.empty() || sPullPath.empty())
			continue;
		
		pullURL(sUrl,sPullPath);
		
		v_stUserData.clear();
		
		if(dispose_zhihu_get_follow_brief_html(sPullPath,v_stUserData) == 0)
		{
			insert_redis_ready_pull_queue(v_stUserData);
			
			userdata_pkg_userhash(v_stUserData,m_stUserHash);
		
			iCount++;
			
			if(iCount >= 25)
			{
				iCount = 0;
				
				mysql_insert_t_user_hash(pmysql,m_stUserHash);
				
				m_stUserHash.clear();
			}
		}
	}
	
	mysql_insert_t_user_hash(pmysql,m_stUserHash);
	
	return 0;
}

int update_zhihu_user_hash_join_info(MYSQL *pmysql,const string &strFileDir,vector<string> &v_stUserHash)
{
	if(v_stUserHash.empty())
		return 0;
	
	string sEnName = "";
	vector<SUSERINFODATA> v_stSucUserData;
	string sPullPath = "";
	string sUrl = "";
	string sIndexUrl = "";
	map<string,TUSERHASH> m_stUserHash;
	map<string,TUSERINFO> m_stUserInfo;
	map<string,TUSERJOIN> m_stUserJoin;
	string sBout = aZhihuPeople[7];
	vector<string> v_stUpdateUserHash;
	
	vector<string>::iterator it = v_stUserHash.begin();
	for(;it!=v_stUserHash.end();it++)
	{
		sEnName = (string)*it;
		
		if(sEnName.empty())
			continue;
		
		sIndexUrl = ZHIHU_PEOPLE + sEnName + sBout;
		
		sIndexUrl = "\'" + sIndexUrl + "\'";
		
		getCurlZhiHuUrl(sIndexUrl,sUrl,strFileDir);
				
		sPullPath = strFileDir+INDEX_PEOPLE_HTML+"zhihu_people_index_"+ sEnName +".txt";
					
		pullURL(sUrl,sPullPath);
		
		SUSERINFODATA stData;
		
		if(dispose_zhihu_get_people_about_html(sPullPath,stData) == 0)
		{			
			v_stSucUserData.push_back(stData);
		}
		else
		{
			v_stUpdateUserHash.push_back(sEnName);
		}
	}
	
	userdata_pkg_userhash(v_stSucUserData,m_stUserHash);
	userdata_pkg_userInfo(v_stSucUserData,m_stUserInfo);
	userdata_pkg_userJoin(v_stSucUserData,m_stUserJoin);

	mysql_insert_t_user_hash(pmysql,m_stUserHash);
	mysql_insert_t_user_info(pmysql,m_stUserInfo);
	mysql_insert_t_user_join(pmysql,m_stUserJoin);
	
	mysql_update_time(pmysql,v_stUpdateUserHash);
	
	return 0;
}

int dispose_sub_topic_top_answers(string &sPullPath,vector<TSUBTOPICTOPANSWERS> &v_TopAnswers,int iType,string &sSubID)
{
	TSUBTOPICTOPANSWERS stData;
	string sUrl = "";
	string sTemp = "";
	string sInfo = "";
	string sQuestionID = "";
	string sQuestionInfo = "";
	string sAbsInfo = "";
	string::size_type pos(0);   
	bool bImg = false;

	if(get_file_info(sPullPath,sInfo) < 0)
		return -1;
	
	if(sInfo.find("</html>")==string::npos)  
	{		
		cout<<"data only part"<<endl;
		return -1;
	}
	
	HTML::ParserDom parser;
	tree<HTML::Node>::iterator it;
	tree<HTML::Node>::iterator end;
	tree<HTML::Node>::iterator itTemp;
	tree<HTML::Node>::iterator endTemp;
	tree<HTML::Node>::iterator tempTemp;
	tree<HTML::Node> dom;
	std::pair<bool, string> pAttr;

	dom = parser.parseTree(sInfo);
	
	it= dom.begin();
	end= dom.end();
	
	for(; it != end; ++it)
	{			
		if (it->isTag() && !strcasecmp(it->tagName().c_str(),"div"))
		{
			it->parseAttributes();
			pAttr = it->attribute("class");
			if(pAttr.first && !strcasecmp(pAttr.second.c_str(),"zh-summary summary clearfix"))
			{		
				if(iType == 1)
				{
					it->parseAttributes();
					pAttr = it->attribute("style");
					if(!pAttr.first || strcasecmp(pAttr.second.c_str(),"display:none;"))
						continue;
				}
				
				itTemp= dom.begin(it);
				endTemp= dom.end(it);
				
				sAbsInfo = "";
				bImg = false;
	
				for(; itTemp != endTemp; ++itTemp)
				{	
					if(html_dispose_get_other_attr_second(itTemp,"img","class","origin_image inline-img zh-lightbox-thumb","src",sTemp) == 1)
					{
						stData.sAnswerImgUrl = sTemp;
						
						bImg = true;
					}
					else
					{
						sAbsInfo = sAbsInfo + itTemp->text();
					}
				}	
				
				sAbsInfo = replace_all(sAbsInfo,"\n","");
				sAbsInfo = replace_all(sAbsInfo,"显示全部","");
				sAbsInfo = trim(sAbsInfo);
				
				while(sAbsInfo.find("<") != string::npos && sAbsInfo.find(">") != string::npos)
					del_symbol_pair_info(sAbsInfo,"<",">");
				
				sAbsInfo = replace_all(sAbsInfo,"&lt;","<");
				sAbsInfo = replace_all(sAbsInfo,"&gt;",">");
				
				stData.sAnswerAbs = sAbsInfo;
				
				if(!bImg)
					stData.sAnswerImgUrl = "";
				
				v_TopAnswers.push_back(stData);
				
				if(iType == 1)
					break;
			}
		}
	
		if(iType == 1)
		{
			stData.sSubID = sSubID;
			
			if (it->isTag() && !strcasecmp(it->tagName().c_str(),"div"))
			{
				it->parseAttributes();
				pAttr = it->attribute("id");
				if(pAttr.first && !strcasecmp(pAttr.second.c_str(),"zh-question-title"))
				{		
					itTemp= dom.begin(it);
					endTemp= dom.end(it);
		
					for(; itTemp != endTemp; ++itTemp)
					{	
						if(html_dispose_get_attr_second(itTemp,"a","href",sTemp) == 1)
						{
							sUrl = sTemp;
				
							sUrl = ZHIHU_COM_URL+sUrl;
							
							cout<<"question_url:"<<sUrl<<endl;
							
							stData.sQuestionUrl = sUrl;
							
							tempTemp= dom.begin(itTemp);
							
							sQuestionInfo = tempTemp->text();
							sQuestionInfo = replace_all(sQuestionInfo,"\n","");
							sQuestionInfo = trim(sQuestionInfo);
								
							cout<<"question_info:"<<sQuestionInfo<<endl;
							
							stData.sQuestionInfo = sQuestionInfo;
						
							sQuestionID = replace_all(sTemp,ZHIHU_QUESTION,"");
							
							stData.sQuestionID = sQuestionID;
							
							break;
						}
					}
				}
			}
		}
		else
		{
			if(html_dispose_get_other_attr_second(it,"a","class","question_link","href",sTemp) == 1)
			{
				sUrl = sTemp;
				
				sUrl = ZHIHU_COM_URL+sUrl;
				
				stData.sQuestionUrl = sUrl;
				
				cout<<stData.sQuestionUrl<<endl;
				
				itTemp= dom.begin(it);
				
				sQuestionInfo = itTemp->text();
				sQuestionInfo = replace_all(sQuestionInfo,"\n","");
				sQuestionInfo = trim(sQuestionInfo);
					
				cout<<"question_info:"<<sQuestionInfo<<endl;
				
				stData.sQuestionInfo = sQuestionInfo;
			
				sQuestionID = replace_all(sTemp,ZHIHU_QUESTION,"");
				
				stData.sQuestionID = sQuestionID;
			}
		}
		
		if(html_dispose_get_other_attr_second(it,"link","rel","canonical","href",sTemp) == 1)
		{
			sTemp = replace_all(sTemp,ZHIHU_SUB_TOPIC,"");
			
			stData.sSubID = sTemp;
		}
		
		if(html_dispose_get_other_attr_second(it,"div","class","zm-item-vote-info","data-votecount",sTemp) == 1)
		{
			stData.sAgreeNum = sTemp;
		}
		
		if(html_dispose_get_other_attr_second(it,"a","class","author-link","href",sUrl) == 1)
		{
			sUrl = ZHIHU_COM_URL+sUrl;
			
			stData.sAnswerUserUrl = sUrl;
			
			itTemp= dom.begin(it);
			
			stData.sAnswerName = itTemp->text();
		}
		else
		{
			if(html_dispose_get_attr_next_text(it,"span","class","name",sTemp) == 1)
			{
				stData.sAnswerUserUrl = "";
				
				stData.sAnswerName = sTemp;
				
				cout<<stData.sAnswerName<<endl;
			}
		}
		
		if(html_dispose_get_other_attr_second(it,"span","class","bio","title",sTemp) == 1)
		{			
			stData.sAnswerIntro = sTemp;
		}
		
		if(html_dispose_get_other_attr_second(it,"div","class","zm-item-rich-text expandable js-collapse-body","data-entry-url",sUrl) == 1)
		{
			sTemp = sUrl;
			
			sUrl = ZHIHU_COM_URL+sUrl;
			
			stData.sAnswerUrl = sUrl;
			
			if((pos = sTemp.find_last_of("/")) != string::npos && pos+1 <= sTemp.length())
				stData.sAnswerID = sTemp.substr(pos+1,string::npos);
		}
	}
	
	return 0;
}

int pull_and_insert_sub_topic_top(MYSQL *pmysql,const string &strFileDir,map<string,string> &m_stSubTopics,multimap<string,string> &mm_stTopicIdSub)
{
	if(m_stSubTopics.empty())
		return 0;
	
	string sSubTopicUrl = "";
	string sTopAnswers = "";
	string sUrl = "";
	string sSubTopicId = "";
	string sPullPath = "";
	vector<TSUBTOPICTOPANSWERS> v_TopAnswers;
	
	map<string,string>::iterator it = m_stSubTopics.begin();
	for(;it!=m_stSubTopics.end();it++)
	{
		sSubTopicId = (string)it->first;
		sSubTopicUrl = (string)it->second;
		
		if(sSubTopicId.empty() || sSubTopicUrl.empty())
			continue;
		
		sTopAnswers = sSubTopicUrl + ZHIHU_SUB_TOPIC_TOP_ANSWERS;
		
		sTopAnswers = "\'" + sTopAnswers + "\'";
		
		getCurlZhiHuUrl(sTopAnswers,sUrl,strFileDir);
				
		sPullPath = strFileDir+SUB_TOP_ANSWERS+"zhihu_topic_top_"+ sSubTopicId +".txt";
					
		pullURL(sUrl,sPullPath);
		
		dispose_sub_topic_top_answers(sPullPath,v_TopAnswers,0,sSubTopicId);
	}
	
	sub_topic_map_topicid(v_TopAnswers,mm_stTopicIdSub);
	
	mysql_insert_t_top_answers(pmysql,v_TopAnswers);
	
	return 0;
}

int update_and_insert_top_answers(MYSQL *pmysql,const string &strFileDir,multimap<string,string> &mm_stTopAnswers,multimap<string,string> &mm_stTopicIdSub)
{
	if(mm_stTopAnswers.empty())
		return 0;
	
	string sSubID = "";
	string sAnswerUrl = "";
	string sUrl = "";
	string sPullPath = "";
	string sTxtName = "";
	vector<TSUBTOPICTOPANSWERS> v_TopAnswers;
	
	multimap<string,string>::iterator it = mm_stTopAnswers.begin();
	for(;it!=mm_stTopAnswers.end();it++)
	{
		sSubID = (string)it->first;
		sAnswerUrl = (string)it->second;
		
		if(sSubID.empty() || sAnswerUrl.empty())
			continue;
		
		sTxtName = sAnswerUrl;
		
		sAnswerUrl = "\'" + sAnswerUrl + "\'";
		
		getCurlZhiHuUrl(sAnswerUrl,sUrl,strFileDir);
		
		sTxtName = replace_all(sTxtName,ZHIHU_COM_URL,"");
		sTxtName = replace_all(sTxtName,ZHIHU_QUESTION,"");
		sTxtName = replace_all(sTxtName,"/answer/","_");
						
		sPullPath = strFileDir+SUB_TOP_ANSWERS+"zhihu_topic_top_"+ sTxtName +".txt";
					
		pullURL(sUrl,sPullPath);
		
		dispose_sub_topic_top_answers(sPullPath,v_TopAnswers,1,sSubID);
	}
	
	sub_topic_map_topicid(v_TopAnswers,mm_stTopicIdSub);
	
	mysql_insert_t_top_answers(pmysql,v_TopAnswers);
	
	return 0;
}

//之前没考虑清楚在话题精华表插入母话题id，改动又太大，现在只能用子话题来找回母话题id了
int sub_topic_map_topicid(vector<TSUBTOPICTOPANSWERS> &v_TopAnswers,multimap<string,string> &mm_stTopicIdSub)
{
	if(v_TopAnswers.empty() || mm_stTopicIdSub.empty())
	{
		cout<<"v_TopAnswers empty"<<endl;
		return 0;
	}
	
	string sSubID = "";
	string sTopicID = "";
	multimap<string,string>::iterator itFind;
	TSUBTOPICTOPANSWERS szData;
	
	vector<TSUBTOPICTOPANSWERS>::iterator it = v_TopAnswers.begin();
	for(;it!= v_TopAnswers.end();it++)
	{	
		szData = (TSUBTOPICTOPANSWERS)*it;
		
		sSubID = szData.sSubID;
		
		if(sSubID.empty())
			continue;
		
		itFind = mm_stTopicIdSub.find(sSubID);
		if(itFind != mm_stTopicIdSub.end())
		{
			sTopicID = (string)itFind->second;
			
			if(sTopicID.empty())
				continue;
			
			(*it).sTopicID = sTopicID;
		}
	}
	
	return 0;
}

int get_cookies_by_account_passwd(const string& sFileName,TCOOKIES &stCookies)
{	
	string sData = "";
	string sBaseUrl = stCookies.sLoginUrl;
	string sPassword = stCookies.sPassword;
	string sTmpAccount = stCookies.sAccount;
	
	if(sTmpAccount.empty() || sPassword.empty() || sBaseUrl.empty())
		return 0;

	if(sTmpAccount.find("@") != string::npos || sTmpAccount.find("%40") != string::npos)  //email
	{
		sTmpAccount = replace_all(sTmpAccount,"@","%40");
		
		sData = "email=";
		sData = sData+sTmpAccount+"&password="+sPassword;
	}
	else //phone		
	{
		sData = "phone_num=";
		sData = sData+sTmpAccount+"&password="+sPassword;
	}
	
	sBaseUrl = "\'" + sBaseUrl + "\'";
	sData = "\'" + sData + "\'";
	
	getCurlCookies(sBaseUrl,sFileName,sData);
	
	return 0;
}

