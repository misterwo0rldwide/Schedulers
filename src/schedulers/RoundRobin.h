#pragma once

#include "Scheduler.h"
#include "../process/task.h"
#include "../data_structures/list/CircList.h"

class RoundRobin : public Scheduler {
    private:
        CircList<Task *> processes;
        Node<Task *>*    sched_proc;
        int64_t          time_slice;
    public:
        RoundRobin(int64_t time_slice) : processes(nullptr), sched_proc(nullptr),  time_slice(time_slice) {}
        ~RoundRobin() override = default;

        bool schedule(void) override;
        void add(Task* t) override;
        void remove(Task* t) override;
        void remove(Node<Task *>* node);

    protected:
        bool update(Task* t, int64_t time) override;
        void requeue(void) override;
};