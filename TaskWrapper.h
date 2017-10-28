#ifndef TASK_WRAPPER_H
#define TASK_WRAPPER_H
#include <memory>
#include <atomic>
#include <functional>

class TaskWrapper
{
    int priority_;
    int timeout_;//timespan
    int endtime_;//now + timespan - set by start()
    std::function<void(std::atomic_bool&)> task_;//callable has periodically to check the flag passed by reference and exit when it is set to true
    std::atomic_bool cancalation_;
    bool finished_{false};
public:
    TaskWrapper() = delete;
    TaskWrapper(int priority, int timeout, std::function<void(std::atomic_bool&)> f);
    ~TaskWrapper();

    friend bool CmpTime(std::shared_ptr<TaskWrapper> left, std::shared_ptr<TaskWrapper> right);

    friend bool operator< (std::shared_ptr<TaskWrapper> left, std::shared_ptr<TaskWrapper> right)
    {
        return left->priority_ < right->priority_;
    }

    void run() {
        cancalation_ = false;
        task_(cancalation_);
        finished_ = true;
    }
    void abort()
    {
        cancalation_ = true;
    }
    bool finished(){
        return finished_;
    }
    bool expired(){
        //return endtime_ < now();
        return false;
    }
    void start(){
        //endtime_ = now + timeout;
    }
};

bool CmpTime(std::shared_ptr<TaskWrapper> left, std::shared_ptr<TaskWrapper> right);
#endif
