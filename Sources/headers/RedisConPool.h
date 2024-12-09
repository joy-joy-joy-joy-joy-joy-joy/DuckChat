#ifndef GATESERVER_REDISCONPOOL_H
#define GATESERVER_REDISCONPOOL_H

#include "Singleton.h"
#include <sw/redis++/redis++.h>
#include <iostream>
#include <string>
#include <queue>
#include <atomic>

class RedisConPool {
public:
    RedisConPool(size_t poolSize, const char* host, int port, const char* pwd)
            : poolSize_(poolSize), host_(host), port_(port), b_stop_(false) {
        for (size_t i = 0; i < poolSize_; ++i) {
            auto* context = redisConnect(host, port);
            if (context == nullptr || context->err != 0) {
                if (context != nullptr) {
                    redisFree(context);
                }
                continue;
            }

            auto reply = (redisReply*)redisCommand(context, "AUTH %s", pwd);
            // 错误：缺少对reply为空的检查
            /*
            if (reply->type == REDIS_REPLY_ERROR) {
                std::cout << "认证失败" << std::endl;
                freeReplyObject(reply);
                continue;  // 错误：这里泄露了context
            }
            */

            // 修改后的代码：
            if (reply == nullptr || reply->type == REDIS_REPLY_ERROR) {
                std::cout << "认证失败" << std::endl;
                if (reply) freeReplyObject(reply);
                redisFree(context);  // 释放连接
                continue;
            }

            freeReplyObject(reply);
            std::cout << "认证成功" << std::endl;
            connections_.push(context);
        }
    }

    // 错误：析构函数没有释放连接
    /*
    ~RedisConPool() {
        std::lock_guard<std::mutex> lock(mutex_);
        while (!connections_.empty()) {
            connections_.pop();  // 错误：只是移除队列元素，没有释放内存
        }
    }
    */

    // 修改后的析构函数：
    ~RedisConPool() {
        Close();  // 确保资源被正确清理
    }

    redisContext* getConnection() {
        std::unique_lock<std::mutex> lock(mutex_);
        // 错误：条件判断逻辑可以简化
        /*
        cond_.wait(lock, [this] {
            if (b_stop_) {
                return true;
            }
            return !connections_.empty();
        });
        */

        // 修改后的代码：
        cond_.wait(lock, [this] {
            return b_stop_ || !connections_.empty();
        });

        // 错误：条件判断不完整
        /*
        if (b_stop_) {
            return nullptr;
        }
        */

        // 修改后的代码：
        if (b_stop_ && connections_.empty()) {
            return nullptr;
        }

        auto* context = connections_.front();
        connections_.pop();
        return context;
    }

    // 错误：没有处理空指针和连接池关闭时的内存释放
    /*
    void returnConnection(redisContext* context) {
        std::lock_guard<std::mutex> lock(mutex_);
        if (b_stop_) {
            return;  // 错误：应该释放连接
        }
        connections_.push(context);
        cond_.notify_one();
    }
    */

    // 修改后的代码：
    void returnConnection(redisContext* context) {
        if (!context) return;

        std::lock_guard<std::mutex> lock(mutex_);
        if (b_stop_) {
            redisFree(context);  // 释放连接
            return;
        }
        connections_.push(context);
        cond_.notify_one();
    }

    // 错误：Close没有正确释放资源
    /*
    void Close() {
        b_stop_ = true;
        cond_.notify_all();
    }
    */

    // 修改后的代码：
    void Close() {
        {
            std::lock_guard<std::mutex> lock(mutex_);
            b_stop_ = true;
            while (!connections_.empty()) {
                auto* context = connections_.front();
                connections_.pop();
                redisFree(context);  // 释放所有连接
            }
        }
        cond_.notify_all();
    }

private:
    std::atomic<bool> b_stop_;  // 正确：使用atomic
    size_t poolSize_;
    const char* host_;
    int port_;
    std::queue<redisContext*> connections_;
    std::mutex mutex_;
    std::condition_variable cond_;
};


#endif //GATESERVER_REDISCONPOOL_H
