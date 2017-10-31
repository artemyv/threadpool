#include "ThreadPool.h"
#include "TaskWrapper.h"




ThreadPool::ThreadPool():
finished_{ false },
pool_{},
queue_{}
, cancellation_{ TimeComparator() }
,watcher_(&ThreadPool::DoWatch,this)
{
    for(int i=0;i<10;i++)
            pool_.push_back(std::thread(&ThreadPool::DoWork,this));
}


ThreadPool::~ThreadPool()
{
    finished_ = true;
    ready_.notify_all();
    watcher_.join();
    for (auto& t : pool_) {
        if (t.joinable()) {
            t.join();
        }
    }
    while( !queue_.empty() ) queue_.pop();
}

void ThreadPool::DoWatch()
{
    while (!finished_) {
        //lock mutex
        if (!cancellation_.empty()) {
            auto& curr = cancellation_.top();
            if(curr->expired() && !curr->finished()){
                curr->abort();
                cancellation_.pop();
                continue;
            }
        }
        //unlock mutex
        
        //std::this_thread::sleep_for(100ms);
    }
    while(!cancellation_.empty()){
        cancellation_.top()->abort();
        cancellation_.pop();
    }
}
void ThreadPool::DoWork()
{
    while (!finished_) {
        //lock mutex
        if (queue_.empty()) {
            //wait for conditional var
            if(finished_){
                //unlock mutex;
                break;
            }
        }
        TW curr{queue_.top()};
        queue_.pop();
        //unlock mutex

        // set endtime_
        curr->start();
        cancellation_.push(std::move(curr));


        curr->run();
    }
}

void ThreadPool::AddTask(int priority, int timeout, std::function<void(std::atomic_bool&)> f)
{
    TW task = std::make_shared<TaskWrapper>(priority, timeout, f);
    //lock mutex
    bool notify = queue_.empty();
    queue_.push(std::move(task));
    if(notify) {
        ready_.notify_one();
    }
    //unlock mutex
}
