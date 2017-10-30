#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <vector>
#include <thread>
#include <atomic>
#include <queue>
#include <memory>
#include <mutex>
#include <condition_variable>
#include <functional>

#include "TaskWrapper.h"

struct TimeComparator
{
    bool operator()(const std::shared_ptr<TaskWrapper>& left, const std::shared_ptr<TaskWrapper>& right) const {
        return left->CmpTime(right);
    }

};
class ThreadPool
{
    using TW = std::shared_ptr<TaskWrapper>;
    std::atomic_bool finished_{ false };
    std::vector<std::thread> pool_;
    std::priority_queue<TW> queue_;
    std::mutex muq_; //prtotect queue_
    std::priority_queue<TW, std::vector<TW>, TimeComparator> cancellation_;
    std::mutex muc_; //protect cancellation_
    std::thread watcher_; //watch over cancellation queue
    std::condition_variable ready_; //notify working threads that queue is not empty

    void DoWork();
    void DoWatch();
public:
    ThreadPool();
    ~ThreadPool();
    void AddTask(int priority, int timeout, std::function<void(std::atomic_bool&)> f);
};
#endif

