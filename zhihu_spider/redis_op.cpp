#include "redis_op.h"
#include "redis_pkg.h"
#include <stdio.h>  
#include <stdlib.h>  
#include <string.h>
 
int insert_redis_ready_pull_queue(vector<SUSERINFODATA> &v_stUserData)
{
	if(v_stUserData.empty())
		return 0;
	
	SUSERINFODATA stTempUserData;
	
	string sKeyName = REDIS_READY_QUEUE_KEY;
	string sRpush = REDIS_CMD_RPUSH;
	sRpush = sRpush + " " + sKeyName + " ";
	string response = "";
	string sEnameHid = "";
	string sCmd = "";
	
	KGRedisClient kg("127.0.0.1", 6379);

	vector<SUSERINFODATA>::iterator it = v_stUserData.begin();
	for(;it != v_stUserData.end();it++)
	{
		stTempUserData = (SUSERINFODATA)*it;
		
		sEnameHid = stTempUserData.sEName + SEG_SMYBOL + stTempUserData.sHid;
		
		cout<<sEnameHid<<endl;
		
		sCmd = sRpush + sEnameHid;
		
		if(kg.ExecuteCmd(sCmd.c_str(),response)) 
		{
			cout<<"true:"<<response<<endl;
		}
		else
		{
			cout<<"false:"<<response<<endl;
		}
	}
	
	return 0;
}

int get_redis_ready_pull_queue(int iGetNum,map<string,SUSERINFODATA> &m_stUserData)
{
	string sLlen = REDIS_CMD_LLEN;
	sLlen = sLlen + " " + REDIS_READY_QUEUE_KEY + " ";
	string sLPop = REDIS_CMD_LPOP;
	sLPop = sLPop + " " + REDIS_READY_QUEUE_KEY + " ";
	string response = "";
	string sEnameHid = "";
	int iLen = 0;
	int iCount = 0;
	SUSERINFODATA stData;
	string sTmepFist = "";
	string sTmepSecond = "";
	string::size_type pos(0); 
	string sSymbol = SEG_SMYBOL;
	int iSymbolLen = 0;

	KGRedisClient kg("127.0.0.1", 6379);
	
	if(kg.ExecuteCmd(sLlen.c_str(),response)) 
	{
		cout<<"true:"<<response<<endl;
		
		if(response.empty())
			return 0;
		
		iLen = atoi(response.c_str());
		
		if(iLen <= 0)
			return 0;
		
		cout<<"redis llen:"<<iLen<<endl;
		
		iCount = iGetNum;
		
		if(iLen < iGetNum)
			iCount = iLen;
		
		while(iCount--)
		{
			if(kg.ExecuteCmd(sLPop.c_str(),response)) 
			{
				sEnameHid = response;
			
				cout<<sEnameHid<<endl;
				
				if(sEnameHid.empty())
					return 0;
				
				iSymbolLen = sSymbol.length();
				
				if((pos=sEnameHid.find_last_of(sSymbol))!=string::npos)   
				{			
					sTmepFist = sEnameHid.substr(0,pos);
					
					if(pos+iSymbolLen <= sEnameHid.length())
					{
						sTmepSecond = sEnameHid.substr(pos+iSymbolLen,sEnameHid.length());
						
						stData.sEName = sTmepFist;
						stData.sHid = sTmepSecond;
						
						cout<<stData.sEName<<"|"<<stData.sHid<<endl;
						
						m_stUserData.insert(make_pair<string,SUSERINFODATA>(stData.sHid,stData));
					}
				}
			}
			else
			{
				cout<<"false:"<<response<<endl;
			}
		}
	}
	else
	{
		cout<<"false:"<<response<<endl;
	}
	
	return 0;
}

int get_redis_should_pull_queue(map<string,SUSERINFODATA> &m_stUserData,map<string,SUSERINFODATA> &m_stPullUserData)
{
	if(m_stUserData.empty())
		return 0;
	
	SUSERINFODATA stTempUserData;
	
	string sKeyName = REDIS_ALREADY_HASH_KEY;
	string sHexists = REDIS_CMD_HEXISTS;
	sHexists = sHexists + " " + sKeyName + " ";
	string response = "";
	string sHid = "";
	string sEname = "";
	string sCmd = "";
	bool bExist = false;
	int iRet = 0;
	
	KGRedisClient kg("127.0.0.1", 6379);

	map<string,SUSERINFODATA>::iterator it = m_stUserData.begin();
	for(;it != m_stUserData.end();it++)
	{
		stTempUserData = (SUSERINFODATA)it->second;
		
		sHid = stTempUserData.sHid;
		sEname = stTempUserData.sEName;
		
		cout<<sHid<<","<<sEname<<endl;
		
		sCmd = sHexists + sHid;
		
		if(kg.ExecuteCmd(sCmd.c_str(),response)) 
		{
			cout<<"true:"<<response<<endl;
			
			if(response.empty())
				return 0;
		
			iRet = atoi(response.c_str());
			
			if(iRet <= 0)
				bExist = false;
			else
				bExist = true;
			
			if(!bExist)
			{
				m_stPullUserData.insert(make_pair<string,SUSERINFODATA>(sHid,stTempUserData));
			}
			else
			{
				cout<<"exist:"<<sHid<<","<<sEname<<endl;
			}
		}
		else
		{
			cout<<"false:"<<response<<endl;
		}
	}
	
	return 0;
}

int insert_redis_already_pull_hash(vector<SUSERINFODATA> &v_stUserData)
{
	if(v_stUserData.empty())
		return 0;
	
	SUSERINFODATA stTempUserData;
	
	string sKeyName = REDIS_ALREADY_HASH_KEY;
	string sHset = REDIS_CMD_HSET;
	sHset = sHset + " " + sKeyName + " ";
	string response = "";
	string sHid = "";
	string sEname = "";
	string sCmd = "";
	bool bExist = false;
	int iRet = 0;
	
	KGRedisClient kg("127.0.0.1", 6379);

	vector<SUSERINFODATA>::iterator it = v_stUserData.begin();
	for(;it != v_stUserData.end();it++)
	{
		stTempUserData = (SUSERINFODATA)*it;
		
		sHid = stTempUserData.sHid;
		sEname = stTempUserData.sEName;
		
		cout<<sHid<<","<<sEname<<endl;
		
		sCmd = sHset + sHid + " " + sEname;
		
		if(kg.ExecuteCmd(sCmd.c_str(),response)) 
		{
			cout<<"true:"<<response<<endl;
		}
		else
		{
			cout<<"false:"<<response<<endl;
		}
	}
	
	return 0;
}

int get_list_llen(const string& sKeyName)
{
	if(sKeyName.empty())
		return 0;

	string sLLen = REDIS_CMD_LLEN;
	string response = "";
	string sCmd = sLLen + " " + sKeyName;
	
	KGRedisClient kg("127.0.0.1", 6379);
		
	if(kg.ExecuteCmd(sCmd.c_str(),response)) 
	{		
		if(response.empty())
			return 0;
		
		return atoi(response.c_str());
	}
	
	return 0;
}

int insert_redis_sub_topics_queue(map<string,string> &m_stSubTopics)
{
	if(m_stSubTopics.empty())
		return 0;
	
	string sKeyName = REDIS_SUB_TOPICS_QUEUE_KEY;
	string sRpush = REDIS_CMD_RPUSH;
	sRpush = sRpush + " " + sKeyName + " ";
	string response = "";
	string sInfo = "";
	string sCmd = "";
	string sSubTopicID = "";
	string sTopicUrl = "";
	
	KGRedisClient kg("127.0.0.1", 6379);

	map<string,string>::iterator it = m_stSubTopics.begin();
	for(;it != m_stSubTopics.end();it++)
	{
		sSubTopicID = (string)it->first;
		sTopicUrl = (string)it->second;
		
		if(sSubTopicID.empty() || sTopicUrl.empty())
			continue;
		
		sInfo = sSubTopicID + SEG_SMYBOL + sTopicUrl;
		
		cout<<sInfo<<endl;
		
		sCmd = sRpush + sInfo;
		
		if(kg.ExecuteCmd(sCmd.c_str(),response)) 
		{
			cout<<"true:"<<response<<endl;
		}
		else
		{
			cout<<"false:"<<response<<endl;
		}
	}
	
	return 0;
}

int get_redis_sub_topics_queue(int iGetNum,map<string,string> &m_stSubTopics)
{
	string sLlen = REDIS_CMD_LLEN;
	sLlen = sLlen + " " + REDIS_SUB_TOPICS_QUEUE_KEY + " ";
	string sLPop = REDIS_CMD_LPOP;
	sLPop = sLPop + " " + REDIS_SUB_TOPICS_QUEUE_KEY + " ";
	string response = "";
	string sInfo = "";
	int iLen = 0;
	int iCount = 0;
	string sTmepFist = "";
	string sTmepSecond = "";
	string::size_type pos(0); 
	string sSymbol = SEG_SMYBOL;
	int iSymbolLen = 0;
	string sSubTopicID = "";
	string sTopicUrl = "";

	KGRedisClient kg("127.0.0.1", 6379);
	
	if(kg.ExecuteCmd(sLlen.c_str(),response)) 
	{
		cout<<"true:"<<response<<endl;
		
		if(response.empty())
			return 0;
		
		iLen = atoi(response.c_str());
		
		if(iLen <= 0)
			return 0;
		
		cout<<"redis llen:"<<iLen<<endl;
		
		iCount = iGetNum;
		
		if(iLen < iGetNum)
			iCount = iLen;
		
		while(iCount--)
		{
			if(kg.ExecuteCmd(sLPop.c_str(),response)) 
			{
				sInfo = response;
			
				cout<<sInfo<<endl;
				
				if(sInfo.empty())
					return 0;
				
				iSymbolLen = sSymbol.length();
				
				if((pos=sInfo.find_last_of(sSymbol))!=string::npos)   
				{			
					sTmepFist = sInfo.substr(0,pos);
					
					if(pos+iSymbolLen <= sInfo.length())
					{
						sTmepSecond = sInfo.substr(pos+iSymbolLen,sInfo.length());
						
						sSubTopicID = sTmepFist;
						sTopicUrl = sTmepSecond;
						
						cout<<sSubTopicID<<"|"<<sTopicUrl<<endl;
						
						m_stSubTopics.insert(make_pair<string,string>(sSubTopicID,sTopicUrl));
					}
				}
			}
			else
			{
				cout<<"false:"<<response<<endl;
			}
		}
	}
	else
	{
		cout<<"false:"<<response<<endl;
	}
	
	return 0;
}