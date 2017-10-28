#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <vector>
#include <thread>
#include <atomic>
#include <queue>
#include <memory>
#include <mutex>
#include <condition_variable>

#include "TaskWrapper.h"

class ThreadPool
{
    std::atomic_bool finished_{ false };
    std::vector<std::thread> pool_;
    std::priority_queue<std::shared_ptr<TaskWrapper>> queue_;
    std::mutex muq_; //prtotect queue_
    //std::priority_queue<std::shared_ptr<TaskWrapper>, std::vector<std::shared_ptr<TaskWrapper>>, bool(std::shared_ptr<TaskWrapper >, std::shared_ptr<TaskWrapper >)> cancellation_;
    std::priority_queue<std::shared_ptr<TaskWrapper>> cancellation_; //workaround
    std::mutex muc_; //protect cancellation_
    std::thread watcher_; //watch over cancellation queue
    std::condition_variable ready_; //notify working threads that queue is not empty

    void DoWork();
    void DoWatch();
public:
    ThreadPool();
    ~ThreadPool();
    void AddTask(int priority, int timeout, void (*f)(std::atomic_bool&));
};
#endif

