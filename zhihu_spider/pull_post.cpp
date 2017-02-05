#include "pull_post.h"


int posts_format(string &sContent,TDAILYHOTDATA &stHotData)
{
	string sAnswerTmp = "<a href=\"" + stHotData.sQuestionHref + "\"" +" target=\"_blank\" style=\"font-weight: bold;\">"+stHotData.sQuestionInfo+"</a>";
	
	string sPeopleTmp = "<a href=\"" + stHotData.sPeopleHref + "\"" + " target=\"_blank\">" + stHotData.sPeopleName +"</a>:";
	
	string sAgreeTmp = "<span class=\"agreetext\"> ("+stHotData.sAgreeNum+"赞) </span>";
	
	string sReadMoreTmp = "<span class=\"readmore\"><a href=\"" +stHotData.sQuestionHref+ "\""+ " target=\"_blank\">[阅读全文]</a></span>";
	
	sContent = sContent+"<div class=\"entry\"><div class=\"entry-inner\"><ul><li><h3>" + sAnswerTmp + "</h3><p><span class=\"summary\">" + sPeopleTmp + sAgreeTmp + stHotData.sAnswerInfo + sReadMoreTmp + "</span></li><div class=\"cleardiv\"></div></ul></div></div>";
	
	return 0; 
} 
