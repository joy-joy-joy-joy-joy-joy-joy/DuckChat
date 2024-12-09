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
            // ����ȱ�ٶ�replyΪ�յļ��
            /*
            if (reply->type == REDIS_REPLY_ERROR) {
                std::cout << "��֤ʧ��" << std::endl;
                freeReplyObject(reply);
                continue;  // ��������й¶��context
            }
            */

            // �޸ĺ�Ĵ��룺
            if (reply == nullptr || reply->type == REDIS_REPLY_ERROR) {
                std::cout << "��֤ʧ��" << std::endl;
                if (reply) freeReplyObject(reply);
                redisFree(context);  // �ͷ�����
                continue;
            }

            freeReplyObject(reply);
            std::cout << "��֤�ɹ�" << std::endl;
            connections_.push(context);
        }
    }

    // ������������û���ͷ�����
    /*
    ~RedisConPool() {
        std::lock_guard<std::mutex> lock(mutex_);
        while (!connections_.empty()) {
            connections_.pop();  // ����ֻ���Ƴ�����Ԫ�أ�û���ͷ��ڴ�
        }
    }
    */

    // �޸ĺ������������
    ~RedisConPool() {
        Close();  // ȷ����Դ����ȷ����
    }

    redisContext* getConnection() {
        std::unique_lock<std::mutex> lock(mutex_);
        // ���������ж��߼����Լ�
        /*
        cond_.wait(lock, [this] {
            if (b_stop_) {
                return true;
            }
            return !connections_.empty();
        });
        */

        // �޸ĺ�Ĵ��룺
        cond_.wait(lock, [this] {
            return b_stop_ || !connections_.empty();
        });

        // ���������жϲ�����
        /*
        if (b_stop_) {
            return nullptr;
        }
        */

        // �޸ĺ�Ĵ��룺
        if (b_stop_ && connections_.empty()) {
            return nullptr;
        }

        auto* context = connections_.front();
        connections_.pop();
        return context;
    }

    // ����û�д����ָ������ӳعر�ʱ���ڴ��ͷ�
    /*
    void returnConnection(redisContext* context) {
        std::lock_guard<std::mutex> lock(mutex_);
        if (b_stop_) {
            return;  // ����Ӧ���ͷ�����
        }
        connections_.push(context);
        cond_.notify_one();
    }
    */

    // �޸ĺ�Ĵ��룺
    void returnConnection(redisContext* context) {
        if (!context) return;

        std::lock_guard<std::mutex> lock(mutex_);
        if (b_stop_) {
            redisFree(context);  // �ͷ�����
            return;
        }
        connections_.push(context);
        cond_.notify_one();
    }

    // ����Closeû����ȷ�ͷ���Դ
    /*
    void Close() {
        b_stop_ = true;
        cond_.notify_all();
    }
    */

    // �޸ĺ�Ĵ��룺
    void Close() {
        {
            std::lock_guard<std::mutex> lock(mutex_);
            b_stop_ = true;
            while (!connections_.empty()) {
                auto* context = connections_.front();
                connections_.pop();
                redisFree(context);  // �ͷ���������
            }
        }
        cond_.notify_all();
    }

private:
    std::atomic<bool> b_stop_;  // ��ȷ��ʹ��atomic
    size_t poolSize_;
    const char* host_;
    int port_;
    std::queue<redisContext*> connections_;
    std::mutex mutex_;
    std::condition_variable cond_;
};


#endif //GATESERVER_REDISCONPOOL_H
