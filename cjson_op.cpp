#include "cjson_op.h"
#include <stdlib.h>
#include <fstream>
 
int top_answere_top_to_json_data(const string &strFileDir,multimap<unsigned,TSUBTOPICTOPANSWERS,greater<unsigned> > &mm_stTopAnswers,string &sTopicID)
{
	if(mm_stTopAnswers.empty() || strFileDir.empty() || sTopicID.empty())
		return 0;
	
	string sFilePath = strFileDir + "top_answers_"+ sTopicID +".json";
	string sQuestionInfo = "";
	string sQuestionUrl = "";
	string sAgreeNum = "";
	string sAnswerName = "";
	string sAnswerAbs = "";
	string sAnswerUserUrl = "";
	string sAnswerUrl = "";
	TSUBTOPICTOPANSWERS stData;
	char *out ;
	cJSON *root,*info,*subdir;
	ofstream in;

	root=cJSON_CreateObject();
	
	cJSON_AddItemToObject(root, "total", cJSON_CreateNumber(mm_stTopAnswers.size()));
	cJSON_AddItemToObject(root, "rows", subdir=cJSON_CreateArray());
	
	multimap<unsigned,TSUBTOPICTOPANSWERS>::iterator it = mm_stTopAnswers.begin();
	for(;it != mm_stTopAnswers.end();it++)
	{
		stData = (TSUBTOPICTOPANSWERS)it->second;
		
		sQuestionInfo = stData.sQuestionInfo;
		sQuestionUrl = stData.sQuestionUrl;
		sAgreeNum = stData.sAgreeNum;
		sAnswerName = stData.sAnswerName;
		sAnswerAbs = stData.sAnswerAbs;
		sAnswerUserUrl = stData.sAnswerUserUrl;
		sAnswerUrl = stData.sAnswerUrl;
		
		if(sQuestionInfo.empty() || sAnswerUrl.empty() || sAgreeNum.empty())
			continue;
	
		cJSON_AddItemToObject(subdir,"rows",info=cJSON_CreateObject());
		cJSON_AddStringToObject(info,"questioninfo", sQuestionInfo.c_str());
		cJSON_AddStringToObject(info,"questionUrl", sQuestionUrl.c_str());
		cJSON_AddStringToObject(info,"agreenum", sAgreeNum.c_str());
		cJSON_AddStringToObject(info,"answerName", sAnswerName.c_str());
		cJSON_AddStringToObject(info,"answerAbs", sAnswerAbs.c_str());
		cJSON_AddStringToObject(info,"answerUserUrl", sAnswerUserUrl.c_str());
		cJSON_AddStringToObject(info,"answerUrl", sAnswerUrl.c_str());
	}
	
	//out=cJSON_Print(root);
	out=cJSON_PrintUnformatted(root);
	cout<<out<<endl;
	
	in.open(sFilePath.c_str(),ios::trunc);
	if(in)
	{
		in<<out<<endl;
		in.close();
	}

	free(out);
	cJSON_Delete(root);
	
	return 0;
}

int top_user_top_to_json_data(const string &strFileDir,multimap<unsigned,TUSERINFO,greater<unsigned> > &mm_stTopUsers,string &sRankFieldName)
{
	if(mm_stTopUsers.empty() || strFileDir.empty() || sRankFieldName.empty())
		return 0;
	
	string sFilePath = strFileDir + "top_user_"+ sRankFieldName +".json";
	string sID = "";
	string sENname = "";
	string sZHname = "";
	string sIconUrl = "";
	string sIntroduce = "";
	string sSex = "";
	string sPlace = "";
	string sVocation = "";
	string sSchool = "";
	string sMajor = "";
	string sSkill = "";
	TUSERINFO stData;
	unsigned uiNum = 0;
	char *out ;
	cJSON *root,*info,*subdir;
	ofstream in;

	root=cJSON_CreateObject();
	
	cJSON_AddItemToObject(root, "total", cJSON_CreateNumber(mm_stTopUsers.size()));
	cJSON_AddItemToObject(root, "rows", subdir=cJSON_CreateArray());
	
	multimap<unsigned,TUSERINFO>::iterator it = mm_stTopUsers.begin();
	for(;it != mm_stTopUsers.end();it++)
	{
		uiNum = (unsigned)it->first;
		stData = (TUSERINFO)it->second;
		
		sID = stData.sID;
		sENname = stData.sENname;
		sZHname = stData.sZHname;
		sIconUrl = stData.sIconUrl;
		sIntroduce = stData.sIntroduce;
		sSex = stData.sSex;
		sPlace = stData.sPlace;
		sVocation = stData.sVocation;
		sSchool = stData.sSchool;
		sMajor = stData.sMajor;
		sSkill = stData.sSkill;
		
		if(sID.empty() || sENname.empty())
			continue;
	
		cJSON_AddItemToObject(subdir,"rows",info=cJSON_CreateObject());
		cJSON_AddStringToObject(info,"id", sID.c_str());
		cJSON_AddStringToObject(info,"en_name", sENname.c_str());
		cJSON_AddStringToObject(info,"zh_name", sZHname.c_str());
		cJSON_AddStringToObject(info,"icon_url", sIconUrl.c_str());
		cJSON_AddStringToObject(info,"introduce", sIntroduce.c_str());
		cJSON_AddStringToObject(info,"sex", sSex.c_str());
		cJSON_AddStringToObject(info,"place", sPlace.c_str());
		cJSON_AddStringToObject(info,"vocation", sVocation.c_str());
		cJSON_AddStringToObject(info,"school", sSchool.c_str());
		cJSON_AddStringToObject(info,"major", sMajor.c_str());
		cJSON_AddStringToObject(info,"skill", sSkill.c_str());
		cJSON_AddNumberToObject(info,"num",uiNum);
	}
	
	//out=cJSON_Print(root);
	out=cJSON_PrintUnformatted(root);
	cout<<out<<endl;
	
	in.open(sFilePath.c_str(),ios::trunc);
	if(in)
	{
		in<<out<<endl;
		in.close();
	}

	free(out);
	cJSON_Delete(root);
	
	return 0;
}

int user_info_chart_to_json_data(const string &strFileDir,multimap<unsigned,string,greater<unsigned> > &mm_stChartData,string &sChartFieldName)
{
	if(mm_stChartData.empty() || strFileDir.empty() || sChartFieldName.empty())
		return 0;
	
	string sFilePath = strFileDir + "top_chart_"+ sChartFieldName +".json";
	char *out ;
	cJSON *root,*info;
	ofstream in;
	string sName = "";
	unsigned uiNum = 0;

	root=cJSON_CreateArray();

	multimap<unsigned,string>::iterator it = mm_stChartData.begin();
	for(;it != mm_stChartData.end();it++)
	{
		uiNum = (unsigned)it->first;
		sName = (string)it->second;
		
		if(sName.empty())
			continue;

		cJSON_AddItemToObject(root,"",info=cJSON_CreateObject());
		cJSON_AddStringToObject(info,"name", sName.c_str());
		cJSON_AddNumberToObject(info,"y",uiNum);
	}
	
	//out=cJSON_Print(root);
	out=cJSON_PrintUnformatted(root);
	cout<<out<<endl;
	
	in.open(sFilePath.c_str(),ios::trunc);
	if(in)
	{
		in<<out<<endl;
		in.close();
	}

	free(out);
	cJSON_Delete(root);
	
	return 0;
}

int top_count_to_json_data(const string &strFileDir,const string &strFileName,unsigned &uiCount)
{
	if(strFileDir.empty())
		return 0;
	
	string sFilePath = strFileDir + strFileName;
	char *out ;
	cJSON *root;
	ofstream in;

	root=cJSON_CreateObject();

	cJSON_AddItemToObject(root, "total", cJSON_CreateNumber(uiCount));

	out=cJSON_PrintUnformatted(root);
	cout<<out<<endl;
	
	in.open(sFilePath.c_str(),ios::trunc);
	if(in)
	{
		in<<out<<endl;
		in.close();
	}

	free(out);
	cJSON_Delete(root);
	
	return 0;
}

