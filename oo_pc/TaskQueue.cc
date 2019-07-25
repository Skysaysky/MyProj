#include "TaskQueue.h"

#include <iostream>
using std::endl;
using std::cout;

TaskQueue::TaskQueue(size_t queSize)
    : _queSize(queSize)
    , _mutex()//无参构造函数
    , _notFull(_mutex)
    , _notEmpty(_mutex)
{}

bool TaskQueue::empty()const
{
    return _que.size() == 0;
}
bool TaskQueue::full() const
{
    return _que.size() == _queSize;
}
//运行在生产者线程
void TaskQueue::push(int number)
{
    MutexLockGuard autolock (_mutex);

    while(full()){//使用while 是为了防止出现虚假唤醒
        _notFull.wait();//当还是full 是异常唤醒，虚假唤醒
        //也就是唤醒之后没哟再一次判断，que 是不是full
        //应该在一次判断是不是满的状态
        //
    }
    
    _que.push(number);

    _notEmpty.notify();
}

//运行在消费者线程
int TaskQueue::pop()
{
    //RAII 
    MutexLockGuard autolock(_mutex);

    while(empty()){
        _notEmpty.wait();
    }
    
    int number = _que.front();
  
    _que.pop();
    
    _notFull.notify();
    
    return number;
}
