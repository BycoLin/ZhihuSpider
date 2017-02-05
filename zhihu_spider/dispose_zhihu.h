#ifndef _DISPOSE_ZHIHU_H_
#define _DISPOSE_ZHIHU_H_

#include <iostream>
#include "pull_zhihu.h"
#include "pull_op.h"
#include "file_str_op.h"
#include "pull_db.h"
#include <string>

#define JSON_DIR "/usr/local/apache2/htdocs/json/"
#define CHART_JSON_DIR "/usr/local/apache2/htdocs/chartjson/"

#define USER_INFO_JSON "top_chart_count.json"
#define USER_HASH_JSON "top_chart_hash_count.json"

#define DUCK_HOTPOT_CATE 4 
#define DUCK_RECOMMEND_CATE 5 

#define MAX_RAND_NUM 10

#define MAX_FILE_NAME_NUM 21

const string aZhihuFileName[MAX_FILE_NAME_NUM] = 
{
	"zhihu_index",        
	"zhihu_daily_hot",             
	"zhihu_monthly_hot",			 
	"zhihu_topic",			
	"zhihu_follow",		
	"zhihu_asks", 
	"zhihu_answers",			
	"zhihu_posts",          
	"zhihu_collections",
	"zhihu_zhuanlan_small",
	"zhihu_zhuanlan",
	"zhihu_topics",
	"zhihu_test",
	"zhihu_follow_all",
	"zhihu_begin",
	"zhihu_daily_hot_",
	"zhihu_people_index",
	"zhihu_people_about",
	"zhihu_topic_error",
	"zhihu_err",
	"zhihu_cookies",
};    

#define MAX_TOP_USER_RANK_FIELD_NUM 11

const string aZhihuTopUserRankField[MAX_TOP_USER_RANK_FIELD_NUM] = 
{
	"fans_num",	
	"attention_num",
	"agree_num",        
	"thank_num",             	
	"asks_num",		
	"answers_num", 
	"posts_num",			
	"collections_num",          
	"public_editor_num",
	"ach_collect_num",
	"share_num",
};

#define MAX_CHART_FIELD_NUM 6

const string aZhihuChartField[MAX_CHART_FIELD_NUM] = 
{
	"sex",	
	"place",
	"vocation",        
	"school",             	
	"major",		
	"skill", 
};

bool get_zhihu_file_name(int iIdx,string &sFileName);
int dispose_index_part(const string &strFileDir);
int dispose_daily_hot_part(const string &strFileDir);
int dispose_month_hot_part(const string &strFileDir);
int dispose_topic_part(MYSQL *pmysql,const string &strFileDir);
int dispose_followees_part(const string &strFileDir);
int dispose_asks_part(const string &strFileDir);
int dispose_answers_part(const string &strFileDir);
int dispose_posts_part(const string &strFileDir);
int dispose_collections_part(const string &strFileDir);
int dispose_zhuanlan_small_part(const string &strFileDir);
int dispose_zhuanlan_part(const string &strFileDir);
int dispose_topics_part(const string &strFileDir);
int dispose_test_part(const string &strFileDir);
int dispose_followees_all_part(const string &strFileDir);
int dispose_publish_post_part(const string &strFileDir);
int dispose_daily_hot_thity_part(const string &strFileDir);
int dispose_publish_daily_hot_thity_post_part(const string &strFileDir,int iOffset);
int dispose_pull_all_people_from_seed_part(MYSQL *pmysql,const string &strFileDir);
int dispose_pull_all_people_from_ready_queue_part(MYSQL *pmysql,const string &strFileDir);
int dispose_test_byco_part(const string &strFileDir);
int dispose_sub_topics_part(MYSQL *pmysql,const string &strFileDir);
int dispose_err_fowweer_file(MYSQL *pmysql,const string &strFileDir);
int update_user_hash_join_info(MYSQL *pmysql,const string &strFileDir);
int dispose_sub_topic_top(MYSQL *pmysql,const string &strFileDir);
int update_sub_topic_top(MYSQL *pmysql,const string &strFileDir);
int top_answer_json_out(MYSQL *pmysql);
int top_user_json_out(MYSQL *pmysql);
int chart_json_out(MYSQL *pmysql);
int login_and_get_cookies(const string &strFileDir,MYSQL *pmysql);
int get_path_file_name(int iIdx,const string &strFileDir,string &strPullPath);
int publish_top_answers_post(const string &strFileDir,MYSQL *pmysql,int iOffset);
int update_sig_user_hash_join_info(MYSQL *pmysql,const string &strFileDir,string &sEnName);

#endif