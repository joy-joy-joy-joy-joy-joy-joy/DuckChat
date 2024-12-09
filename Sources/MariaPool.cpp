#include "MariaPool.h"

void MariaPool::Close() {
    {
        std::unique_lock<std::mutex> lock(mutex_);
        if (b_stop_) return;
        b_stop_ = true;
    }

    cond_.notify_all();

    // 等待所有活动连接归还
    std::unique_lock<std::mutex> lock(mutex_);
    close_cond_.wait(lock, [this] {
        return active_connections_ == 0;
    });

    // 清理连接池
    while (!pool_.empty()) {
        pool_.pop();
    }
}