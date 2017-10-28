#include "TaskWrapper.h"
#include <memory>


bool CmpTime(std::shared_ptr<TaskWrapper> left, std::shared_ptr<TaskWrapper> right)
{
    return left->endtime_ > right->endtime_;
}

TaskWrapper::TaskWrapper(int priority, int timeout, std::function<void(std::atomic_bool&)> f) :
    priority_(priority),
    timeout_(timeout),
    task_(f)
{
}


TaskWrapper::~TaskWrapper()
{
}

