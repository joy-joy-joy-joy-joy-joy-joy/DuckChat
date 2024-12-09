#include "RedisMgr.h"
#include "global.h"
#include "RedisConPool.h"

RedisMgr::RedisMgr() {
    auto& gCfgMgr = ConfigMgr::Inst();
    auto host = gCfgMgr["Redis"]["Host"];
    auto port = gCfgMgr["Redis"]["Port"];
    auto pwd = gCfgMgr["Redis"]["Passwd"];

    if (host.empty() || port.empty() || pwd.empty()) {
        std::cout << "Failed to read Redis configuration" << std::endl;
        return;
    }

    std::cout << "Initializing Redis connection pool with:"
              << "\nHost: " << host
              << "\nPort: " << port << std::endl;

    _con_pool.reset(new RedisConPool(5, host.c_str(), atoi(port.c_str()), pwd.c_str()));
}

RedisMgr::~RedisMgr() {

}

bool RedisMgr::Get(const std::string& key, std::string& value)
{
    auto connect = _con_pool->getConnection();
    if (connect == nullptr) {
        return false;
    }
    auto reply = (redisReply*)redisCommand(connect, "GET %s", key.c_str());
    if (reply == NULL) {
        std::cout << "[ GET  " << key << " ] failed" << std::endl;
        freeReplyObject(reply);
        _con_pool->returnConnection(connect);
        return false;
    }

    if (reply->type != REDIS_REPLY_STRING) {
        std::cout << "[ GET  " << key << " ] failed" << std::endl;
        freeReplyObject(reply);
        _con_pool->returnConnection(connect);
        return false;
    }

    value = reply->str;
    freeReplyObject(reply);

    std::cout << "Succeed to execute command [ GET " << key << "  ]" << std::endl;
    _con_pool->returnConnection(connect);
    return true;
}

bool RedisMgr::Set(const std::string &key, const std::string &value) {
    auto connect = _con_pool->getConnection();
    if (connect == nullptr) {
        std::cout << "Failed to get Redis connection" << std::endl;
        return false;
    }

    this->_reply = (redisReply*)redisCommand(connect, "SET %s %s", key.c_str(), value.c_str());
    if (NULL == this->_reply) {
        std::cout << "Execute command [ SET " << key << "  "<< value << " ] failed" << std::endl;
        _con_pool->returnConnection(connect);
        return false;
    }

    bool success = (this->_reply->type == REDIS_REPLY_STATUS &&
                    (strcmp(this->_reply->str, "OK") == 0 || strcmp(this->_reply->str, "ok") == 0));

    freeReplyObject(this->_reply);
    _con_pool->returnConnection(connect);

    if (success) {
        std::cout << "Execute command [ SET " << key << "  " << value << " ] success" << std::endl;
    }

    return success;
}

bool RedisMgr::LPush(const std::string &key, const std::string &value) {
    auto connect = _con_pool->getConnection();
    if (connect == nullptr) {
        std::cout << "Failed to get Redis connection for LPUSH command" << std::endl;
        return false;
    }

    auto reply = (redisReply*)redisCommand(connect, "LPUSH %s %s",
                                           key.c_str(), value.c_str());

    // 检查命令执行结果
    if (reply == nullptr) {
        std::cout << "Execute command [ LPUSH " << key << " " << value
                  << " ] failed: null reply" << std::endl;
        _con_pool->returnConnection(connect);
        return false;
    }

    // 检查返回类型和值
    if (reply->type != REDIS_REPLY_INTEGER || reply->integer <= 0) {
        std::cout << "Execute command [ LPUSH " << key << " " << value
                  << " ] failed: " << (reply->type != REDIS_REPLY_INTEGER ?
                                       "wrong type" : "invalid length") << std::endl;
        freeReplyObject(reply);
        _con_pool->returnConnection(connect);
        return false;
    }

    std::cout << "Execute command [ LPUSH " << key << " " << value
              << " ] success: list length = " << reply->integer << std::endl;

    freeReplyObject(reply);
    _con_pool->returnConnection(connect);
    return true;
}

bool RedisMgr::LPop(const std::string &key, std::string& value) {
    // 1. 获取连接
    auto connect = _con_pool->getConnection();
    if (connect == nullptr) {
        std::cout << "Failed to get Redis connection for LPOP command" << std::endl;
        return false;
    }

    // 2. 执行命令
    auto reply = (redisReply*)redisCommand(connect, "LPOP %s", key.c_str());

    // 3. 检查命令执行结果
    if (reply == nullptr) {
        std::cout << "Execute command [ LPOP " << key << " ] failed: null reply" << std::endl;
        _con_pool->returnConnection(connect);
        return false;
    }

    // 4. 检查返回类型
    if (reply->type == REDIS_REPLY_NIL) {
        std::cout << "Execute command [ LPOP " << key << " ] notice: list empty or not exists" << std::endl;
        freeReplyObject(reply);
        _con_pool->returnConnection(connect);
        return false;
    }

    // 5. 确保返回的是字符串类型
    if (reply->type != REDIS_REPLY_STRING) {
        std::cout << "Execute command [ LPOP " << key << " ] failed: wrong type" << std::endl;
        freeReplyObject(reply);
        _con_pool->returnConnection(connect);
        return false;
    }

    // 6. 安全地复制值
    if (reply->str != nullptr) {
        value = std::string(reply->str, reply->len);
        std::cout << "Execute command [ LPOP " << key << " ] success: value = "
                  << value << std::endl;
    } else {
        std::cout << "Execute command [ LPOP " << key << " ] failed: null string" << std::endl;
        freeReplyObject(reply);
        _con_pool->returnConnection(connect);
        return false;
    }

    // 7. 释放资源
    freeReplyObject(reply);
    _con_pool->returnConnection(connect);
    return true;
}

bool RedisMgr::RPush(const std::string& key, const std::string& value) {
    // 1. 获取连接
    auto connect = _con_pool->getConnection();
    if (connect == nullptr) {
        std::cout << "Failed to get Redis connection for RPUSH command" << std::endl;
        return false;
    }

    // 2. 执行命令
    auto reply = (redisReply*)redisCommand(connect, "RPUSH %s %s",
                                           key.c_str(), value.c_str());

    // 3. 检查命令执行结果
    if (reply == nullptr) {
        std::cout << "Execute command [ RPUSH " << key << " " << value
                  << " ] failed: null reply" << std::endl;
        _con_pool->returnConnection(connect);
        return false;
    }

    // 4. 检查返回类型和值
    bool success = false;
    switch (reply->type) {
        case REDIS_REPLY_INTEGER:
            if (reply->integer > 0) {
                std::cout << "Execute command [ RPUSH " << key << " " << value
                          << " ] success: list length = " << reply->integer << std::endl;
                success = true;
            } else {
                std::cout << "Execute command [ RPUSH " << key << " " << value
                          << " ] failed: invalid length" << std::endl;
            }
            break;

        case REDIS_REPLY_ERROR:
            std::cout << "Execute command [ RPUSH " << key << " " << value
                      << " ] failed: " << reply->str << std::endl;
            break;

        default:
            std::cout << "Execute command [ RPUSH " << key << " " << value
                      << " ] failed: unexpected type " << reply->type << std::endl;
            break;
    }

    // 5. 释放资源
    freeReplyObject(reply);
    _con_pool->returnConnection(connect);

    return success;
}

bool RedisMgr::RPop(const std::string& key, std::string& value) {
    // 1. 获取连接
    auto connect = _con_pool->getConnection();
    if (connect == nullptr) {
        std::cout << "Failed to get Redis connection for RPOP command" << std::endl;
        return false;
    }

    // 2. 执行命令
    auto reply = (redisReply*)redisCommand(connect, "RPOP %s", key.c_str());

    // 3. 检查命令执行结果
    if (reply == nullptr) {
        std::cout << "Execute command [ RPOP " << key << " ] failed: null reply" << std::endl;
        _con_pool->returnConnection(connect);
        return false;
    }

    // 4. 检查返回类型和处理结果
    bool success = false;
    switch (reply->type) {
        case REDIS_REPLY_STRING:
            if (reply->str != nullptr) {
                value.assign(reply->str, reply->len);  // 安全地复制字符串
                std::cout << "Execute command [ RPOP " << key
                          << " ] success: value = " << value << std::endl;
                success = true;
            } else {
                std::cout << "Execute command [ RPOP " << key
                          << " ] failed: null string" << std::endl;
            }
            break;

        case REDIS_REPLY_NIL:
            std::cout << "Execute command [ RPOP " << key
                      << " ] notice: list empty or not exists" << std::endl;
            break;

        case REDIS_REPLY_ERROR:
            std::cout << "Execute command [ RPOP " << key
                      << " ] failed: " << reply->str << std::endl;
            break;

        default:
            std::cout << "Execute command [ RPOP " << key
                      << " ] failed: unexpected type " << reply->type << std::endl;
            break;
    }

    // 5. 释放资源
    freeReplyObject(reply);
    _con_pool->returnConnection(connect);

    return success;
}

bool RedisMgr::HSet(const std::string &key, const std::string &hkey, const std::string &value) {
    auto connect = _con_pool->getConnection();
    if (connect == nullptr) {
        std::cout << "Failed to get Redis connection" << std::endl;
        return false;
    }

    auto reply = (redisReply*)redisCommand(connect, "HSET %s %s %s",
                                           key.c_str(), hkey.c_str(), value.c_str());
    if (reply == nullptr || reply->type != REDIS_REPLY_INTEGER) {
        std::cout << "Execute command [ HSet " << key << "  " << hkey
                  << "  " << value << " ] failed" << std::endl;
        if (reply) freeReplyObject(reply);
        _con_pool->returnConnection(connect);
        return false;
    }

    std::cout << "Execute command [ HSet " << key << "  " << hkey
              << "  " << value << " ] success" << std::endl;
    freeReplyObject(reply);
    _con_pool->returnConnection(connect);
    return true;
}

bool RedisMgr::HSet(const char* key, const char* hkey, const char* hvalue, size_t hvaluelen) {
    // 1. 参数验证
    if (!key || !hkey || !hvalue) {
        std::cout << "Invalid parameters: null pointer(s)" << std::endl;
        return false;
    }

    // 2. 获取连接
    auto connect = _con_pool->getConnection();
    if (connect == nullptr) {
        std::cout << "Failed to get Redis connection for HSET command" << std::endl;
        return false;
    }

    // 3. 准备命令参数
    const char* argv[4];
    size_t argvlen[4];
    try {
        argv[0] = "HSET";
        argvlen[0] = 4;
        argv[1] = key;
        argvlen[1] = strlen(key);
        argv[2] = hkey;
        argvlen[2] = strlen(hkey);
        argv[3] = hvalue;
        argvlen[3] = hvaluelen;
    } catch (const std::exception& e) {
        std::cout << "Failed to prepare HSET command: " << e.what() << std::endl;
        _con_pool->returnConnection(connect);
        return false;
    }

    // 4. 执行命令
    auto reply = (redisReply*)redisCommandArgv(connect, 4, argv, argvlen);

    // 5. 检查命令执行结果
    bool success = false;
    if (reply == nullptr) {
        std::cout << "Execute command [ HSET " << key << " " << hkey
                  << " ] failed: null reply" << std::endl;
    } else {
        switch (reply->type) {
            case REDIS_REPLY_INTEGER:
                // 0: 字段已存在，值被更新
                // 1: 新字段被创建并设置值
                success = true;
                std::cout << "Execute command [ HSET " << key << " " << hkey
                          << " ] success: " << (reply->integer == 1 ? "created" : "updated")
                          << std::endl;
                break;

            case REDIS_REPLY_ERROR:
                std::cout << "Execute command [ HSET " << key << " " << hkey
                          << " ] failed: " << reply->str << std::endl;
                break;

            default:
                std::cout << "Execute command [ HSET " << key << " " << hkey
                          << " ] failed: unexpected type " << reply->type << std::endl;
                break;
        }
        freeReplyObject(reply);
    }

    // 6. 释放资源
    _con_pool->returnConnection(connect);
    return success;
}

std::string RedisMgr::HGet(const std::string &key, const std::string &hkey) {
    auto connect = _con_pool->getConnection();
    if (connect == nullptr) {
        std::cout << "Failed to get Redis connection" << std::endl;
        return "";
    }

    const char* argv[3];
    size_t argvlen[3];
    argv[0] = "HGET";
    argvlen[0] = 4;
    argv[1] = key.c_str();
    argvlen[1] = key.length();
    argv[2] = hkey.c_str();
    argvlen[2] = hkey.length();

    auto reply = (redisReply*)redisCommandArgv(connect, 3, argv, argvlen);

    // 1. 检查reply是否为空
    if (reply == nullptr) {
        std::cout << "Execute command [ HGet " << key << " " << hkey << " ] failed: null reply" << std::endl;
        _con_pool->returnConnection(connect);
        return "";
    }

    // 2. 检查reply类型
    if (reply->type == REDIS_REPLY_NIL) {
        std::cout << "Execute command [ HGet " << key << " " << hkey << " ] failed: key not found" << std::endl;
        freeReplyObject(reply);
        _con_pool->returnConnection(connect);
        return "";
    }

    // 3. 确保返回的是字符串类型
    if (reply->type != REDIS_REPLY_STRING) {
        std::cout << "Execute command [ HGet " << key << " " << hkey << " ] failed: wrong type" << std::endl;
        freeReplyObject(reply);
        _con_pool->returnConnection(connect);
        return "";
    }

    // 4. 检查字符串内容
    if (reply->str == nullptr) {
        std::cout << "Execute command [ HGet " << key << " " << hkey << " ] failed: null string" << std::endl;
        freeReplyObject(reply);
        _con_pool->returnConnection(connect);
        return "";
    }

    // 5. 安全地复制字符串
    std::string value(reply->str, reply->len);

    freeReplyObject(reply);
    std::cout << "Execute command [ HGet " << key << " " << hkey << " ] success" << std::endl;
    _con_pool->returnConnection(connect);

    return value;
}

bool RedisMgr::Del(const std::string &key) {
    // 1. 获取连接
    auto connect = _con_pool->getConnection();
    if (connect == nullptr) {
        std::cout << "Failed to get Redis connection for DEL command" << std::endl;
        return false;
    }

    // 2. 执行命令
    auto reply = (redisReply*)redisCommand(connect, "DEL %s", key.c_str());

    // 3. 检查命令执行结果
    if (reply == nullptr) {
        std::cout << "Execute command [ DEL " << key << " ] failed: null reply" << std::endl;
        _con_pool->returnConnection(connect);
        return false;
    }

    // 4. 检查返回类型和值
    if (reply->type != REDIS_REPLY_INTEGER) {
        std::cout << "Execute command [ DEL " << key << " ] failed: wrong type" << std::endl;
        freeReplyObject(reply);
        _con_pool->returnConnection(connect);
        return false;
    }

    // 5. 检查删除结果
    bool success = (reply->integer > 0);
    if (success) {
        std::cout << "Execute command [ DEL " << key << " ] success: deleted "
                  << reply->integer << " key(s)" << std::endl;
    } else {
        std::cout << "Execute command [ DEL " << key << " ] notice: key not found" << std::endl;
    }

    // 6. 释放资源
    freeReplyObject(reply);
    _con_pool->returnConnection(connect);

    return success;
}

bool RedisMgr::ExistsKey(const std::string &key) {
    auto connect = _con_pool->getConnection();
    if (connect == nullptr) {
        std::cout << "Failed to get Redis connection" << std::endl;
        return false;
    }

    auto reply = (redisReply*)redisCommand(connect, "EXISTS %s", key.c_str());

    // 检查reply是否为空
    if (reply == nullptr) {
        std::cout << "Execute command [ EXISTS " << key << " ] failed: null reply" << std::endl;
        _con_pool->returnConnection(connect);
        return false;
    }

    // 检查返回类型
    if (reply->type != REDIS_REPLY_INTEGER) {
        std::cout << "Execute command [ EXISTS " << key << " ] failed: wrong type" << std::endl;
        freeReplyObject(reply);
        _con_pool->returnConnection(connect);
        return false;
    }

    bool exists = (reply->integer != 0);

    if (exists) {
        std::cout << "Key [ " << key << " ] exists" << std::endl;
    } else {
        std::cout << "Key [ " << key << " ] not found" << std::endl;
    }

    freeReplyObject(reply);
    _con_pool->returnConnection(connect);
    return exists;
}

void RedisMgr::Close()
{
    redisFree(_connect);
}