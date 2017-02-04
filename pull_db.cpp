#include "pull_db.h"
#include "file_str_op.h"
#include <string.h>

string getConf(string sInfo,const string &sLValue,const string &sRValue)
{
	if(sInfo.empty() || sLValue.empty() || sRValue.empty())
		return sInfo;
	
	string::size_type  pos(0);   
	int iLlen = sLValue.length();		
	
    if((pos=sInfo.find(sLValue))!=string::npos)     
	{
		if(pos+iLlen <= sInfo.length())
			sInfo = sInfo.substr(pos+iLlen,sInfo.length());
		
		if((pos=sInfo.find(sRValue))!=string::npos)
		{
			sInfo = sInfo.substr(0,pos);
		}
	}		
	
	return sInfo;
}

int getMysqlConf(const string& strFileDir,const string& strDBConfName,MYSQL_CONF &stMysqlConf)
{
	string sfilepath = strFileDir+ strDBConfName;

	fstream FileStream;
	FileStream.open(sfilepath.c_str(),ios::in); 
	istreambuf_iterator<char> fileBeg(FileStream),fileEnd;
	string sInfo(fileBeg, fileEnd);
	FileStream.close();
	
	if(sInfo.empty())
	{
		cout<<"no db conf"<<endl;
		return -1;
	}

	string sTempPort = "";

	stMysqlConf.host = getConf(sInfo,"<host>","</host>");
	stMysqlConf.user = getConf(sInfo,"<user>","</user>");
	stMysqlConf.passwd = getConf(sInfo,"<passwd>","</passwd>");
	stMysqlConf.db = getConf(sInfo,"<db>","</db>");
	sTempPort = getConf(sInfo,"<port>","</port>");
	
	if(!sTempPort.empty())
		stMysqlConf.port = atoi(sTempPort.c_str());
	
	return 0;
}

int mysql_my_init(MYSQL *pmysql,MYSQL_CONF &mMySqlConf)
{
	if(!mysql_init(pmysql))
	{
		cout<<"mysql_init err"<<endl;
		return -1;
	}
			
	char cOpt = 1;
	
	if(mysql_options(pmysql,MYSQL_OPT_RECONNECT,(char *)&cOpt))
	{
		cout<<"mysql option error"<<endl;
	}
	
	if(!mysql_real_connect(pmysql,mMySqlConf.host.c_str(),mMySqlConf.user.c_str(),mMySqlConf.passwd.c_str(),\
			mMySqlConf.db.c_str(),mMySqlConf.port,mMySqlConf.unix_socket.c_str(),mMySqlConf.client_flag)){
		 fprintf(stderr,"Failedtoconnecttodatabase:Error:%s\\n",mysql_error(pmysql));
		 return -1;
	}
	
	return 0;
}

int mysql_my_query(MYSQL *pmysql,char *query,unsigned len)
{
	if(!pmysql || !query || len == 0)
		return -1;
	
	if(mysql_real_query(pmysql,query,len)!=0)
	{
		 cout<<"query failed"<<endl;  
		 return -1;
	}
	
	return 0;
}

int mysql_my_drop_query(MYSQL *pmysql,string sTable)
{
	char sqlDrop[] = "drop table if exists %s;";
	char query[200]={0};
	unsigned len = 0;
	
	sprintf(query,sqlDrop,sTable.c_str());
	len = (unsigned)strlen(query);
	if(0 > mysql_my_query(pmysql,query,len))
	{
		cout<<"drop table failed:"<<sTable<<endl;
		return -1;
	}
	
	return 0;
}

int mysql_my_create_query(MYSQL *pmysql,string sGoalTable,string sTable)
{
	char sqlCreate[] = "create table %s like %s;";
	char query[200]={0};
	unsigned len = 0;
	
	//复制表中结构
	sprintf(query,sqlCreate,sGoalTable.c_str(),sTable.c_str());
	len = (unsigned)strlen(query);
	if(0 > mysql_my_query(pmysql,query,len))
	{
		cout<<"create table failed:"<<sGoalTable<<endl;
		return -1;
	}
	
	return 0;
}

int mysql_my_copy_query(MYSQL *pmysql,string sGoalTable,string sTable)
{
	char sqlCopy[] = "insert into %s select * from %s;";
	char query[200]={0};
	unsigned len = 0;
	
	sprintf(query,sqlCopy,sGoalTable.c_str(),sTable.c_str());
	len = (unsigned)strlen(query);
	if(0 > mysql_my_query(pmysql,query,len))
	{
		cout<<"copy table data to table	failed:"<<sGoalTable<<endl;
		return -1;
	}
	
	return 0;
}

int mysql_my_rename_query(MYSQL *pmysql,string sTable,string sGoalTable)
{
	char sqlRname[] = "rename table %s to %s;";
	char query[200]={0};
	unsigned len = 0;
	
	sprintf(query,sqlRname,sTable.c_str(),sGoalTable.c_str());
	len = (unsigned)strlen(query);
	if(0 > mysql_my_query(pmysql,query,len))
	{
		cout<<"rename to table failed:"<<sGoalTable<<endl;
		return -1;
	}
	
	return 0;
}

int mysql_insert_t_user_hash(MYSQL *pmysql,map<string,TUSERHASH> &m_stUserHash)
{
	if(m_stUserHash.empty())
	{
		cout<<"m_stUserHash empty"<<endl;
		return 0;
	}

	//自动插入update_time当前时间
	char sql[] = "replace into %s (id,en_name,zh_name) value(\"%s\",\"%s\",\"%s\");";		

	char query[5024]={0};
	string slastQuery="";

	int iNum = 0;
	int i = 0;
	TUSERHASH szData;
	int iSize = m_stUserHash.size();

	string sOrgTable = TABLE_USER_HASH;
	
	mysql_query(pmysql,"START TRANSACTION");
	
	mysql_query(pmysql,"set names utf8mb4");
	
	map<string,TUSERHASH>::iterator it = m_stUserHash.begin();
	for(;it!= m_stUserHash.end();it++)
	{
		i++;
	
		szData = (TUSERHASH)it->second;	

		sprintf(query, sql,sOrgTable.c_str(),szData.sID.c_str(),szData.sENname.c_str(),szData.sZHname.c_str());
	
		slastQuery = query;	
		cout<<slastQuery<<endl;		
		
		if(mysql_real_query(pmysql,slastQuery.c_str(),(unsigned int)slastQuery.length())==0)
		{
			 iNum++;
			 cout<<i<<" for query successful"<<endl;
		}
		else
		{
			 fprintf(stderr,"query failed:Error:%s\n",mysql_error(pmysql));
		}
		
		if(iNum >= 100 || i >= iSize)
		{
			mysql_query(pmysql,"COMMIT");	
			iNum = 0;
		}
	}

	return 0;
}

int mysql_insert_t_user_info(MYSQL *pmysql,map<string,TUSERINFO> &m_stUserInfo)
{
	if(m_stUserInfo.empty())
	{
		cout<<"m_stUserInfo empty"<<endl;
		return 0;
	}

	//自动插入update_time当前时间
	char sql[] = "replace into %s (id,en_name,zh_name,icon_url,introduce,sex,place,vocation,school,major,skill) value(\"%s\",\"%s\",\"%s\",\"%s\",\"%s\",\"%s\",\"%s\",\"%s\",\"%s\",\"%s\",\"%s\");";		

	char query[3000]={0};
	string slastQuery="";

	int iNum = 0;
	int i = 0;
	TUSERINFO szData;
	int iSize = m_stUserInfo.size();

	string sOrgTable = TABLE_USER_INFO;
	
	mysql_query(pmysql,"START TRANSACTION");
	
	mysql_query(pmysql,"set names utf8mb4");
	
	map<string,TUSERINFO>::iterator it = m_stUserInfo.begin();
	for(;it!= m_stUserInfo.end();it++)
	{
		i++;
	
		szData = (TUSERINFO)it->second;	

		sprintf(query, sql,sOrgTable.c_str(),szData.sID.c_str(),szData.sENname.c_str(),szData.sZHname.c_str(),szData.sIconUrl.c_str(),szData.sIntroduce.c_str(),szData.sSex.c_str(),szData.sPlace.c_str(),szData.sVocation.c_str(),szData.sSchool.c_str(),szData.sMajor.c_str(),szData.sSkill.c_str());
		
		slastQuery = query;	
		cout<<slastQuery<<endl;		
		
		if(mysql_real_query(pmysql,slastQuery.c_str(),(unsigned int)slastQuery.length())==0)
		{
			 iNum++;
			 cout<<i<<" for query successful"<<endl;
		}
		else
		{
			 fprintf(stderr,"query failed:Error:%s\n",mysql_error(pmysql));
		}
		
		if(iNum >= 100 || i >= iSize)
		{
			mysql_query(pmysql,"COMMIT");	
			iNum = 0;
		}
	}

	return 0;
}

int mysql_insert_t_user_join(MYSQL *pmysql,map<string,TUSERJOIN> &m_stUserJoin)
{
	if(m_stUserJoin.empty())
	{
		cout<<"m_stUserJoin empty"<<endl;
		return 0;
	}

	//自动插入update_time当前时间
	char sql[] = "replace into %s (id,en_name,agree_num,thank_num,attention_num,fans_num,asks_num,answers_num,posts_num,collections_num,public_editor_num,ach_collect_num,share_num) value(\"%s\",\"%s\",%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d);";		

	char query[3000]={0};
	string slastQuery="";

	int iNum = 0;
	int i = 0;
	TUSERJOIN szData;
	int iSize = m_stUserJoin.size();

	string sOrgTable = TABLE_USER_JOIN;
	
	mysql_query(pmysql,"START TRANSACTION");
	
	mysql_query(pmysql,"set names utf8mb4");
	
	map<string,TUSERJOIN>::iterator it = m_stUserJoin.begin();
	for(;it!= m_stUserJoin.end();it++)
	{
		i++;
	
		szData = (TUSERJOIN)it->second;
		
		sprintf(query, sql,sOrgTable.c_str(),szData.sID.c_str(),szData.sENname.c_str(),szData.iAgreeNum,szData.iThankNum,szData.iAttentionNum,szData.iFansNum,szData.iAsksNum,szData.iAnswersNum,szData.iPostsNum,szData.iCollectionsNum,szData.iPublicEditorNum,szData.iAchCollectNum,szData.iShareNum);
		
		slastQuery = query;	
		cout<<slastQuery<<endl;		
		
		if(mysql_real_query(pmysql,slastQuery.c_str(),(unsigned int)slastQuery.length())==0)
		{
			 iNum++;
			 cout<<i<<" for query successful"<<endl;
		}
		else
		{
			 fprintf(stderr,"query failed:Error:%s\n",mysql_error(pmysql));
		}
		
		if(iNum >= 100 || i >= iSize)
		{
			mysql_query(pmysql,"COMMIT");	
			iNum = 0;
		}
	}

	return 0;
}

int mysql_insert_t_wp_posts(MYSQL *pmysql,vector<TWPPOSTS> &v_stWpPosts)
{
	if(v_stWpPosts.empty())
	{
		cout<<"v_stWpPosts empty"<<endl;
		return -1;
	}

	//自动插入id
	char sql[] = "replace into %s (post_author,post_date,post_date_gmt,post_content,post_title,post_excerpt,post_status,comment_status,ping_status,post_password,post_name,to_ping,pinged,post_modified,post_modified_gmt,post_content_filtered,post_parent,guid,menu_order,post_type,post_mime_type,comment_count) value(%d,\"%s\",\"%s\",\"%s\",\"%s\",\"%s\",\"%s\",\"%s\",\"%s\",\"%s\",\"%s\",\"%s\",\"%s\",\"%s\",\"%s\",\"%s\",%d,\"%s\",%d,\"%s\",\"%s\",%d);";		

	char query[50000]={0};
	string slastQuery="";

	int iNum = 0;
	int i = 0;
	TWPPOSTS szData;
	int iSize = v_stWpPosts.size();

	string sOrgTable = TABLE_WP_POSTS;
	
	mysql_query(pmysql,"START TRANSACTION");
	
	mysql_query(pmysql,"set names utf8");
	
	vector<TWPPOSTS>::iterator it = v_stWpPosts.begin();
	for(;it!= v_stWpPosts.end();it++)
	{
		i++;
	
		szData = (TWPPOSTS)*it;	
	
		sprintf(query, sql,sOrgTable.c_str(),szData.uiPostAuthor,szData.sPostDate.c_str(),szData.sPostDateGmt.c_str(),szData.sPostContent.c_str(),szData.sPostTitle.c_str(),szData.sPostExcerpt.c_str(),szData.sPostStatus.c_str(),szData.sCommentStatus.c_str(),szData.sPingStatus.c_str(),szData.sPostPassWd.c_str(),szData.sPostName.c_str(),szData.sToPing.c_str(),szData.sPinged.c_str(),szData.sPostModified.c_str(),szData.sPostModifiedGmt.c_str(),szData.sPostContFiltered.c_str(),szData.uiPostParent,szData.sGuid.c_str(),szData.iMenuOrder,szData.sPostType.c_str(),szData.sPostMimeType.c_str(),szData.uiCommentCount);
	
		slastQuery = query;	
		cout<<slastQuery<<endl;		
		
		if(mysql_real_query(pmysql,slastQuery.c_str(),(unsigned int)slastQuery.length())==0)
		{
			 iNum++;
			 cout<<i<<" for query successful"<<endl;
		}
		else
		{
			 fprintf(stderr,"query failed:Error:%s\n",mysql_error(pmysql));
		}
		
		if(iNum >= 100 || i >= iSize)
		{
			mysql_query(pmysql,"COMMIT");	
			iNum = 0;
		}
	}

	return 0;
}
  
int mysql_insert_t_topics(MYSQL *pmysql,map<int,TTOPICS> &m_stTopics)
{
	if(m_stTopics.empty())
	{
		cout<<"m_stTopics empty"<<endl;
		return 0;
	}

	//自动插入update_time当前时间
	char sql[] = "replace into %s (topicId,topicName,topicUrl) value(%d,\"%s\",\"%s\");";		

	char query[5024]={0};
	string slastQuery="";

	int iNum = 0;
	int i = 0;
	TTOPICS szData;
	int iSize = m_stTopics.size();

	string sOrgTable = TABLE_TOPICS;
	
	mysql_query(pmysql,"START TRANSACTION");
	
	mysql_query(pmysql,"set names utf8mb4");
	
	map<int,TTOPICS>::iterator it = m_stTopics.begin();
	for(;it!= m_stTopics.end();it++)
	{
		i++;
	
		szData = (TTOPICS)it->second;
	
		sprintf(query, sql,sOrgTable.c_str(),szData.iTopicID,szData.sTopicName.c_str(),szData.sTopicUrl.c_str());
	
		slastQuery = query;	
		cout<<slastQuery<<endl;		
		
		if(mysql_real_query(pmysql,slastQuery.c_str(),(unsigned int)slastQuery.length())==0)
		{
			 iNum++;
			 cout<<i<<" for query successful"<<endl;
		}
		else
		{
			 fprintf(stderr,"query failed:Error:%s\n",mysql_error(pmysql));
		}
		
		if(iNum >= 100 || i >= iSize)
		{
			mysql_query(pmysql,"COMMIT");	
			iNum = 0;
		}
	}

	return 0;
}

int mysql_insert_t_sub_topics(string sFile,MYSQL *pmysql,vector<TSUBTOPIC> &v_stSubTopic)
{
	if(v_stSubTopic.empty())
	{
		cout<<"v_stSubTopic empty"<<endl;
		return 0;
	}

	//自动插入update_time当前时间
	char sql[] = "replace into %s (topicId,subId,subName,subUrl) value(%d,%d,\"%s\",\"%s\");";		

	char query[5024]={0};
	string slastQuery="";

	int iNum = 0;
	int i = 0;
	TSUBTOPIC szData;
	int iSize = v_stSubTopic.size();
	
	ofstream in;
	bool bF = false;
	
	if(!sFile.empty())
	{
		if(!get_file_stream(sFile,in))
		{
			bF = true;
		}
	}

	string sOrgTable = TABLE_SUB_TOPIC;
	
	mysql_query(pmysql,"START TRANSACTION");
	
	mysql_query(pmysql,"set names utf8mb4");
	
	vector<TSUBTOPIC>::iterator it = v_stSubTopic.begin();
	for(;it!= v_stSubTopic.end();it++)
	{
		i++;
	
		szData = (TSUBTOPIC)*it;
	
		sprintf(query, sql,sOrgTable.c_str(),szData.iTopicID,szData.iSubID,szData.sSubName.c_str(),szData.sSubUrl.c_str());
	
		slastQuery = query;
		
		if(mysql_real_query(pmysql,slastQuery.c_str(),(unsigned int)slastQuery.length())==0)
		{
			 iNum++;
			 //cout<<i<<" for query successful"<<endl;
		}
		else
		{
			 fprintf(stderr,"query failed:Error:%s\n",mysql_error(pmysql));
			 
			 if(bF)
			 {
				in<<slastQuery<<endl;
				in<<mysql_error(pmysql)<<endl;
			 }
		}
		
		if(iNum >= 100 || i >= iSize)
		{
			cout<<slastQuery<<endl;		
			
			mysql_query(pmysql,"COMMIT");	
			iNum = 0;
		}
	}
	
	if(bF)
		in.close();

	return 0;
}

int mysql_insert_t_top_answers(MYSQL *pmysql,vector<TSUBTOPICTOPANSWERS> &v_TopAnswers)
{
	if(v_TopAnswers.empty())
	{
		cout<<"v_TopAnswers empty"<<endl;
		return 0;
	}
	
	//自动插入update_time当前时间
	char sql[] = "replace into %s (subId,questionId,answerId,topicId,questionInfo,questionUrl,sAgreeNum,answerName,answerIntro,answerAbs,answerImgUrl,answerUserUrl,answerUrl) value(\"%s\",\"%s\",\"%s\",\"%s\",\"%s\",\"%s\",\"%s\",\"%s\",\"%s\",\"%s\",\"%s\",\"%s\",\"%s\");";		

	char query[3000]={0};
	string slastQuery="";
	
	int iNum = 0;
	int i = 0;
	TSUBTOPICTOPANSWERS szData;
	int iSize = v_TopAnswers.size();
	
	string sOrgTable = TABLE_TOP_ANSWERS;
	
	mysql_query(pmysql,"START TRANSACTION");
	
	mysql_query(pmysql,"set names utf8mb4");
	
	vector<TSUBTOPICTOPANSWERS>::iterator it = v_TopAnswers.begin();
	for(;it!= v_TopAnswers.end();it++)
	{
		i++;
	
		szData = (TSUBTOPICTOPANSWERS)*it;
		
		insert_spe_sign_dispose(szData.sQuestionInfo);
		insert_spe_sign_dispose(szData.sAnswerName);
		insert_spe_sign_dispose(szData.sAnswerIntro);
		insert_spe_sign_dispose(szData.sAnswerAbs);
	
		sprintf(query, sql,sOrgTable.c_str(),szData.sSubID.c_str(),szData.sQuestionID.c_str(),szData.sAnswerID.c_str(),szData.sTopicID.c_str(),szData.sQuestionInfo.c_str(),szData.sQuestionUrl.c_str(),szData.sAgreeNum.c_str(),szData.sAnswerName.c_str(),szData.sAnswerIntro.c_str(),szData.sAnswerAbs.c_str(),szData.sAnswerImgUrl.c_str(),szData.sAnswerUserUrl.c_str(),szData.sAnswerUrl.c_str());
	
		slastQuery = query;
		
		if(mysql_real_query(pmysql,slastQuery.c_str(),(unsigned int)slastQuery.length())==0)
		{
			 iNum++;
			 //cout<<i<<" for query successful"<<endl;
		}
		else
		{
			 fprintf(stderr,"query failed:Error:%s\n",mysql_error(pmysql));
			 
			 cout<<"error:"<<slastQuery<<endl;		
		}
		
		if(iNum >= 100 || i >= iSize)
		{
			cout<<slastQuery<<endl;		
			
			mysql_query(pmysql,"COMMIT");	
			iNum = 0;
		}
	}

	return 0;
}

int mysql_insert_t_wp_term_relationships(MYSQL *pmysql,TWPTERMRELATIONSHIPS &stWpTermRelationships)
{
	if(stWpTermRelationships.uiObjectId <= 0)
	{
		return 0;
	}
	
	char sql[] = "replace into %s (object_id,term_taxonomy_id,term_order) value(%d,%d,%d);";		

	char query[100]={0};
	string slastQuery="";

	string sOrgTable = TABLE_WP_TERM_RELATIONSHIPS;
	
	mysql_query(pmysql,"START TRANSACTION");
	
	mysql_query(pmysql,"set names utf8mb4");
	
	sprintf(query,sql,sOrgTable.c_str(),stWpTermRelationships.uiObjectId,stWpTermRelationships.uiTermTaId,stWpTermRelationships.uiTermOrder);

	slastQuery = query;	
	
	cout<<slastQuery<<endl;		
	
	if(mysql_real_query(pmysql,slastQuery.c_str(),(unsigned int)slastQuery.length())!=0)
	{
		 fprintf(stderr,"query failed:Error:%s\n",mysql_error(pmysql));
	}
	
	mysql_query(pmysql,"COMMIT");	

	return 0;
}

int mysql_select_t_cookies(MYSQL *pmysql,TCOOKIES &stCookies)
{
	MYSQL_RES *res;
	MYSQL_ROW row;
	
	char sql[] = "select * from %s order by rand() limit 1;";
	char query[1024]={0};

	sprintf(query,sql,TABLE_COOKIES);
	
	if(mysql_real_query(pmysql,query,(unsigned int)strlen(query))!=0)
	{
		 cout<<__FUNCTION__<<" query failed"<<endl;  
		 return -1;
	}
	
	string sAccount = "";
	string sPassword = "";
	string sLoginUrl = "";
		
	res=mysql_store_result(pmysql);
	while((row=mysql_fetch_row(res)))
	{		
		sAccount = row[0];
		sPassword = row[1];
		sLoginUrl = row[2];
		
		if(sAccount.empty() || sPassword.empty() || sLoginUrl.empty())
			continue;
		
		cout<<sAccount<<endl;
	
		stCookies.sAccount = sAccount;
		stCookies.sPassword = sPassword;
		stCookies.sLoginUrl = sLoginUrl;
	}

	return 0;
}

int mysql_select_t_topics(MYSQL *pmysql,vector<string> &v_stTopics)
{
	MYSQL_RES *res;
	MYSQL_ROW row;
	
	char sql[] = "select topicId from %s;";
	char query[1024]={0};

	sprintf(query,sql,TABLE_TOPICS);
	
	if(mysql_real_query(pmysql,query,(unsigned int)strlen(query))!=0)
	{
		 cout<<__FUNCTION__<<" query failed"<<endl;  
		 return -1;
	}
	
	string sTopicID = "";
		
	res=mysql_store_result(pmysql);
	while((row=mysql_fetch_row(res)))
	{		
		sTopicID = row[0];
		
		if(sTopicID.empty())
			continue;
	
		v_stTopics.push_back(sTopicID);
	}

	return 0;
}

int mysql_select_t_sub_topic(MYSQL *pmysql,map<string,string> &m_stSubTopics)
{
	MYSQL_RES *res;
	MYSQL_ROW row;
	
	char sql[] = "select subId,subUrl from %s";
	char query[4024]={0};

	sprintf(query,sql,TABLE_SUB_TOPIC);
	
	if(mysql_real_query(pmysql,query,(unsigned int)strlen(query))!=0)
	{
		 cout<<__FUNCTION__<<" query failed"<<endl;  
		 return -1;
	}
	
	string sSubTopicID = "";
	string sTopicUrl = "";
		
	res=mysql_store_result(pmysql);
	while((row=mysql_fetch_row(res)))
	{		
		sSubTopicID = row[0];
		sTopicUrl = row[1];
		
		if(sSubTopicID.empty() || sTopicUrl.empty())
			continue;
	
		m_stSubTopics.insert(make_pair<string,string>(sSubTopicID,sTopicUrl));
	}

	return 0;
}

int mysql_select_t_sub_topic_and_topicId(MYSQL *pmysql,multimap<string,string> &mm_stTopicIdSub)
{
	MYSQL_RES *res;
	MYSQL_ROW row;
	
	char sql[] = "select topicId,subId from %s";
	char query[5024]={0};

	sprintf(query,sql,TABLE_SUB_TOPIC);
	
	if(mysql_real_query(pmysql,query,(unsigned int)strlen(query))!=0)
	{
		 cout<<__FUNCTION__<<" query failed"<<endl;  
		 return -1;
	}
	
	string sSubTopicID = "";
	string sTopicId = "";
		
	res=mysql_store_result(pmysql);
	while((row=mysql_fetch_row(res)))
	{		
		sTopicId = row[0];
		sSubTopicID = row[1];
		
		if(sTopicId.empty() || sSubTopicID.empty())
			continue;
	
		mm_stTopicIdSub.insert(make_pair<string,string>(sSubTopicID,sTopicId));
	}

	return 0;
}

int mysql_select_t_top_answers(MYSQL *pmysql,multimap<string,string> &mm_stTopAnswers)
{
	MYSQL_RES *res;
	MYSQL_ROW row;
	
	char sql[] = "select subId,answerUrl from %s order by update_time limit 500;";
	//char sql[] = "select subId,answerUrl from %s where questionId='21130452' and answerId='18633715';";
	char query[5024]={0};

	sprintf(query,sql,TABLE_TOP_ANSWERS);
	
	if(mysql_real_query(pmysql,query,(unsigned int)strlen(query))!=0)
	{
		 cout<<__FUNCTION__<<" query failed"<<endl;  
		 return -1;
	}

	string sAnswerUrl = "";
	string sSubid = "";
		
	res=mysql_store_result(pmysql);
	while((row=mysql_fetch_row(res)))
	{		
		sSubid = row[0];
		sAnswerUrl = row[1];
		
		if(sSubid.empty() || sAnswerUrl.empty())
			continue;
	
		mm_stTopAnswers.insert(make_pair<string,string>(sSubid,sAnswerUrl));
	}

	return 0;
}

int mysql_select_t_top_answers_desc(MYSQL *pmysql,vector<TSUBTOPICTOPANSWERS> &v_stTopAnswers)
{
	MYSQL_RES *res;
	MYSQL_ROW row;
	
	char sql[] = "select * from %s order by update_time desc limit 50;";
	char query[20000]={0};

	sprintf(query,sql,TABLE_TOP_ANSWERS);
	
	mysql_query(pmysql,"set names utf8mb4");
	
	if(mysql_real_query(pmysql,query,(unsigned int)strlen(query))!=0)
	{
		 cout<<__FUNCTION__<<" query failed"<<endl;  
		 return -1;
	}

	string sSubid = "";
	string sQuestionID = "";
	string sAnswerID = "";
	string sTopicID = "";	
	string sQuestionInfo = "";
	string sQuestionUrl = "";
	string sAgreeNum = "";
	string sAnswerName = "";
	string sAnswerIntro = "";
	string sAnswerAbs = "";
	string sAnswerImgUrl = "";
	string sAnswerUserUrl = "";
	string sAnswerUrl = "";
	TSUBTOPICTOPANSWERS stData;
		
	res=mysql_store_result(pmysql);
	while((row=mysql_fetch_row(res)))
	{		
		sSubid = row[0];
		sQuestionID = row[1];
		sAnswerID = row[2];
		sTopicID = row[3];
		sQuestionInfo = row[4];
		sQuestionUrl = row[5];
		sAgreeNum = row[6];
		sAnswerName = row[7];
		sAnswerIntro = row[8];
		sAnswerAbs = row[9];
		sAnswerImgUrl = row[10];
		sAnswerUserUrl = row[11];
		sAnswerUrl = row[12];
		
		if(sQuestionInfo.empty() || sQuestionUrl.empty() || sAnswerUrl.empty() || sAnswerAbs.empty())
			continue;
		
		stData.sSubID = sSubid;
		stData.sQuestionID = sQuestionID;
		stData.sAnswerID = sAnswerID;
		stData.sTopicID	= sTopicID;
		stData.sQuestionInfo = sQuestionInfo;
		stData.sQuestionUrl = sQuestionUrl;
		stData.sAgreeNum = sAgreeNum;
		stData.sAnswerName = sAnswerName;
		stData.sAnswerIntro = sAnswerIntro;
		stData.sAnswerAbs = sAnswerAbs;
		stData.sAnswerImgUrl = sAnswerImgUrl;
		stData.sAnswerUserUrl = sAnswerUserUrl;
		stData.sAnswerUrl = sAnswerUrl;
	
		v_stTopAnswers.push_back(stData);
	}

	return 0;
}

int mysql_select_t_user_hash(MYSQL *pmysql,vector<string> &v_stUserHash)
{
	MYSQL_RES *res;
	MYSQL_ROW row;
	
	char sql[] = "select en_name from %s order by update_time limit 500;";
	char query[5024]={0};

	sprintf(query,sql,TABLE_USER_HASH);
	
	if(mysql_real_query(pmysql,query,(unsigned int)strlen(query))!=0)
	{
		 cout<<__FUNCTION__<<" query failed"<<endl;  
		 return -1;
	}
	
	string sEnName = "";
		
	res=mysql_store_result(pmysql);
	while((row=mysql_fetch_row(res)))
	{		
		sEnName = row[0];
		
		if(sEnName.empty())
			continue;
	
		v_stUserHash.push_back(sEnName);
	}

	return 0; 
}

int mysql_select_t_top_answers_top(MYSQL *pmysql,multimap<unsigned,TSUBTOPICTOPANSWERS,greater<unsigned> > &mm_stTopAnswers,string &sTopicID)
{
	MYSQL_RES *res;
	MYSQL_ROW row;
	
	char sql[] = "select questionInfo,questionUrl,sAgreeNum,answerName,answerAbs,answerUserUrl,answerUrl from %s where topicId=%s group by questionId,answerId order by CONVERT(sAgreeNum,SIGNED) desc limit 50;";
	char sqlAll[] = "select questionInfo,questionUrl,sAgreeNum,answerName,answerAbs,answerUserUrl,answerUrl from %s group by questionId,answerId order by CONVERT(sAgreeNum,SIGNED) desc limit 50;";
	
	char query[5024]={0};

	if(sTopicID == "")
		sprintf(query,sqlAll,TABLE_TOP_ANSWERS);
	else
		sprintf(query,sql,TABLE_TOP_ANSWERS,sTopicID.c_str());
	
	mysql_query(pmysql,"set names utf8mb4");
	
	if(mysql_real_query(pmysql,query,(unsigned int)strlen(query))!=0)
	{
		 cout<<__FUNCTION__<<" query failed"<<endl;  
		 return -1;
	}

	string sQuestionInfo = "";
	string sQuestionUrl = "";
	string sAgreeNum = "";
	string sAnswerName = "";
	string sAnswerAbs = "";
	string sAnswerUserUrl = "";
	string sAnswerUrl = "";
	TSUBTOPICTOPANSWERS stData;
	unsigned uiAgreeNum = 0;
		
	res=mysql_store_result(pmysql);
	while((row=mysql_fetch_row(res)))
	{		
		sQuestionInfo = row[0];
		sQuestionUrl = row[1];
		sAgreeNum = row[2];
		sAnswerName = row[3];
		sAnswerAbs = row[4];
		sAnswerUserUrl = row[5];
		sAnswerUrl = row[6];
		
		if(sQuestionInfo.empty() || sAnswerUrl.empty() || sAgreeNum.empty())
			continue;
		  
		uiAgreeNum = extract_number(sAgreeNum);
		
		stData.sQuestionInfo = sQuestionInfo;
		stData.sQuestionUrl = sQuestionUrl;
		stData.sAgreeNum = sAgreeNum;
		stData.sAnswerName = sAnswerName;
		stData.sAnswerAbs = sAnswerAbs;
		stData.sAnswerUserUrl = sAnswerUserUrl;
		stData.sAnswerUrl = sAnswerUrl;
	
		mm_stTopAnswers.insert(make_pair<unsigned,TSUBTOPICTOPANSWERS>(uiAgreeNum,stData));
	}

	return 0;
}

int mysql_select_t_top_user_top_by_num(MYSQL *pmysql,multimap<unsigned,TUSERINFO,greater<unsigned> > &mm_stTopUsers,string &sRankFieldName)
{
	MYSQL_RES *res;
	MYSQL_ROW row;
	
	char sql[] = "select a.*,b.%s from %s a inner join (select id,%s from %s order by %s desc limit 50) as b on a.id=b.id;";

	char query[5024]={0};
	
	if(sRankFieldName.empty())
		return 0;

	sprintf(query,sql,sRankFieldName.c_str(),TABLE_USER_INFO,sRankFieldName.c_str(),TABLE_USER_JOIN,sRankFieldName.c_str());
	
	cout<<query<<endl;
	
	mysql_query(pmysql,"set names utf8mb4");
	
	if(mysql_real_query(pmysql,query,(unsigned int)strlen(query))!=0)
	{
		 cout<<__FUNCTION__<<" query failed"<<endl;  
		 return -1;
	}

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
	string sNum = "";
		
	res=mysql_store_result(pmysql);
	while((row=mysql_fetch_row(res)))
	{		
		sID = row[0];
		sENname = row[1];
		sZHname = row[2];
		sIconUrl = row[3];
		sIntroduce = row[4];
		sSex = row[5];
		sPlace = row[6];
		sVocation = row[7];
		sSchool = row[8];
		sMajor = row[9];
		sSkill = row[10];
		
		sNum = row[12];
		
		if(sID.empty() || sNum.empty() || sENname.empty())
			continue;
		  
		uiNum = extract_number(sNum);
		
		stData.sID = sID;
		stData.sENname = sENname;
		stData.sZHname = sZHname;
		stData.sIconUrl = sIconUrl;
		stData.sIntroduce = sIntroduce;
		stData.sSex = sSex;
		stData.sPlace = sPlace;
		stData.sVocation = sVocation;
		stData.sSchool = sSchool;
		stData.sMajor = sMajor;
		stData.sSkill = sSkill;
	
		mm_stTopUsers.insert(make_pair<unsigned,TUSERINFO>(uiNum,stData));
	}

	return 0;
}

int mysql_select_max_id_from_t_wp_posts(MYSQL *pmysql,unsigned &uiID)
{
	MYSQL_RES *res;
	MYSQL_ROW row;
	
	char sql[] = "select max(ID) from %s;";
	char query[1024]={0};

	sprintf(query,sql,TABLE_WP_POSTS);
	
	if(mysql_real_query(pmysql,query,(unsigned int)strlen(query))!=0)
	{
		 cout<<__FUNCTION__<<" query failed"<<endl;  
		 return -1;
	}
	
	string sID = "";
		
	res=mysql_store_result(pmysql);
	while((row=mysql_fetch_row(res)))
	{		
		sID = row[0];
		
		if(sID.empty())
			continue;
	
		uiID = atoi(sID.c_str());
	}

	return 0; 
}

int mysql_select_t_user_info_chart(MYSQL *pmysql,multimap<unsigned,string,greater<unsigned> > &mm_stChartData,string &sChartFieldName)
{
	MYSQL_RES *res;
	MYSQL_ROW row;
	
	char sql[] = "select %s,count(*) as num from %s group by %s order by num desc limit 21;";
	char sql2[] = "select %s,count(*) as num from %s group by replace(%s,'市','') order by num desc limit 21;";

	char query[1024]={0};
	
	if(sChartFieldName.empty())
		return 0;

	if(sChartFieldName == "place")
		sprintf(query,sql2,sChartFieldName.c_str(),TABLE_USER_INFO,sChartFieldName.c_str());
	else
		sprintf(query,sql,sChartFieldName.c_str(),TABLE_USER_INFO,sChartFieldName.c_str());
	
	cout<<query<<endl;
	
	mysql_query(pmysql,"set names utf8mb4");
	
	if(mysql_real_query(pmysql,query,(unsigned int)strlen(query))!=0)
	{
		 cout<<__FUNCTION__<<" query failed"<<endl;  
		 return -1;
	}

	string sName = "";
	unsigned uiNum = 0;
	string sNum = "";
		
	res=mysql_store_result(pmysql);
	while((row=mysql_fetch_row(res)))
	{		
		sName = row[0];
		sNum = row[1];
		
		if(sNum.empty())
			continue;
		
		if(sChartFieldName == "sex")
		{
			if(sName == "0")
				sName = "男";
			else if(sName == "1")
				sName = "女";
			else
				sName = "未知";
		}
			
		if(sName.empty())
			continue;
		
		uiNum = extract_number(sNum);
		
		mm_stChartData.insert(make_pair<unsigned,string>(uiNum,sName));
	}

	return 0;
}

int mysql_select_count_table(MYSQL *pmysql,const string& sTableName,unsigned &uiCount)
{
	MYSQL_RES *res;
	MYSQL_ROW row;
	
	char sql[] = "select count(*) from %s;";
	char query[1024]={0};
	
	if(sTableName.empty())
		return -1;

	sprintf(query,sql,sTableName.c_str());
	
	if(mysql_real_query(pmysql,query,(unsigned int)strlen(query))!=0)
	{
		 cout<<__FUNCTION__<<" query failed"<<endl;  
		 return -1;
	}
	
	string sCount = "";
		
	res=mysql_store_result(pmysql);
	while((row=mysql_fetch_row(res)))
	{		
		if(!row[0])
			continue;
		
		sCount = row[0];
		
		if(sCount.empty())
			continue;
	
		uiCount = extract_number(sCount);
	}

	return 0; 
}

int mysql_update_time(MYSQL *pmysql,vector<string> &v_stUserHash)
{
	if(v_stUserHash.empty())
	{
		cout<<"v_stUserHash empty"<<endl;
		return 0;
	}

	//更新update_time当前时间
	char sql[] = "update %s set update_time=null where en_name=\"%s\";";		

	char query[1024]={0};
	string slastQuery="";

	int iNum = 0;
	int i = 0;
	int iSize = v_stUserHash.size();
	string sEnName = "";

	string sOrgTable = TABLE_USER_HASH;
	
	mysql_query(pmysql,"START TRANSACTION");
	
	mysql_query(pmysql,"set names utf8mb4");
	
	vector<string>::iterator it = v_stUserHash.begin();
	for(;it!= v_stUserHash.end();it++)
	{
		i++;
	
		sEnName = (string)*it;
		
		if(sEnName.empty())
			continue;

		sprintf(query, sql,sOrgTable.c_str(),sEnName.c_str());
	
		slastQuery = query;	
		cout<<slastQuery<<endl;		
		
		if(mysql_real_query(pmysql,slastQuery.c_str(),(unsigned int)slastQuery.length())==0)
		{
			 iNum++;
			 cout<<i<<" for query successful"<<endl;
		}
		else
		{
			 fprintf(stderr,"query failed:Error:%s\n",mysql_error(pmysql));
		}
		
		if(iNum >= 100 || i >= iSize)
		{
			mysql_query(pmysql,"COMMIT");	
			iNum = 0;
		}
	}
	
	return 0;
}