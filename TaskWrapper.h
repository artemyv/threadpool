#pragma once
#include <memory>
#include <atomic>

class TaskWrapper
{
    int priority_;
    int timeout_;//timespan
    int endtime_;//now + timespan
    void (*task_)(std::atomic_bool&);//callable
    std::atomic_bool cancalation_;
public:
    TaskWrapper() = delete;
    TaskWrapper(int priority, int timeout, void (*f)(std::atomic_bool&));
    ~TaskWrapper();

    friend bool CmpTime(std::shared_ptr<TaskWrapper> left, std::shared_ptr<TaskWrapper> right);

    friend bool operator< (std::shared_ptr<TaskWrapper> left, std::shared_ptr<TaskWrapper> right)
    {
        return left->priority_ < right->priority_;
    }

    void run() {
        cancalation_ = false;
        (*task_)(cancalation_);
    }
    void abort()
    {
        cancalation_ = true;
    }
};

bool CmpTime(std::shared_ptr<TaskWrapper> left, std::shared_ptr<TaskWrapper> right);