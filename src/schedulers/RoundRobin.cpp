#include "RoundRobin.h"

bool RoundRobin::schedule(void) {
    if (!this->processes.getHead())
        return false;

    if (!this->sched_proc) {
        this->sched_proc = this->processes.getHead();
    }

    Task* t = this->sched_proc->getValue();
    if (this->update(t, this->time_slice)) {
        this->remove(this->sched_proc);
        this->sched_proc = nullptr;
    }
    
    this->requeue();
    return this->processes.getHead() != nullptr;
}

void RoundRobin::add(Task* t) {
    this->processes.insert(t);
}

void RoundRobin::remove(Task* t) {
    Node<Task *>* node = this->processes.search(t);

    if (node) {
        if (node == sched_proc) {
            sched_proc = sched_proc->getNext();
        }
        processes.remove(node);
    }
}

void RoundRobin::remove(Node<Task *>* node) {
    this->processes.remove(node);
}

bool RoundRobin::update(Task* t, int64_t time_slice) {
    if (!t) return true;

    t->time -= time_slice;
    return t->time <= 0;
}

void RoundRobin::requeue(void) {
    if (this->sched_proc && this->processes.getHead())
        this->sched_proc = this->sched_proc->getNext();
}