#pragma once

#include "../process/task.h"

class Scheduler {
public:
    Scheduler() = default;
    virtual ~Scheduler() = default; // Virtual destructor for proper cleanup of derived classes

    virtual void schedule() = 0;
    
    virtual void add(Task* t) = 0;
    virtual void remove(Task* t) = 0;
protected:
    virtual bool update(Task* t, int64_t time);
    virtual void requeue();
};