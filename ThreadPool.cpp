#include "ThreadPool.h"
#include "TaskWrapper.h"



ThreadPool::ThreadPool():
finished_{ false },
pool_{},
queue_{}
//,cancellation_{&CmpTime}
{
    for(int i=0;i<10;i++)
            pool_.push_back(std::thread(&ThreadPool::DoWork,this));
}


ThreadPool::~ThreadPool()
{
    finished_ = true;
    for (auto& t : pool_) {
        if (t.joinable()) {
            t.join();
        }
    }
}

void ThreadPool::DoWork()
{
    while (!finished_) {
        //lock mutex
        if (queue_.empty()) {
            //wait for conditional var
        }
        //top , pop
        //unlock mutex

        // set endtime_
        // add to cancellation_
        // run task
    }
}