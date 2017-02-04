#ifndef __REDIS_OP_H_
#define __REDIS_OP_H_

#include <vector>
#include <map>
#include <string>
#include "definition.h"
using namespace std;

#define REDIS_READY_QUEUE_KEY "zhihu_ready_q"
#define REDIS_ALREADY_HASH_KEY "zhihu_already_hash"
#define REDIS_SUB_TOPICS_QUEUE_KEY "zhihu_sub_topics_q"

#define REDIS_CMD_RPUSH "RPUSH"
#define REDIS_CMD_LLEN "LLEN"
#define REDIS_CMD_LPOP "LPOP"
#define REDIS_CMD_SADD "SADD"
#define REDIS_CMD_HEXISTS "HEXISTS"
#define REDIS_CMD_HSET "HSET"
#define REDIS_CMD_KEY_EXISTS "EXISTS"

#define SEG_SMYBOL "_"
 
int insert_redis_ready_pull_queue(vector<SUSERINFODATA> &v_stUserData);
int get_redis_ready_pull_queue(int iGetNum,map<string,SUSERINFODATA> &m_stUserData);
int get_redis_should_pull_queue(map<string,SUSERINFODATA> &m_stUserData,map<string,SUSERINFODATA> &m_stPullUserData);
int insert_redis_already_pull_hash(vector<SUSERINFODATA> &v_stUserData);

int get_list_llen(const string& sKeyName);
int insert_redis_sub_topics_queue(map<string,string> &m_stSubTopics);
int get_redis_sub_topics_queue(int iGetNum,map<string,string> &m_stSubTopics);

#endif