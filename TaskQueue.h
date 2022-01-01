#pragma once


#include "sthread.h"
#include <iostream>
#include <queue>

typedef void (*handler_t) (void *); 

struct Task {
    handler_t handler;
    void* arg; //request
};

/*
 * ------------------------------------------------------------------
 * TaskQueue --
 * 
 *      A thread-safe task queue. This queue should be implemented
 *      as a monitor.
 *
 * ------------------------------------------------------------------
 */
class TaskQueue {
    private:
    // TODO: More needed here.
    smutex_t mutex;
    scond_t nonempty; //condition variable nonempty
    std::queue<Task> task_q; //task queue

    public:
    TaskQueue();
    ~TaskQueue();
    
    // no default copy constructor and assignment operators. this will prevent some
    // painful bugs by converting them into compiler errors.
    TaskQueue(const TaskQueue&) = delete; 
    TaskQueue& operator=(const TaskQueue &) = delete;

    void enqueue(Task task);
    Task dequeue();

    private:
    int size();
    bool empty();
};

