#ifndef TASK_WRAPPER_H
#define TASK_WRAPPER_H
#include <memory>
#include <atomic>
#include <functional>

class TaskWrapper;

using TW = std::shared_ptr<TaskWrapper>; //prefer to use unique_ptr - but it impossible to move out of queue without https://stackoverflow.com/a/27000256/8491726

class TaskWrapper
{
    int priority_;
    int timeout_;//timespan
    int endtime_;//now + timeout_ - set by start()
    std::function<void(std::atomic_bool&)> task_;//callable has periodically to check the flag passed by reference and exit when it is set to true
    std::atomic_bool cancalation_;
    bool finished_{false};
public:
    TaskWrapper() = delete;
    TaskWrapper(int priority, int timeout, std::function<void(std::atomic_bool&)> f);
    ~TaskWrapper();

    //used to order tasks in the calcellation queue by the endtime (from soonest to farest)
    bool CmpTime(const  TW& right) const
    {
        return endtime_ > right->endtime_;
    }

    //used to order tasks in the queue based on priority - from hiest to lowest
    friend bool operator< (const  TW& left, const  TW& right) 
    {
        return left->priority_ < right->priority_;
    }

    //run task in the currnt work thread
    void run() {
        cancalation_ = false;
        task_(cancalation_);
        finished_ = true;
    }

    //notify task_ that it shoudl exit due to timeout
    void abort()  noexcept
    {
        cancalation_ = true;
    }

    //is task finished/aborted?
    bool finished() const noexcept{
        return finished_;
    }

    //is task timeout expired?
    bool expired() const noexcept {
        //return endtime_ < now();
        return false;
    }

    //task is going to be started - calculate endtime based on timeout
    void start(){
        //endtime_ = now + timeout;
    }
};

#endif
