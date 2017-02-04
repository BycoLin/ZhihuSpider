#ifndef _DEFINITION_H_
#define _DEFINITION_H_

#include <string>
using namespace std;

typedef struct sUserData
{
	sUserData()
	{
		sEName = "";
		sHid = "";
		iFowwers = 0;
		iFowwees = 0;
	}
	
	string sEName;
	string sHid;
	int iFowwees;
	int iFowwers;
}SUSERDATA;

typedef struct sUserInfoData
{
	sUserInfoData()
	{
		sEName = "";
		sHid = "";
		iFowwers = 0;
		iFowwees = 0;
		sZHname = "";
		iAgreeNum = 0;
		iThankNum = 0;
		iAsksNum = 0;
		iAnswersNum = 0;
		iPostsNum = 0;
		iCollectionsNum = 0;
		iPublicEditorNum = 0;
		sIconUrl = "";
		sIntroduce = "";
		sSex = "";
		sPlace = "";
		sVocation = "";
		sSchool = "";
		sMajor = "";
		sSkill = "";	
		iShareNum = 0;
		iAchCollectNum = 0;
	}
	
	string sEName;
	string sHid;
	int iFowwees;
	int iFowwers;
	string sZHname;
	string sIconUrl;
	string sIntroduce;
	string sSex;
	string sPlace;
	string sVocation;
	string sSchool;
	string sMajor;
	string sSkill;	
	int iAgreeNum;
	int iThankNum;
	int iAsksNum;
	int iAnswersNum;
	int iPostsNum;
	int iCollectionsNum;
	int iPublicEditorNum;
	int iShareNum;
	int iAchCollectNum;
}SUSERINFODATA;
  
typedef struct tDailyHotData
{	
	tDailyHotData()
	{
		sQuestionHref = "";
		sQuestionInfo = "";
		sPeopleHref = "";
		sPeopleName = "";
		sAgreeNum = "";
		sAnswerInfo = "";
	}

	string sQuestionHref;
	string sQuestionInfo;
	string sPeopleHref;
	string sPeopleName;
	string sAgreeNum;
	string sAnswerInfo;
}TDAILYHOTDATA;

//table struct
typedef struct tCookies
{
	tCookies()
	{
		sAccount = "";
		sPassword = "";
		sLoginUrl = "";
	}
	
	string sAccount;
	string sPassword;
	string sLoginUrl;
}TCOOKIES;

typedef struct tUserHash
{
	tUserHash()
	{
		sID = "";
		sENname = "";
		sZHname = "";
	}
	
	string sID;
	string sENname;
	string sZHname;
}TUSERHASH;

typedef struct tUserInfo
{
	tUserInfo()
	{
		sID = "";
		sENname = "";
		sZHname = "";
		sIconUrl = "";
		sIntroduce = "";
		sSex = "";
		sPlace = "";
		sVocation = "";
		sSchool = "";
		sMajor = "";
		sSkill = "";	
	}
	
	string sID;
	string sENname;
	string sZHname;
	string sIconUrl;
	string sIntroduce;
	string sSex;
	string sPlace;
	string sVocation;
	string sSchool;
	string sMajor;
	string sSkill;	
}TUSERINFO;

typedef struct tUserJoin
{
	tUserJoin()
	{
		sID = "";
		sENname = "";
		iAgreeNum = 0;
		iThankNum = 0;
		iAttentionNum = 0;
		iFansNum = 0;
		iAsksNum = 0;
		iAnswersNum = 0;
		iPostsNum = 0;
		iCollectionsNum = 0;
		iPublicEditorNum = 0;
		iShareNum = 0;
		iAchCollectNum = 0;
	}
	
	string sID;
	string sENname;
	int iAgreeNum;
	int iThankNum;
	int iAttentionNum;
	int iFansNum;
	int iAsksNum;
	int iAnswersNum;
	int iPostsNum;
	int iCollectionsNum;
	int iPublicEditorNum;
	int iShareNum;
	int iAchCollectNum;
}TUSERJOIN;

typedef struct tTopics
{
	tTopics()
	{
		iTopicID = 0;
		sTopicName = "";
		sTopicUrl = "";
	}
	
	int iTopicID;
	string sTopicName;
	string sTopicUrl;
}TTOPICS;

typedef struct tSubTopic
{
	tSubTopic()
	{
		iTopicID = 0;
		iSubID = 0;
		sSubName = "";
		sSubUrl = "";
	}
	
	int iTopicID;
	int iSubID;
	string sSubName;
	string sSubUrl;
}TSUBTOPIC;

typedef struct tSubTopicTopAnswers
{
	tSubTopicTopAnswers()
	{
		 sSubID = "";
		 sQuestionID = "";
		 sAnswerID = "";
		 sTopicID = "";
		 sQuestionInfo = "";
		 sQuestionUrl = "";
		 sAgreeNum = "";
		 sAnswerName = "";
		 sAnswerIntro = "";
		 sAnswerAbs = "";
		 sAnswerImgUrl = "";
		 sAnswerUserUrl = "";
		 sAnswerUrl = "";
	}
	
	string sSubID;
	string sQuestionID;
	string sAnswerID;
	string sTopicID;
	string sQuestionInfo;
	string sQuestionUrl;
	string sAgreeNum;
	string sAnswerName;
	string sAnswerIntro;
	string sAnswerAbs;//回答摘要
	string sAnswerImgUrl;
	string sAnswerUserUrl;
	string sAnswerUrl;
}TSUBTOPICTOPANSWERS;
		
typedef struct tWpPosts
{
	tWpPosts()
	{
		uiID = 0;
		uiPostAuthor = 0;
		sPostDate = "";
		sPostDateGmt = "";
		sPostContent = "";
		sPostTitle = "";
		sPostExcerpt = "";
		sPostStatus = "";
		sCommentStatus = "";
		sPingStatus = "";
		sPostPassWd = "";
		sPostName = "";
		sToPing = "";
		sPinged = "";
		sPostModified = "";
		sPostModifiedGmt = "";
		sPostContFiltered = "";
		uiPostParent = 0;
		sGuid = "";
		iMenuOrder = 0;
		sPostType = "";
		sPostMimeType = "";
		uiCommentCount = 0;
	}
	
	/*
	ID：自增唯一ID
	post_author：对应作者ID
	post_date：发布时间
	post_date_gmt：发布时间（GMT+0时间）
	post_content：正文
	post_title：标题
	post_excerpt：摘录
	post_status：文章状态（publish/auto-draft/inherit等）
	comment_status：评论状态（open/closed）
	ping_status：PING状态（open/closed）
	post_password：文章密码
	post_name：文章缩略名
	to_ping：未知
	pinged：已经PING过的链接
	post_modified：修改时间
	post_modified_gmt：修改时间（GMT+0时间）
	post_content_filtered：未知
	post_parent：父文章，主要用于PAGE
	guid：未知
	menu_order：排序ID
	post_type：文章类型（post/page等）
	post_mime_type：MIME类型
	comment_count：评论总数
	*/

	unsigned int uiID;
	unsigned int uiPostAuthor;
	string sPostDate;
	string sPostDateGmt;
	string sPostContent;
	string sPostTitle;
	string sPostExcerpt;
	string sPostStatus;
	string sCommentStatus;
	string sPingStatus;
	string sPostPassWd;
	string sPostName;
	string sToPing;
	string sPinged;
	string sPostModified;
	string sPostModifiedGmt;
	string sPostContFiltered;
	unsigned int uiPostParent;
	string sGuid;
	int iMenuOrder;
	string sPostType;
	string sPostMimeType;
	unsigned int uiCommentCount;
}TWPPOSTS; 

typedef struct tWpTermRelationShips
{
	tWpTermRelationShips()
	{
		uiObjectId = 0;
		uiTermTaId = 0;
		uiTermOrder = 0;
	}
	
	unsigned uiObjectId;
	unsigned uiTermTaId;
	unsigned uiTermOrder;
}TWPTERMRELATIONSHIPS;

#endif