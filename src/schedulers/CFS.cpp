#include <cstdlib>
#include <algorithm>
#include <random>

#include "CFS.h"
#include "../process/task.h"
#include "../data_structures/rb_tree/RBTree.h"

bool CFS::schedule(void) {
    RBNode<Task*>* node = this->processes.getLeftMost();
    if (!node)
        return false;
    
    Task* task = node->getValue();
    if (!task) {
        this->processes.remove(node);
        return this->processes.getLeftMost() != nullptr;
    }

    size_t taskWeight = prioNice::get_nice_weight(task->getNice());
    size_t totalWeight = this->processes.getTotalWeight();
    if (totalWeight == 0)
        return false;

    double time_slice = TARGET_LATENCY * (static_cast<double>(taskWeight) / totalWeight);
    if (time_slice <= 0) {
        this->processes.remove(node);
        return this->processes.getLeftMost() != nullptr;
    }

    // Randomize task given time like it is being interrupted mid run
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<size_t> dis(1, static_cast<size_t>(time_slice) + 1);
    size_t actual_runtime = dis(gen);

    size_t vruntime_increment = static_cast<size_t>(actual_runtime * (static_cast<double>(NICE_0_LOAD) / taskWeight));
    vruntime_increment = std::min(vruntime_increment, static_cast<size_t>(MAX_TIMESLICE));

    task->addVruntime(vruntime_increment);

    // Check if task should be removed (exceeded max CPU time)
    if (this->update(task, actual_runtime))
        this->processes.remove(node);
    else
        this->requeue();

    return this->processes.getLeftMost() != nullptr;
}


void CFS::add(Task* t) {
    if (t) {
        RBNode<Task*>* leftmost = this->processes.getLeftMost();
        if (leftmost) {
            t->setVruntime(leftmost->getValue()->getVruntime());
        }
        this->processes.insert(t);
    }
}

// Remove a task from scheduler
void CFS::remove(Task* t) {
    if (t)
        if (this->processes.remove(t))
            delete t;

}

// Returns wether we need to remove this process
bool CFS::update(Task* t, int64_t physical_time) {
    if (!t) return true;

	t->time += physical_time;
    return t->time > MAX_CPU_TIME;
}

// Move left most node to it's new place in the RBTree
void CFS::requeue(void) {
    RBNode<Task*>* node = this->processes.getLeftMost();
    if (!node)
        return;

    Task* task = node->getValue();
    if (!task)
        return;

    this->processes.remove(node);
    this->processes.insert(task);
}