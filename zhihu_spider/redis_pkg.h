#ifndef __REDIS_PKG_H_
#define __REDIS_PKG_H_

#include <hiredis/hiredis.h>
#include <string>
#include <queue>
#include <iostream>
using namespace std;
 
class KGRedisClient  
{  
public:  
    KGRedisClient(string ip, int port, int timeout = 2000);  
    virtual ~KGRedisClient();  
  
    bool ExecuteCmd(const char *cmd, string &response);  
    redisReply* ExecuteCmd(const char *cmd); 
  
private:  
    int m_timeout;  
    int m_serverPort;  
    string m_setverIp; 
    queue<redisContext *> m_clients;  
  
    time_t m_beginInvalidTime;  
    static const int m_maxReconnectInterval = 3;  
  
    redisContext* CreateContext();  
    void ReleaseContext(redisContext *ctx, bool active);  
    bool CheckStatus(redisContext *ctx);  
};  

#endif