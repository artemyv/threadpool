#include "ThreadPool.h"


class SampleTask
{
    public:
        void operator()(std::atomic_bool& cancelled){
            while(!cancelled){
                //do part of the long task
            }
        }
};
int main()
{
    ThreadPool t;
    SampleTask f;

    t.AddTask(1,100,f);
}