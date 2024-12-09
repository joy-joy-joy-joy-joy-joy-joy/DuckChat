//
// Created by w2552 on 2024/11/12.
//

#ifndef GATESERVER_LOGINSYSTEM_H
#define GATESERVER_LOGINSYSTEM_H

#include "Singleton.h"
#include <functional>
#include <map>
#include "global.h"

class HttpConnection;
typedef std::function<void(std::shared_ptr<HttpConnection>)> HttpHandler;
class LogicSystem :public Singleton<LogicSystem>
{
    friend class Singleton<LogicSystem>;
public:
    ~LogicSystem();
    bool HandleGet(std::string, std::shared_ptr<HttpConnection>);
    void RegGet(std::string, HttpHandler handler);
    void RegPost(std::string, HttpHandler handler);
    bool HandlePost(std::string, std::shared_ptr<HttpConnection>);
private:
    LogicSystem();
    std::map<std::string, HttpHandler> _post_handlers;
    std::map<std::string, HttpHandler> _get_handlers;
};



#endif //GATESERVER_LOGINSYSTEM_H
