#include "TaskWrapper.h"
#include <memory>

TaskWrapper::TaskWrapper(int priority, int timeout, std::function<void(std::atomic_bool&)> f) :
    priority_(priority),
    timeout_(timeout),
    task_(f)
{
}


TaskWrapper::~TaskWrapper()
{
}

