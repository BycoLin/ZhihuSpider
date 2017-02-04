#ifndef _CJSON_H_
#define _CJSON_H_

#include "cJSON.h"
#include "definition.h"
#include <map>
#include <iostream>
using namespace std;

int top_answere_top_to_json_data(const string &strFileDir,multimap<unsigned,TSUBTOPICTOPANSWERS,greater<unsigned> > &mm_stTopAnswers,string &sTopicID);
int top_user_top_to_json_data(const string &strFileDir,multimap<unsigned,TUSERINFO,greater<unsigned> > &mm_stTopUsers,string &sRankFieldName);
int user_info_chart_to_json_data(const string &strFileDir,multimap<unsigned,string,greater<unsigned> > &mm_stChartData,string &sChartFieldName);
int top_count_to_json_data(const string &strFileDir,const string &strFileName,unsigned &uiCount);

#endif