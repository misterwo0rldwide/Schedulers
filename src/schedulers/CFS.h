#pragma once

#include <cstdint>

#include "../data_structures/rb_tree/RBTree.h"
#include "Scheduler.h"

#define MAX_TIMESLICE (20)
#define TARGET_LATENCY (30)
#define NICE_0_LOAD (1024)

class CFS : public Scheduler {
    private:
        RBTree<Task*, TimeCmp> processes;
    public:
        CFS() = default;
        ~CFS() override = default;

        void schedule(void) override;
        void add(Task* t) override;
        void remove(Task* t) override;

    protected:
        bool update(Task* t, int64_t time) override;
        void requeue(void) override;
};

class prioNice {
public:
    static inline const size_t prio_weight[40] = {
        88817, 71054, 56843, 45474, 36379, 29103, 23283,
        18626, 14901, 11920, 9536, 7629, 6103, 4882,
        3906, 3124, 2499, 1999, 1599, 1279, 1024, 819,
        655, 524, 419, 335, 268, 214, 171, 137, 109, 87,
        70, 56, 45, 36, 28, 23, 18, 14
    };

    static size_t get_nice_weight(int8_t nice) { return prio_weight[nice + 20]; }
};