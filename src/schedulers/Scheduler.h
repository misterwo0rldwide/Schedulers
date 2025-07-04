#pragma once

#include "../process/task.h"

class Scheduler {
public:
    virtual ~Scheduler() = default;

    virtual bool schedule() = 0;
    virtual void add(Task* t) = 0;
    virtual void remove(Task* t) = 0;

protected:
    virtual bool update(Task* t, int64_t time) = 0;
    virtual void requeue() = 0;
};
