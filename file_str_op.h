#ifndef _FILE_STR_OP_H
#define _FILE_STR_OP_H

#include <string>
#include <map>
using namespace std;

#define BAIDU_DOCID "docid="
#define BAIDU_AND "&"

int get_file_stream(const string& strTmPath,ofstream &in);
unsigned extract_all_number(const string& str);
string& trim(std::string &s);
unsigned extract_number(const string& str);
string& replace_all(string& str,const string& old_value,const string& new_value);
bool if_digit(const string& sStr);
bool if_english(const string& sStr);
bool if_english_num(const string& sStr);
int english_to_upper(string& sStr);
bool if_no_english(const string& sStr);
bool if_part_english_same(const string& sStrA,const string& sStrB);
int rm_space_between_en_and_non_english(string& sStrPhrase);
int string_find_count(const string &str,const string &sValue);
int string_find_count_info(const string &str,const string &sValue,string *pSvalue,int iMax);
wstring s2ws(const string& s);
int digit_rail_digit_dispose(string& sFullName,const string& sRail);
int del_symbol_pair_info(string &sFullName,const string& sLeftSymbol,const string &sRightSymbol);
string UrlEncode(const std::string& szToEncode);

int copy_str_array(const string *strOrg,const int iOrgNum,string *strGoal,int *iGoalNum);
bool fGetFileDir(string& strFileDir);

string unicode2chinese(string& sInfo);
int filterEmojiStr(const string &source,string &sFilterStr);
int filterNoUTF8Str(const string &source,string &sFilterStr);

int get_baidu_docid(const string &sRetUrl,string &sDocID);
void IntToString(string& out, const int value);
string filterSpeStr(string& sInfo);
void insert_spe_sign_dispose(string &sOrgStr);

#endif