#include "dispose_zhihu.h"
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <stdio.h>
#include "pull_post.h"
#include "redis_op.h"
#include <math.h>
#include "err_op.h"
#include "cjson_op.h"
#include <time.h> 

bool get_zhihu_file_name(int iIdx,string &sFileName)
{
	if(iIdx < 0 || iIdx >= MAX_FILE_NAME_NUM)
		return false;
	
	sFileName = aZhihuFileName[iIdx];
	
	return true;
}

int dispose_index_part(const string &strFileDir)
{
	string sUrl = "";
	string sPullPath = "";
	
	if(get_path_file_name(0,strFileDir,sPullPath) < 0)
		return -1;
	
	getCurlZhiHuUrl(ZHIHU_URL_SIGNIN,sUrl,strFileDir);
	
	pullURL(sUrl,sPullPath);
	
	dispose_zhihu_html(sPullPath);
	
	return 0;
}

int dispose_daily_hot_part(const string &strFileDir)
{
	string sTempUrl = "";
	string sUrl = "";
	string sPullPath = "";
	
	if(get_path_file_name(1,strFileDir,sPullPath) < 0)
		return -1;
	
	vector<TDAILYHOTDATA> v_stDailyHotData;
	
	get_daily_hot_url("5",sTempUrl);
	
	getCurlZhiHuUrl(sTempUrl,sUrl,strFileDir);

	pullURL(sUrl,sPullPath);
	
	dispose_zhihu_daily_hot_html(sPullPath,v_stDailyHotData);
	
	return 0;
}

int dispose_month_hot_part(const string &strFileDir)
{
	string sTempUrl = "";
	string sUrl = "";
	
	string sPullPath = "";
	
	if(get_path_file_name(2,strFileDir,sPullPath) < 0)
		return -1;
	
	get_month_hot_url("0",sTempUrl);
	
	getCurlZhiHuUrl(sTempUrl,sUrl,strFileDir);
	
	pullURL(sUrl,sPullPath);

	dispose_zhihu_monthly_hot_html(sPullPath); 
	
	return 0;
}

int dispose_topic_part(MYSQL *pmysql,const string &strFileDir)
{
	string sPullPath = "";
	
	if(get_path_file_name(3,strFileDir,sPullPath) < 0)
		return -1;

	string sUrl = "";
				
	sUrl = ZHIHU_TOPIC;
	
	pullURL(sUrl,sPullPath);
	
	map<int,TTOPICS> m_stTopics;
	
	dispose_zhihu_topic_html(sPullPath,m_stTopics);
	
	mysql_insert_t_topics(pmysql,m_stTopics);
	
	return 0;
}

int dispose_followees_part(const string &strFileDir)
{
	string sPullPath = "";
	
	if(get_path_file_name(4,strFileDir,sPullPath) < 0)
		return -1;
	
	string sUrl = "";
	getCurlZhiHuUrl(ZHIHU_FOLLOWEES,sUrl,strFileDir);
				
	pullURL(sUrl,sPullPath);
	
	dispose_zhihu_follow_html(sPullPath,0);
	
	return 0;
}

int dispose_asks_part(const string &strFileDir)
{
	string sPullPath = "";
	
	if(get_path_file_name(5,strFileDir,sPullPath) < 0)
		return -1;
	
	string sUrl = "";
	getCurlZhiHuUrl(ZHIHU_ASKS,sUrl,strFileDir);
			
	pullURL(sUrl,sPullPath);
	
	dispose_zhihu_asks_html(sPullPath);
				
	return 0;
}

int dispose_answers_part(const string &strFileDir)
{
	string sPullPath = "";
	
	if(get_path_file_name(6,strFileDir,sPullPath) < 0)
		return -1;
	
	string sUrl = "";
	getCurlZhiHuUrl(ZHIHU_ANSWERS,sUrl,strFileDir);
	
	pullURL(sUrl,sPullPath);
	
	dispose_zhihu_answers_html(sPullPath);

	return 0;
}

int dispose_posts_part(const string &strFileDir)
{
	string sPullPath = "";
	
	if(get_path_file_name(7,strFileDir,sPullPath) < 0)
		return -1;
	
	string sUrl = "";
	getCurlZhiHuUrl(ZHIHU_POSTS,sUrl,strFileDir);
				
	pullURL(sUrl,sPullPath);
	
	dispose_zhihu_posts_html(sPullPath);
	
	return 0;
}
	
int dispose_collections_part(const string &strFileDir)
{
	string sPullPath = "";
	
	if(get_path_file_name(8,strFileDir,sPullPath) < 0)
		return -1;
	
	string sUrl = "";
	getCurlZhiHuUrl(ZHIHU_COLLECTIONS,sUrl,strFileDir);
			
	pullURL(sUrl,sPullPath);
	
	dispose_zhihu_collections_html(sPullPath);
	
	return 0;
}

int dispose_zhuanlan_small_part(const string &strFileDir)
{
	string sPullPath = "";
	
	if(get_path_file_name(9,strFileDir,sPullPath) < 0)
		return -1;
	
	string sUrl = "";
	getCurlZhiHuUrl(ZHIHU_ZHUANLAN_SMALL,sUrl,strFileDir);
			
	pullURL(sUrl,sPullPath);
	
	dispose_zhihu_zhuanlan_small_html(sPullPath);
	
	return 0;
}

int dispose_zhuanlan_part(const string &strFileDir)
{
	string sPullPath = "";
	
	if(get_path_file_name(10,strFileDir,sPullPath) < 0)
		return -1;
	
	string sUrl = "";
	getCurlZhiHuUrl(ZHIHU_ZHUANLAN,sUrl,strFileDir);
				
	pullURL(sUrl,sPullPath);
	
	dispose_zhihu_zhuanlan_html(sPullPath);
	
	return 0;
}

int dispose_topics_part(const string &strFileDir)
{
	string sPullPath = "";
	
	if(get_path_file_name(11,strFileDir,sPullPath) < 0)
		return -1;
	
	string sUrl = "";
	getCurlZhiHuUrl(ZHIHU_TOPICS,sUrl,strFileDir);
				
	pullURL(sUrl,sPullPath);
	
	dispose_zhihu_topics_html(sPullPath); 
	
	return 0;		
}

int dispose_test_part(const string &strFileDir)
{
	string sPullPath = "";
	
	if(get_path_file_name(12,strFileDir,sPullPath) < 0)
		return -1;
	
	string sUrl = "";
	getCurlZhiHuUrl(ZHIHU_FOLLOWEES,sUrl,strFileDir);
				
	pullURL(sUrl,sPullPath);
	
	dispose_zhihu_follow_html(sPullPath,0);
	
	circle_dispose_e_name(strFileDir);
	
	print_e_name();
	
	return 0;
}

int dispose_followees_all_part(const string &strFileDir)
{
	string sPullPath = "";
	
	if(get_path_file_name(13,strFileDir,sPullPath) < 0)
		return -1;
	
	string sUrl = "";
	getCurlZhiHuUrl(ZHIHU_FOLLOWEES,sUrl,strFileDir);
				
	pullURL(sUrl,sPullPath);
	
	dispose_zhihu_follow_html(sPullPath,0);
	
	dispose_all_follow(strFileDir);
	
	return 0;
}

int dispose_publish_post_part(const string &strFileDir)
{
	MYSQL_CONF stMysqlConf2;
	MYSQL mysql2;
	
	if(getMysqlConf(strFileDir,DB_WP_CONF,stMysqlConf2) < 0)
	{
		cout<<"no db conf"<<endl;
		return -1;
	}
	
	if(mysql_my_init(&mysql2,stMysqlConf2) < 0)
	{
		cout<<"mysql init error"<<endl;
		return -1;
	}
	
	vector<TWPPOSTS> v_wp;
	TWPPOSTS szData;
	TDAILYHOTDATA stHotData;
	
	string sCont = "";
	stHotData.sQuestionHref = "https://www.zhihu.com/question/43453928/answer/96128445";
	stHotData.sQuestionInfo = "有哪些非常有职业道德的记者？他们做过哪些事可以证明这一点？";
	stHotData.sPeopleHref = "https://www.zhihu.com/people/qing-ru-xu-34/";
	stHotData.sPeopleName = "清如许";
	stHotData.sAgreeNum = "10000";
	stHotData.sAnswerInfo = "想对评论里觉得负能量的新闻学生说，如果你家庭允许，不妨学，不妨做，不妨忘记这个行业多操蛋，这一行很锻炼人，我做了半年执行制片，半年深度报道，感觉团队调度能力和执笔能力都有很大的提高，这给我后来各种面试提供了信心和支撑。有一天我从这个行业走…";
	
	posts_format(sCont,stHotData);
	
	stHotData.sQuestionHref = "https://www.zhihu.com/question/27160221/answer/96255093";
	stHotData.sQuestionInfo = "性格不强势，关键时候总被牵着走怎么办？";
	stHotData.sPeopleHref = "https://www.zhihu.com/people/lu-xiao-feng-9/";
	stHotData.sPeopleName = "路小疯";
	stHotData.sAgreeNum = "10000(赞)";
	stHotData.sAnswerInfo = "个月没答题了。撸一发。亲爱的，你知道强势是一种多么珍贵的品质吗？强势如你所言，是一种性格， 但更多的，是一种后天的经历，智慧的打磨，坚定的自信，一种无惧失去的勇敢。说白了，先天的崛强仍需要后天的历练，你才可能形成一种真正强势的人格。强势人…";
	
	posts_format(sCont,stHotData);

	init_wp_posts(szData,sCont,0,0);
	
	v_wp.push_back(szData);

	mysql_insert_t_wp_posts(&mysql2,v_wp);
	
	return 0;
}

int dispose_daily_hot_thity_part(const string &strFileDir)
{
	int iPos = 0;
	string sPos = "";
	vector<TDAILYHOTDATA> v_stDailyHotData;
	string sTempUrl = "";
	string sUrl = "";
	string sPullPath = "";
	string sFileName = "";
	if(!get_zhihu_file_name(15,sFileName))
		return -1;
	
	for(iPos = 0;iPos<6;iPos++)
	{				
		IntToString (sPos,iPos*5);
		
		get_daily_hot_url(sPos,sTempUrl);
		
		getCurlZhiHuUrl(sTempUrl,sUrl,strFileDir);
		
		sPullPath = strFileDir+DAILY_HOT_DIR+sFileName+sPos+TXT_SUFFIX;
		
		pullURL(sUrl,sPullPath);
		
		dispose_zhihu_daily_hot_html(sPullPath,v_stDailyHotData);
	}
			
	return 0;
}

int dispose_publish_daily_hot_thity_post_part(const string &strFileDir,int iOffset)
{
	int iPos = 0;
	string sPos = "";
	vector<TDAILYHOTDATA> v_stDailyHotData;
	
	MYSQL_CONF stMysqlConf2;
	MYSQL mysql2;
	
	string sTempUrl = "";
	string sUrl = "";
	string sPullPath = "";
	string sFileName = "";
	if(!get_zhihu_file_name(15,sFileName))
		return -1;
	
	if(getMysqlConf(strFileDir,DB_WP_CONF,stMysqlConf2) < 0)
	{
		cout<<"no db conf"<<endl;
		return -1;
	}
	
	if(mysql_my_init(&mysql2,stMysqlConf2) < 0)
	{
		cout<<"mysql init error"<<endl;
		return -1;
	}
	
	for(iPos = 0;iPos<6;iPos++)
	{		
		IntToString (sPos,iPos*5);
		
		get_daily_hot_url(sPos,sTempUrl);
		
		getCurlZhiHuUrl(sTempUrl,sUrl,strFileDir);
		
		sPullPath = strFileDir+DAILY_HOT_DIR+sFileName+sPos+TXT_SUFFIX;
		
		pullURL(sUrl,sPullPath);
		
		dispose_zhihu_daily_hot_html(sPullPath,v_stDailyHotData);
	}
	
	vector<TWPPOSTS> v_wp;
	TWPPOSTS szData;
	TDAILYHOTDATA stHotData;
	string sCont = "";
	
	if(v_stDailyHotData.empty())
		return -1;
	
	vector<TDAILYHOTDATA>::iterator it = v_stDailyHotData.begin();
	for(;it!=v_stDailyHotData.end();it++)
	{
		stHotData = (TDAILYHOTDATA)*it;
		
		posts_format(sCont,stHotData);
	}

	init_wp_posts(szData,sCont,0,iOffset);
	
	v_wp.push_back(szData);

	if(mysql_insert_t_wp_posts(&mysql2,v_wp) == 0)
	{
		unsigned uiID = 0;
		if(mysql_select_max_id_from_t_wp_posts(&mysql2,uiID) == 0)
		{
			TWPTERMRELATIONSHIPS stWpTermRelationships;
			
			stWpTermRelationships.uiObjectId = uiID;
			stWpTermRelationships.uiTermTaId = DUCK_HOTPOT_CATE;
			stWpTermRelationships.uiTermOrder = 0;
			
			mysql_insert_t_wp_term_relationships(&mysql2,stWpTermRelationships);
		}
		else
		{
			cout<<"select wp_posts max_id error"<<endl;
		}
	}
	
	return 0;
}

int dispose_pull_all_people_from_seed_part(MYSQL *pmysql,const string &strFileDir)
{
	string strPullPath = "";
	
	if(get_path_file_name(16,strFileDir,strPullPath) < 0)
		return -1;
	
	string sUrl = "";
	getCurlZhiHuUrl(ZHIHU_SEED_PEOPLE_ABOUT,sUrl,strFileDir);
				
	pullURL(sUrl,strPullPath);
	
	SUSERINFODATA stData;
	map<string,TUSERHASH> m_stUserHash;
	map<string,TUSERINFO> m_stUserInfo;
	map<string,TUSERJOIN> m_stUserJoin;
	
	if(dispose_zhihu_get_people_about_html(strPullPath,stData) == 0)
	{
		dispose_all_people_follow(pmysql,strFileDir,stData,0);//关注了
		
		dispose_all_people_follow(pmysql,strFileDir,stData,1);//粉丝
		
		vector<SUSERINFODATA> v_stUserData;
		
		v_stUserData.push_back(stData);
		
		insert_redis_already_pull_hash(v_stUserData);
		
		userdata_pkg_userhash(v_stUserData,m_stUserHash);
		userdata_pkg_userInfo(v_stUserData,m_stUserInfo);
		userdata_pkg_userJoin(v_stUserData,m_stUserJoin);
		
		mysql_insert_t_user_hash(pmysql,m_stUserHash);
		mysql_insert_t_user_info(pmysql,m_stUserInfo);
		mysql_insert_t_user_join(pmysql,m_stUserJoin);
	}
	
	return 0;
}

int dispose_pull_all_people_from_ready_queue_part(MYSQL *pmysql,const string &strFileDir)
{
	map<string,SUSERINFODATA> m_stUserData;
	map<string,SUSERINFODATA> m_stPullUserData;
	
	get_redis_ready_pull_queue(50,m_stUserData);
	
	get_redis_should_pull_queue(m_stUserData,m_stPullUserData);
	
	get_redis_should_pull_queue_fowwer(strFileDir,pmysql,m_stPullUserData);
	
	return 0;
}

int dispose_test_byco_part(const string &strFileDir)
{
	string strPullPath = "";
	
	if(get_path_file_name(17,strFileDir,strPullPath) < 0)
		return -1;
	
	string sUrl = "";
	
	getCurlZhiHuUrl(ZHIHU_PEOPLE_ABOUT,sUrl,strFileDir);
				
	pullURL(sUrl,strPullPath);
	
	SUSERINFODATA stData;
	
	dispose_zhihu_get_people_about_html(strPullPath,stData);
	
	cout<<stData.sEName<<"|"<<stData.sHid<<"|"<<stData.iFowwers<<"|"<<stData.iFowwees<<"|"<<stData.sZHname<<"|"<<stData.iAgreeNum<<"|"<<stData.iThankNum<<"|"<<stData.iAsksNum<<"|"<<stData.iAnswersNum<<"|"<<stData.iPostsNum<<"|"<<stData.iCollectionsNum<<"|"<<stData.iPublicEditorNum<<"|"<<stData.sIconUrl<<"|"<<stData.sIntroduce<<"|"<<stData.sSex<<"|"<<stData.sPlace<<"|"<<stData.sVocation<<"|"<<stData.sSchool<<"|"<<stData.sMajor<<"|"<<stData.sSkill<<"|"<<stData.iShareNum<<"|"<<stData.iAchCollectNum<<"|"<<endl;
	
	return 0;
}

int dispose_sub_topics_part(MYSQL *pmysql,const string &strFileDir)
{
	string strPullPath = "";
	
	if(get_path_file_name(18,strFileDir,strPullPath) < 0)
		return -1;
	
	vector<TSUBTOPIC> v_stSubTopic;
	vector<string> v_stTopics;
	
	mysql_select_t_topics(pmysql,v_stTopics);
	
	get_all_sub_topic_info(v_stTopics,v_stSubTopic);
	
	mysql_insert_t_sub_topics(strPullPath,pmysql,v_stSubTopic);
	
	return 0;
}

int dispose_err_fowweer_file(MYSQL *pmysql,const string &strFileDir)
{
	string strPullPath = "";
	
	if(get_path_file_name(19,strFileDir,strPullPath) < 0)
		return -1;
	
	map<string,string> g_mErrFowwInfo;
	
	getErrFowweerFileInfo(strPullPath,g_mErrFowwInfo);
	
	dispose_fowweer_info(pmysql,g_mErrFowwInfo);
	
	return 0;
}

int update_user_hash_join_info(MYSQL *pmysql,const string &strFileDir)
{
	vector<string> v_stUserHash;
	
	if(mysql_select_t_user_hash(pmysql,v_stUserHash) < 0)
		return -1;
	
	update_zhihu_user_hash_join_info(pmysql,strFileDir,v_stUserHash);
	
	return 0;
}

int update_sig_user_hash_join_info(MYSQL *pmysql,const string &strFileDir,string &sEnName)
{
	if(sEnName.empty())
		return 0;
	
	vector<string> v_stUserHash;
	
	v_stUserHash.push_back(sEnName);
	
	update_zhihu_user_hash_join_info(pmysql,strFileDir,v_stUserHash);
	
	return 0;
}

int dispose_sub_topic_top(MYSQL *pmysql,const string &strFileDir)
{
	map<string,string> m_stSubTopics;
	map<string,string> m_stGetSubTopics;
	multimap<string,string> mm_stTopicIdSub;
	
	if(mysql_select_t_sub_topic_and_topicId(pmysql,mm_stTopicIdSub) < 0)
		return -1;
	
	if(0 >= get_list_llen(REDIS_SUB_TOPICS_QUEUE_KEY))
	{
		if(mysql_select_t_sub_topic(pmysql,m_stSubTopics) < 0)
			return -1;
		
		insert_redis_sub_topics_queue(m_stSubTopics);
	}
	
	get_redis_sub_topics_queue(200,m_stGetSubTopics);
	
	pull_and_insert_sub_topic_top(pmysql,strFileDir,m_stGetSubTopics,mm_stTopicIdSub);
	
	return 0;
}

int update_sub_topic_top(MYSQL *pmysql,const string &strFileDir)
{
	multimap<string,string> mm_stTopAnswers;
	multimap<string,string> mm_stTopicIdSub;
	 
	if(mysql_select_t_top_answers(pmysql,mm_stTopAnswers) < 0)
		return -1;
	
	if(mysql_select_t_sub_topic_and_topicId(pmysql,mm_stTopicIdSub) < 0)
		return -1;
	
	update_and_insert_top_answers(pmysql,strFileDir,mm_stTopAnswers,mm_stTopicIdSub);
	
	return 0;
}

int top_answer_json_out(MYSQL *pmysql)
{
	multimap<unsigned,TSUBTOPICTOPANSWERS,greater<unsigned> > mm_stTopAnswers;
	vector<string> v_stTopics;
	string sTopicId = "";
	
	if(mysql_select_t_topics(pmysql,v_stTopics) < 0)
		return -1;
	
	if(v_stTopics.empty())
		return -1;
	
	vector<string>::iterator it = v_stTopics.begin();
	for(;it != v_stTopics.end();it++)
	{
		sTopicId = (string)*it;
		
		if(sTopicId.empty())
			continue;
		
		mm_stTopAnswers.clear();
		
		if(mysql_select_t_top_answers_top(pmysql,mm_stTopAnswers,sTopicId) < 0)
			continue;
	
		top_answere_top_to_json_data(JSON_DIR,mm_stTopAnswers,sTopicId);
	}
	
	return 0;
}

int top_user_json_out(MYSQL *pmysql)
{
	int i = 0;
	string sRankFieldName = "";
	multimap<unsigned,TUSERINFO,greater<unsigned> > mm_stTopUsers;
	
	for(i=0;i<MAX_TOP_USER_RANK_FIELD_NUM;i++)
	{
		sRankFieldName = aZhihuTopUserRankField[i];
		
		if(sRankFieldName.empty())
			continue;
		
		mm_stTopUsers.clear();
		
		if(mysql_select_t_top_user_top_by_num(pmysql,mm_stTopUsers,sRankFieldName) < 0)
			continue;
	
		top_user_top_to_json_data(JSON_DIR,mm_stTopUsers,sRankFieldName);
	}
	
	return 0;
}

int chart_json_out(MYSQL *pmysql)
{
	int i = 0;
	string sChartFieldName = "";
	multimap<unsigned,string,greater<unsigned> > mm_stChartData;
	unsigned uiCount = 0;
	
	for(i=0;i<MAX_CHART_FIELD_NUM;i++)
	{
		sChartFieldName = aZhihuChartField[i];
		
		if(sChartFieldName.empty())
			continue;
		
		mm_stChartData.clear();
		
		if(mysql_select_t_user_info_chart(pmysql,mm_stChartData,sChartFieldName) < 0)
			continue;
	
		user_info_chart_to_json_data(CHART_JSON_DIR,mm_stChartData,sChartFieldName);
	}
	
	if(mysql_select_count_table(pmysql,TABLE_USER_INFO,uiCount) == 0)
	{
		top_count_to_json_data(CHART_JSON_DIR,USER_INFO_JSON,uiCount);
	}
	
	if(mysql_select_count_table(pmysql,TABLE_USER_HASH,uiCount) == 0)
	{
		top_count_to_json_data(CHART_JSON_DIR,USER_HASH_JSON,uiCount);
	}

	return 0;
}

int get_path_file_name(int iIdx,const string &strFileDir,string &strPullPath)
{
	string sFileName = "";
	if(!get_zhihu_file_name(iIdx,sFileName))
		return -1;
	
	strPullPath = strFileDir+sFileName+TXT_SUFFIX;
	
	return 0;
}

int login_and_get_cookies(const string &strFileDir,MYSQL *pmysql)
{
	TCOOKIES stCookies;
	string strPullPath = "";
	
	if(get_path_file_name(20,strFileDir,strPullPath) < 0)
		return -1;
	
	if(mysql_select_t_cookies(pmysql,stCookies) < 0)
		return 0;
	
	get_cookies_by_account_passwd(strPullPath,stCookies);
	
	return 0;
}

int get_rand_num(int iGetNum,int iBaseNum,int *array)
{
	int n = 0;
	int i=0,idx=0; 
	int flag = 0;
	
	if(iGetNum <= 0 || iBaseNum <= 0 || iGetNum > iBaseNum)
		return -1;
	
	srand(time(0)); 
	
	while(1) 
	{ 
		n=rand()%iBaseNum; 
		
		flag = 0;
		
		for(i=0;i<idx;i++)
		{
			if(array[i]==n) 
			{
				flag = 1;
				break;
			}
		}
		
		if(flag == 0)
		{
			array[idx]=n; 
		
			if((++idx)==iGetNum)
				break; 
		} 
	} 
	
	return 0;
 }

int publish_top_answers_post(const string &strFileDir,MYSQL *pmysql,int iOffset)
{
	vector<TSUBTOPICTOPANSWERS> v_stTopAnswers;
	vector<TSUBTOPICTOPANSWERS> v_stRandTopAnswers;
	MYSQL_CONF stMysqlConf2;
	MYSQL mysql2;
	vector<TWPPOSTS> v_wp;
	TWPPOSTS szData;
	TSUBTOPICTOPANSWERS stData;
	TDAILYHOTDATA stHotData;
	string sCont = "";
	int a[MAX_RAND_NUM]={0}; 
	int isize = 0;
	int i = 0;
	int iRandIdx = 0;
	
	if(getMysqlConf(strFileDir,DB_WP_CONF,stMysqlConf2) < 0)
	{
		cout<<"no db conf"<<endl;
		return -1;
	}
	
	if(mysql_my_init(&mysql2,stMysqlConf2) < 0)
	{
		cout<<"mysql init error"<<endl;
		return -1;
	}
	
	if(mysql_select_t_top_answers_desc(pmysql,v_stTopAnswers) < 0)
		return 0;
	
	if(v_stTopAnswers.empty())
		return -1;
	
	isize = v_stTopAnswers.size();
	
	if(get_rand_num(MAX_RAND_NUM,isize,a) < 0)
		return -1;
	
	for(i = 0;i<MAX_RAND_NUM;i++)
	{
		iRandIdx = a[i];
		
		if(iRandIdx < 0)
			continue;
		
		v_stRandTopAnswers.push_back(v_stTopAnswers[iRandIdx]);
	}
	
	if(v_stRandTopAnswers.empty())
		return 0;
	
	vector<TSUBTOPICTOPANSWERS>::iterator it = v_stRandTopAnswers.begin();
	for(;it!=v_stRandTopAnswers.end();it++)
	{
		stData = (TSUBTOPICTOPANSWERS)*it;
		
		stHotData.sQuestionHref = stData.sAnswerUrl;
		stHotData.sQuestionInfo = stData.sQuestionInfo;
		stHotData.sPeopleHref = stData.sAnswerUserUrl;
		stHotData.sPeopleName = stData.sAnswerName;
		stHotData.sAgreeNum = stData.sAgreeNum;
		stHotData.sAnswerInfo = stData.sAnswerAbs;
		
		posts_format(sCont,stHotData);
	}

	init_wp_posts(szData,sCont,1,iOffset);
	
	v_wp.push_back(szData);

	if(mysql_insert_t_wp_posts(&mysql2,v_wp) == 0)
	{
		unsigned uiID = 0;
		if(mysql_select_max_id_from_t_wp_posts(&mysql2,uiID) == 0)
		{
			TWPTERMRELATIONSHIPS stWpTermRelationships;
			
			stWpTermRelationships.uiObjectId = uiID;
			stWpTermRelationships.uiTermTaId = DUCK_RECOMMEND_CATE;
			stWpTermRelationships.uiTermOrder = 0;
			
			mysql_insert_t_wp_term_relationships(&mysql2,stWpTermRelationships);
		}
		else
		{
			cout<<"select wp_posts max_id error"<<endl;
		}
	}
	
	return 0;
}