#pragma once

#include "Scheduler.h"
#include "../process/task.h"
#include "../data_structures/list/CircList.h"

class RoundRobin : public Scheduler {
    private:
        CircList<Task *> processes;
    public:
        RoundRobin() = default;
        ~RoundRobin() override = default;

        void schedule(void) override;
        void add(Task* t) override;
        void remove(Task* t) override;

    protected:
        bool update(Task* t, int64_t time) override;
        void requeue(void) override;
};