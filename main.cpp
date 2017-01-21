/*
                                   _ooOoo_
                                  o8888888o
                                  88" . "88
                                  (| -_- |)
                                  O\  =  /O
                                ____/`—'\____
                             .'  \\|     |//  `.
                            /  \\|||  :  |||//  \
                           /  _||||| -:- |||||-  \
                           |   | \\\  -  /// |   |
                           | \_|  ''\—/''  |   |
                           \  .-\__  `-`  ___/-. /
                         ___`. .'  /—.—\  `. . __
                      ."" '<  `.___\_<|>_/___.'  >'"".
                     | | :  `- \`.;`\ _ /`;.`/ - ` : | |
                     \  \ `-.   \_ __\ /__ _/   .-` /  /
                ==========`-.____`-.___\_____/___.-`===========
                                   `=—='
               ~~^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^~~~
                         佛祖保佑       永远无BUG
*/

#include "dispose_zhihu.h"
#include <iostream>
#include <fstream>
#include "pull_zhihu.h"
#include "file_str_op.h"
#include "pull_db.h"
#include "time_op.h"

int main(int argc,char *argv[])
{
	time_t timepS,timepE;
	time (&timepS);
	
	if(argc < 2)
	{
		help();
		return -1;
	}

	if(!fGetFileDir(g_strFileDir))
	{
		cout<<"get file dir error"<<endl;
		return -1;
	}
	
	MYSQL_CONF stMysqlConf;
	MYSQL mysql;
	
	if(getMysqlConf(g_strFileDir,DB_CONF,stMysqlConf) < 0)
	{
		cout<<"no db conf"<<endl;
		return -1;
	}
	
	if(mysql_my_init(&mysql,stMysqlConf) < 0)
	{
		cout<<"mysql init error"<<endl;
		return -1;
	}

	string sSelect = argv[1];
	int iSelect = atoi(sSelect.c_str());
	
	string strTmPath = g_strFileDir+"zhihu_tm.txt";
	
	switch(iSelect)
	{
		case DISPOSE_INDEX://首页
			{
				dispose_index_part(g_strFileDir);
				
				break;
			}
		case DISPOSE_DAILY_HOT://今日最热			
			{
				dispose_daily_hot_part(g_strFileDir);
				
				break;
			}
		case DISPOSE_MONTHLY_HOT://本月最热		
			{
				dispose_month_hot_part(g_strFileDir);
				
				break;
			}
		case DISPOSE_TOPIC://话题
			{
				dispose_topic_part(&mysql,g_strFileDir);
				
				break;
			}
		case DISPOSE_FOLLOWEES://关注
			{
				dispose_followees_part(g_strFileDir);
				
				break;
			}
		case DISPOSE_ASKS://提问
			{
				dispose_asks_part(g_strFileDir);
				
				break;
			}
		case DISPOSE_ANSWERS://回答
			{
				dispose_answers_part(g_strFileDir);
				
				break;
			}
		case DISPOSE_POSTS://文章
			{
				dispose_posts_part(g_strFileDir);
				
				break;
			}
		case DISPOSE_COLLECTIONS://收藏
			{
				dispose_collections_part(g_strFileDir);
				
				break;
			}
		case DISPOSE_ZHUANLAN_SMALL://关注专栏少量	
			{
				dispose_zhuanlan_small_part(g_strFileDir);
				
				break;
			}
		case DISPOSE_ZHUANLAN://专栏	
			{
				dispose_zhuanlan_part(g_strFileDir);
				
				break;
			}
		case DISPOSE_TOPICS://话题			
			{
				dispose_topics_part(g_strFileDir);
				
				break;
			}
		case DISPOSE_TEST:
			{
				dispose_test_part(g_strFileDir);
				
				break;
			}
		case DISPOSE_FOLLOWEES_ALL:
			{
				dispose_followees_all_part(g_strFileDir);
				
				break;
			}
		case DISPOSE_PUBLISH_POST:
			{
				dispose_publish_post_part(g_strFileDir);
				
				break;
			}
		case DISPOSE_DAILY_HOT_THITY://今日最热三十条	
			{
				dispose_daily_hot_thity_part(g_strFileDir);
				
				break;
			}
		case DISPOSE_PUBLISH_DAILY_HOT_THITY_POST:
			{
				int iOffset = 0;
				
				if(argc > 2)
				{
					string sOffset = argv[2];
					iOffset = atoi(sOffset.c_str());
				}
	
				dispose_publish_daily_hot_thity_post_part(g_strFileDir,iOffset);
				
				break;
			}
		case DISPOSE_PULL_ALL_PEOPLE_FROM_SEED:
			{
				dispose_pull_all_people_from_seed_part(&mysql,g_strFileDir);
				
				break;
			}
		case DISPOSE_PULL_ALL_PEOPLE_FROM_READY_QUEUE:
			{
				dispose_pull_all_people_from_ready_queue_part(&mysql,g_strFileDir);
				
				break;
			}
		case TEST_BYCO:
			{		
				dispose_test_byco_part(g_strFileDir);
		
				break;
			}
		case DISPOSE_SUB_TOPICS:
			{				
				dispose_sub_topics_part(&mysql,g_strFileDir);
				
				break;
			}
		case DISPOSE_ERR_FOWWEER:
			{
				dispose_err_fowweer_file(&mysql,g_strFileDir);
				
				break;
			}
		case UPDATE_USER_JOIN_INFO:
			{
				update_user_hash_join_info(&mysql,g_strFileDir);
				
				break;
			}
		case PULL_SUB_TOPIC_TOP:
			{
				dispose_sub_topic_top(&mysql,g_strFileDir);
				
				break;
			}
		case UPDATE_SUB_TOPIC_TOP:
			{
				update_sub_topic_top(&mysql,g_strFileDir);
				
				break;
			}
		case TOP_ANSWER_JSON_OUT:
			{
				top_answer_json_out(&mysql);
				break;
			}
		case TOP_USER_JSON_OUT:
			{
				top_user_json_out(&mysql);
				break;
			}
		case CHART_JSON_OUT:
			{
				chart_json_out(&mysql);
				break;
			}
		case LOGIN_AND_GET_COOKIES:
			{
				login_and_get_cookies(g_strFileDir,&mysql);
				break;
			}
		case DISPOSE_PUBLISH_TOP_ANSWERS_POST:
			{
				int iOffset = 0;
				
				if(argc > 2)
				{
					string sOffset = argv[2];
					iOffset = atoi(sOffset.c_str());
				}
				
				publish_top_answers_post(g_strFileDir,&mysql,iOffset);
				break;
			}
		case UPDATE_ONE_USER_JOIN_INFO:
			{
				string sEnName = "";
				
				if(argc > 2)
				{
					sEnName = argv[2];
				}
				else
					return 0;
				
				update_sig_user_hash_join_info(&mysql,g_strFileDir,sEnName);
				
				break;
			}
		default:
			return -1;
	}
	
	cout<<strTmPath<<endl;
	
	time (&timepE);
	cout<<"start:"<<ctime(&timepS);
	cout<<"end:"<<ctime(&timepE);
	
	ofstream in;
	int ret = 0;
	
	if(!strTmPath.empty() && ret == 0)
	{
		cout<<"path:"<<strTmPath<<endl;
		
		if(!get_file_stream(strTmPath,in))
		{
			in<<"op:"<<iSelect<<endl;
			in<<"timeS:"<<ctime(&timepS);
			in<<"timeE:"<<ctime(&timepE)<<endl;
		
			in.close();
		}
	}
	
	return 0;
}
