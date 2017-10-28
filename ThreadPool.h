#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <vector>
#include <thread>
#include <atomic>
#include <queue>
#include <memory>

#include "TaskWrapper.h"

class ThreadPool
{
    std::atomic_bool finished_{ false };
    std::vector<std::thread> pool_;
    std::priority_queue<std::shared_ptr<TaskWrapper>> queue_;
    //std::priority_queue<std::shared_ptr<TaskWrapper>, std::vector<std::shared_ptr<TaskWrapper>>, bool(std::shared_ptr<TaskWrapper >, std::shared_ptr<TaskWrapper >)> cancellation_;

    void DoWork();
public:
    ThreadPool();
    ~ThreadPool();
};
#endif

