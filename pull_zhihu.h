#ifndef _PULL_ZHIHU_H
#define _PULL_ZHIHU_H

#include <vector>
#include <string>
#include <map>
#include <set>
#include <queue> 
#include <mysql.h>
#include "definition.h"
#include "pull_op.h"
#include "file_str_op.h"
#include "pull_db.h"
#include "pull_post.h"
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <iconv.h>
#include <unistd.h> 
#include <errno.h> 
#include "CharsetConverter.h"
#include "Uri.h"
#include "ParserDom.h"
#include "utils.h"
#include <algorithm> 
using namespace htmlcxx;
using namespace std;

extern string g_strFileDir;
extern string g_strFowUrlBak;

#define FILE_SUFFIX ".html"
#define TXT_SUFFIX ".txt"

#define PEOPLE_DIR "people/"
#define FOWWER_DIR "follower/"
#define FOWWEE_DIR "followee/"
#define DAILY_HOT_DIR "daily_hot_html/"
#define INDEX_PEOPLE_HTML "index_people_html/"
#define SUB_TOP_ANSWERS "sub_topic_top_answers/"
#define SUB_TOPIC "sub_topic/"

#define MAX_INT 2147483647

#define ERR_NOM -1

#define DAILY_HOT_PAGE_NUM 5

#define FOW_TIMES 100
extern int iFowEname;
extern string saFowEname[FOW_TIMES];

#define ZHIHU_COM_URL "https://www.zhihu.com"

#define ZHIHU_URL "www.zhihu.com"

#define ZHIHU_URL_SIGNIN " 'https://www.zhihu.com/#signin'"

#define ZHIHU_URL_B " -b "

#define ZHIHU_URL_HOST " -H 'Host: www.zhihu.com'"
#define ZHIHU_URL_USER_AGENT " -H 'User-Agent: Mozilla/5.0 (Windows NT 6.1; rv:44.0) Gecko/20100101 Firefox/44.0'"
#define ZHIHU_URL_ACCEPT " -H 'Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8'"
#define ZHIHU_URL_ACCEPT_LANG " -H 'Accept-Language: zh-CN,zh;q=0.8,en-US;q=0.5,en;q=0.3'"
#define ZHIHU_URL_ACCEPT_ENCODING " -H 'Accept-Encoding:  deflate, br'"

#define ZHIHU_URL_COOKIE " -H 'Cookie: q_c1=623f5f6d93d043598ce6a96e51fe3d84|1466134687000|1466134687000; l_cap_id=\"Yjk1ZDNlNTA1NzFhNDBlYWI3M2Q1NjcyMGVmODQwNTQ=|1466134687|186d39c40ea90e6c34a1ece6fe3348fa716d0607\"; cap_id=\"YmZkMDhmOGZhMzdjNDgyMjg0NTNhOWViZGI4OTA2ZmU=|1466134687|4098daf70c7a76dd85dfc76a86e7e5351fe9e6cf\"; n_c=1; d_c0=\"AHDA7KcvFwqPTm6COb-fBhW5chfiIB7ZJnc=|1466134688\"; _zap=c240da1e-1f78-41c1-80b7-cce98cf9fedb; __utma=51854390.101116638.1466134630.1466134630.1466134630.1; __utmb=51854390.4.10.1466134630; __utmc=51854390; __utmz=51854390.1466134630.1.1.utmcsr=baidu|utmccn=(organic)|utmcmd=organic; __utmv=51854390.100--|2=registration_date=20160324=1^3=entry_date=20160324=1; __utmt=1; login=\"ZDA2ZGNmYjc4NjU4NDY3MTg2MWQyNjU4ZTM4OTYzODQ=|1466134699|5071c54f796cdcc8b561cffe25537e7d15f8d4fb\"; z_c0=Mi4wQUlBQTJnXzBxUWtBY01Ec3B5OFhDaGNBQUFCaEFsVk5xXzJLVndBOFRYTTJzbFgzRkVaQmtENk4tVE5meEpYVXJ3|1466134699|3b9981d38b1553199b2d514dd38228a09b0e9139'"

#define ZHIHU_URL_COOKIE2 " -H 'Cookie: q_c1=1873b009a6f84a5d97bc7426bd83bee6|1462529556000|1462529556000; _za=35c46118-e4ed-4c72-8a61-976827431494; __utma=51854390.996647567.1459324379.1463650665.1463992370.15; __utmz=51854390.1460106570.9.5.utmcsr=zhihu.com|utmccn=(referral)|utmcmd=referral|utmcct=/people/guai-kai; d_c0=\"AICAdADnqQmPTqVSwAAZjIHW-_dMivCGaXc=|1458800776\"; __utmv=51854390.100-2|2=registration_date=20160324=1^3=entry_date=20160324=1; cap_id=\"YjBlNzMxMDlhMDdhNDY2MGFjMjA5NTQ0ODRjZDg5YmM=|1462529556|b0fcb94c06be71d9d7532bcb77e2a3e9b812f20a\"; l_cap_id=\"OTgwZjJjYjhjZTE1NDNjZGI0NzU4YWE3NTJkOTAxYjM=|1462529556|87ff8e88d21605b800bc7699db7ebbbe58d4c376\"; _zap=108b1c24-a264-471c-9736-354da7f6fa55; login=\"Y2JmODdhZDZjZDRhNDNhZGEyOTZhMGM5NjZlNGVhY2E=|1462529719|83c6cd750c2b8e7e2c23bcf6a91538a6ff577a7d\"; z_c0=Mi4wQUlBQTJnXzBxUWtBZ0lCMEFPZXBDUmNBQUFCaEFsVk50X3RUVndDRmkxTnJsbWFxUHprT19tZ0xMcjZWUWdkek1B|1462529719|1201ceaab3f3de39c047eb340229fbd51077bdbb; _xsrf=c21566f18db31e9959ed21f53e8f8deb; __utmc=51854390; __utmb=51854390.2.10.1463992370; __utmt=1'"

#define ZHIHU_URL_CONNECTION " -H 'Connection: keep-alive'"
#define ZHIHU_URL_CACHE_CONTROL " -H 'Cache-Control: max-age=0'"
#define ZHIHU_URL_DATA " --data "

#define ZHIHU_DAY "day"
#define ZHIHU_MONTH "month"

//今日最热、本月最热
#define ZHIHU_HOT_PRE "https://www.zhihu.com/node/ExploreAnswerListV2?params=%7B%22offset%22%3A"
#define ZHIHU_HOT_MID "%2C%22type%22%3A%22"
#define ZHIHU_HOT_BAK "%22%7D"

//子话题
#define ZHIHU_SUB_TOPIC "https://www.zhihu.com/topic/"

//话题广场
#define ZHIHU_TOPIC "https://www.zhihu.com/topics"

//关注
#define ZHIHU_FOLLOWEES "https://www.zhihu.com/people/aton/followees"

//提问
#define ZHIHU_ASKS "https://www.zhihu.com/people/aton/asks"

//回答
#define ZHIHU_ANSWERS "https://www.zhihu.com/people/aton/answers?page=1"

//文章
#define ZHIHU_POSTS "https://www.zhihu.com/people/aton/posts"

//收藏
#define ZHIHU_COLLECTIONS "https://www.zhihu.com/people/kevin-deng-19/collections"

//关注的专栏少量
#define ZHIHU_ZHUANLAN_SMALL "https://www.zhihu.com/people/kevin-deng-19"

//关注的专栏
#define ZHIHU_ZHUANLAN "https://www.zhihu.com/people/kevin-deng-19/columns/followed"

//关注的话题
#define ZHIHU_TOPICS "https://www.zhihu.com/people/kevin-deng-19/topics"

#define ZHIHU_PEOPLE "https://www.zhihu.com/people/"

#define ZHIHU_SEED_PEOPLE_ABOUT "https://www.zhihu.com/people/kaifulee/about"

#define ZHIHU_PEOPLE_ABOUT "https://www.zhihu.com/people/yu-lin-xin-32/about"

#define PAGE_NUMS 20

#define ZHIHU_NODE_1 "https://www.zhihu.com/node/ProfileFolloweesListV2?method=next&params=%7B%22offset%22%3A"
#define ZHIHU_NODE_2 "%2C%22order_by%22%3A%22created%22%2C%22hash_id%22%3A%22"
#define ZHIHU_NODE_3 "%22%7D"

#define ZHIHU_FOLLOWERS_NODE_1 "https://www.zhihu.com/node/ProfileFollowersListV2?method=next&params=%7B%22offset%22%3A"
#define ZHIHU_FOLLOWERS_NODE_2 "%2C%22order_by%22%3A%22created%22%2C%22hash_id%22%3A%22"
#define ZHIHU_FOLLOWERS_NODE_3 "%22%7D"

#define ZHIHU_SUB_TOPIC_MAX_PAGES 250

#define ZHIHU_SUB_TOPIC_URL "https://www.zhihu.com/node/TopicsPlazzaListV2"

#define ZHIHU_SUB_TOPIC_NODE_1 "method=next&params=%7B%22topic_id%22%3A"
#define ZHIHU_SUB_TOPIC_NODE_2 "%2C%22offset%22%3A"
#define ZHIHU_SUB_TOPIC_NODE_3 "%2C%22hash_id%22%3A%22bdf0980d846bf2903b7ad1e7b4b4554e"
#define ZHIHU_SUB_TOPIC_NODE_4 "%22%7D&_xsrf=c21566f18db31e9959ed21f53e8f8deb"

//kaifulee ： _xsrf=62d03642f3344f314c2a8e412eb97109，hash_id=043ff01e5d03c529c268d50f388012c2

#define ZHIHU_SUB_TOPIC_TOP_ANSWERS "/top-answers?page=1"
#define ZHIHU_QUESTION "/question/"

const string aZhihuPeople[] = 
{
	"/followees",        //关注
	"/asks",             //提问
	"/answers",			 //回答
	"/posts",			 //文章
	"/collections",		 //收藏
	"/columns/followed", //关注的专栏
	"/topics",			 //关注的话题
	"/about",            //关于个人
};

enum DISPOSE_OP
{
	DISPOSE_INDEX,
	DISPOSE_DAILY_HOT,
	DISPOSE_MONTHLY_HOT,
	DISPOSE_TOPIC,
	DISPOSE_FOLLOWEES,
	DISPOSE_ASKS,
	DISPOSE_ANSWERS,
	DISPOSE_POSTS,
	DISPOSE_COLLECTIONS,
	DISPOSE_ZHUANLAN_SMALL,
	DISPOSE_ZHUANLAN,
	DISPOSE_TOPICS,
	DISPOSE_TEST,
	DISPOSE_FOLLOWEES_ALL,
	DISPOSE_PUBLISH_POST,
	DISPOSE_DAILY_HOT_THITY,
	DISPOSE_PUBLISH_DAILY_HOT_THITY_POST,
	DISPOSE_PULL_ALL_PEOPLE_FROM_SEED,
	DISPOSE_PULL_ALL_PEOPLE_FROM_READY_QUEUE,
	TEST_BYCO,
	DISPOSE_SUB_TOPICS,
	DISPOSE_ERR_FOWWEER,
	UPDATE_USER_JOIN_INFO,
	PULL_SUB_TOPIC_TOP,
	UPDATE_SUB_TOPIC_TOP,
	TOP_ANSWER_JSON_OUT,
	TOP_USER_JSON_OUT,
	CHART_JSON_OUT,
	LOGIN_AND_GET_COOKIES,
	DISPOSE_PUBLISH_TOP_ANSWERS_POST,
	UPDATE_ONE_USER_JOIN_INFO,
	DISPOSE_LAST,
};

/***********fn*************/

int help();
void getCurlZhiHuUrlCookies(const string& sOrgUrl,string &sUrl,string &sCookiesFileName);
void getCurlZhiHuUrl(const string& sOrgUrl,string &sUrl,const string &strFileDir);
void getCurlZhiHuSubTopicsUrl(const string& sOrgUrl,string &sUrl,string &sData);
void get_daily_hot_url(const string &sOffset,string &sUrl);
void get_month_hot_url(const string &sOffset,string &sUrl);
int get_file_info(const string& strPullPath,string &sInfo);
int html_dispose_get_attr_next_text(tree<HTML::Node>::iterator &it,const string& sTagName,const string& sAttrName,const string& sSecondName,string& sGetTextName);
int html_dispose_get_attr_second(tree<HTML::Node>::iterator &it,const string& sTagName,const string& sAttrName,string& sGetSecondName);
int html_dispose_get_other_attr_second(tree<HTML::Node>::iterator &it,const string& sTagName,const string& sAttrName,const string& sSecondName,const string& sOtherAttrName,string &sGetOtherSecondName);
int disposeAjax(const string& sInfo,const string& sName,string* asName,unsigned &uiNum,unsigned uiMaxNum,int subLen,string subType);
int dispose_zhihu_html(const string& strPullPath);
int dispose_zhihu_daily_hot_html(const string& strPullPath,vector<TDAILYHOTDATA> &v_stDailyHotData);
int dispose_zhihu_topic_html(const string& strPullPath,map<int,TTOPICS> &m_stTopics);
int dispose_zhihu_sub_topic_json(int iParentTopicID,const string& strPullPath,vector<TSUBTOPIC> &v_stSubTopic);
int dispose_zhihu_follow_html(const string& strPullPath,int iType);
int dispose_zhihu_get_follow_brief_html(const string& strPullPath,vector<SUSERINFODATA> &v_stUserData);
int dispose_zhihu_get_people_index_html(const string& strPullPath,SUSERINFODATA &stData);
int dispose_zhihu_get_people_about_html(const string& strPullPath,SUSERINFODATA &stData);
int dispose_zhihu_asks_html(const string& strPullPath);
int dispose_zhihu_answers_html(const string& strPullPath);
int dispose_zhihu_posts_html(const string& strPullPath);
int dispose_zhihu_collections_html(const string& strPullPath);
int dispose_zhihu_zhuanlan_small_html(const string& strPullPath);
int dispose_zhihu_zhuanlan_html(const string& strPullPath);
int dispose_zhihu_topics_html(const string& strPullPath);
int dispose_zhihu_monthly_hot_html(const string& strPullPath);
int circle_dispose_e_name(const string &strFileDir);
int print_e_name();
int dispose_zhihu_profileFolloweesList(const string& strPullPath);
int dispose_all_follow(const string &strFileDir);
int init_wp_posts(TWPPOSTS &szData,string &sContent,int iType,int iOffset);
int dispose_all_people_follow(MYSQL *pmysql,const string& strFileDir,const SUSERINFODATA &stData,int iType);
int userdata_pkg_userhash(vector<SUSERINFODATA> &v_stUserData,map<string,TUSERHASH> &m_stUserHash);
int userdata_pkg_userInfo(vector<SUSERINFODATA> &v_stUserData,map<string,TUSERINFO> &m_stUserInfo);
int userdata_pkg_userJoin(vector<SUSERINFODATA> &v_stUserData,map<string,TUSERJOIN> &m_stUserJoin);
int get_redis_should_pull_queue_fowwer(const string& strFileDir,MYSQL *pmysql,map<string,SUSERINFODATA> &m_stPullUserData);
int get_zhihu_sub_topic_by_parent_topic(string sParentTopicID,vector<TSUBTOPIC> &v_stSubTopic);
int get_all_sub_topic_info(vector<string> &v_stTopics,vector<TSUBTOPIC> &v_stSubTopic);
int dispose_fowweer_info(MYSQL *pmysql,map<string,string> &g_mErrFowwInfo);
int update_zhihu_user_hash_join_info(MYSQL *pmysql,const string &strFileDir,vector<string> &v_stUserHash);
int pull_and_insert_sub_topic_top(MYSQL *pmysql,const string &strFileDir,map<string,string> &m_stSubTopics,multimap<string,string> &mm_stTopicIdSub);
int update_and_insert_top_answers(MYSQL *pmysql,const string &strFileDir,multimap<string,string> &mm_stTopAnswers,multimap<string,string> &mm_stTopicIdSub);
int sub_topic_map_topicid(vector<TSUBTOPICTOPANSWERS> &v_TopAnswers,multimap<string,string> &mm_stTopicIdSub);
int get_cookies_by_account_passwd(const string& sFileName,TCOOKIES &stCookies);

/************************/

#endif