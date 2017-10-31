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


struct TimeComparator //compare tasks using endtime
{
    bool operator()(const TW& left, const TW& right) const {
        return left->CmpTime(right);
    }

};
class ThreadPool
{
    std::atomic_bool finished_{ false }; //notify all threads to exit

    std::vector<std::thread> pool_; //working thread pool
    std::priority_queue<TW> queue_; //priority queue of tasks to be run
    std::mutex muq_;                //protect queue_
    std::priority_queue<TW, std::vector<TW>, TimeComparator> cancellation_; //queue of runing tasks ordered by endtime
    std::mutex muc_;                                                        //protect cancellation_
    std::thread watcher_;                                                   //watch over cancellation queue
    std::condition_variable ready_;                                         //notify working threads that queue is not empty

    void DoWork();  //main working thread function
    void DoWatch(); //main cancellation watcher thread function
public:
    ThreadPool();
    ~ThreadPool();
    void AddTask(int priority, int timeout, std::function<void(std::atomic_bool&)> f); //add task to the queue
};
#endif

