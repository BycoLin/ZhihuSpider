#ifndef _PULL_DB_H_
#define _PULL_DB_H_

#include <mysql.h>
#include <string>
#include <iostream>
#include <map>
#include <vector>
#include "definition.h"
#include <stdio.h>
#include <fstream>
using namespace std;

#define TABLE_COOKIES               "t_cookies"
#define TABLE_USER_HASH             "t_user_hash"
#define TABLE_USER_INFO             "t_user_info"
#define TABLE_USER_JOIN             "t_user_join"
#define TABLE_TOPICS                "t_topics"
#define TABLE_SUB_TOPIC             "t_sub_topic"
#define TABLE_TOP_ANSWERS           "t_top_answers"

//wordpress_db
#define TABLE_WP_POSTS              "wp_posts"
#define TABLE_WP_TERM_RELATIONSHIPS "wp_term_relationships"

#define DB_CONF    "db.conf"
#define DB_WP_CONF "db_wp.conf"

#define GET_LIMIT_NUM 1000                             
            
typedef struct mysql_conf
{
	mysql_conf()
	{
		host = "";
		user = "";
		passwd = ""; 
		db = "";
		port = 0;
		unix_socket = "";
		client_flag = 0;
	}
	string host;
	string user;
	string passwd; 
	string db;
	unsigned int port;
	string unix_socket;
	unsigned long client_flag;
}MYSQL_CONF;

int getMysqlConf(const string& strFileDir,const string& strDBConfName,MYSQL_CONF &stMysqlConf);

int mysql_my_init(MYSQL *pmysql,MYSQL_CONF &mMySqlConf);
int mysql_my_query(MYSQL *pmysql,char *query,unsigned len);
int mysql_my_drop_query(MYSQL *pmysql,string sTable); 
int mysql_my_create_query(MYSQL *pmysql,string sGoalTable,string sTable);
int mysql_my_copy_query(MYSQL *pmysql,string sGoalTable,string sTable);
int mysql_my_rename_query(MYSQL *pmysql,string sTable,string sGoalTable);

int mysql_insert_t_user_hash(MYSQL *pmysql,map<string,TUSERHASH> &m_stUserHash);
int mysql_insert_t_wp_posts(MYSQL *pmysql,vector<TWPPOSTS> &v_stWpPosts);
int mysql_insert_t_user_info(MYSQL *pmysql,map<string,TUSERINFO> &m_stUserInfo);
int mysql_insert_t_user_join(MYSQL *pmysql,map<string,TUSERJOIN> &m_stUserJoin);
int mysql_insert_t_topics(MYSQL *pmysql,map<int,TTOPICS> &m_stTopics);
int mysql_insert_t_sub_topics(string sFile,MYSQL *pmysql,vector<TSUBTOPIC> &v_stSubTopic);
int mysql_insert_t_top_answers(MYSQL *pmysql,vector<TSUBTOPICTOPANSWERS> &v_TopAnswers);
int mysql_insert_t_wp_term_relationships(MYSQL *pmysql,TWPTERMRELATIONSHIPS &stWpTermRelationships);

int mysql_select_t_cookies(MYSQL *pmysql,TCOOKIES &stCookies);
int mysql_select_t_topics(MYSQL *pmysql,vector<string> &v_stTopics);
int mysql_select_t_user_hash(MYSQL *pmysql,vector<string> &v_stUserHash);
int mysql_select_t_sub_topic(MYSQL *pmysql,map<string,string> &m_stSubTopics);
int mysql_select_t_top_answers(MYSQL *pmysql,multimap<string,string> &mm_stTopAnswers);
int mysql_select_t_top_answers_desc(MYSQL *pmysql,vector<TSUBTOPICTOPANSWERS> &v_stTopAnswers);
int mysql_select_t_top_answers_top(MYSQL *pmysql,multimap<unsigned,TSUBTOPICTOPANSWERS,greater<unsigned> > &mm_stTopAnswers,string &sTopicID);
int mysql_select_t_sub_topic_and_topicId(MYSQL *pmysql,multimap<string,string> &mm_stTopicIdSub);
int mysql_select_max_id_from_t_wp_posts(MYSQL *pmysql,unsigned &uiID);
int mysql_select_t_top_user_top_by_num(MYSQL *pmysql,multimap<unsigned,TUSERINFO,greater<unsigned> > &mm_stTopUsers,string &sRankFieldName);
int mysql_select_t_user_info_chart(MYSQL *pmysql,multimap<unsigned,string,greater<unsigned> > &mm_stChartData,string &sChartFieldName);
int mysql_select_count_table(MYSQL *pmysql,const string& sTableName,unsigned &uiCount);

int mysql_update_time(MYSQL *pmysql,vector<string> &v_stUserHash);

#endif