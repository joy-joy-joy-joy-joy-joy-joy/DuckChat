#ifndef GATESERVER_MARIAPOOL_H
#define GATESERVER_MARIAPOOL_H

#include <string>
#include <queue>
#include <memory>
#include <mutex>
#include <atomic>
#include <iostream>
#include <mysql.h>  // MariaDB C API
#include <condition_variable>  // 添加这个
#include <stdexcept>  // 添加这个，用于std::runtime_error

class MariaPool {
public:
    // 先声明Close函数
    void Close();
    MariaPool(const std::string& host, const std::string& user,
              const std::string& pass, const std::string& schema, int poolSize)
            : host_(host), user_(user), pass_(pass), schema_(schema),
              poolSize_(poolSize), b_stop_(false), active_connections_(0) {
        try {
            for (int i = 0; i < poolSize_; ++i) {
                createConnection();
            }
        }
        catch (const std::runtime_error& e) {
            std::cout << "MariaDB pool init failed: " << e.what() << std::endl;
            Close();
            throw;
        }
    }

    std::unique_ptr<MYSQL, decltype(&mysql_close)> getConnection() {
        std::unique_lock<std::mutex> lock(mutex_);

        cond_.wait(lock, [this] {
            return b_stop_ || !pool_.empty();
        });

        if (b_stop_) {
            // 正确的方式创建一个空的unique_ptr
            return std::unique_ptr<MYSQL, decltype(&mysql_close)>{nullptr, &mysql_close};
        }

        auto con = std::move(pool_.front());
        pool_.pop();

        if (mysql_ping(con.get()) != 0) {
            con = createConnection();
        }
        active_connections_++;
        return con;
    }

    void returnConnection(std::unique_ptr<MYSQL, decltype(&mysql_close)> con) {
        if (!con) return;

        std::unique_lock<std::mutex> lock(mutex_);
        if (!b_stop_) {
            if (mysql_ping(con.get()) == 0) {
                pool_.push(std::move(con));
            } else {
                con = createConnection();
                pool_.push(std::move(con));
            }
            active_connections_--;
            cond_.notify_one();
        }
    }

private:
    std::unique_ptr<MYSQL, decltype(&mysql_close)> createConnection() {
        MYSQL* mysql = mysql_init(nullptr);
        if (!mysql) {
            throw std::runtime_error("mysql_init failed");
        }

        if (!mysql_real_connect(mysql, host_.c_str(), user_.c_str(),
                                pass_.c_str(), schema_.c_str(), 0, nullptr, 0)) {
            std::string error = mysql_error(mysql);
            mysql_close(mysql);
            throw std::runtime_error("Connection failed: " + error);
        }

        return std::unique_ptr<MYSQL, decltype(&mysql_close)>(mysql, &mysql_close);
    }

private:
    const std::string host_;
    const std::string user_;
    const std::string pass_;
    const std::string schema_;
    const int poolSize_;
    std::queue<std::unique_ptr<MYSQL, decltype(&mysql_close)>> pool_;
    std::mutex mutex_;
    std::condition_variable cond_;
    std::condition_variable close_cond_;
    std::atomic<bool> b_stop_;
    std::atomic<int> active_connections_;
};

#endif //GATESERVER_MARIAPOOL_H