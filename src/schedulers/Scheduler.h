#pragma once

class Scheduler {
public:
    //void Scheduler(); // Constructor
    virtual void schedule() = 0; // Pure virtual function for scheduling tasks
    virtual ~Scheduler() = default; // Virtual destructor for proper cleanup of derived classes
};