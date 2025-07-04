#pragma once

#include <cstdint>
#include <string>

#define MAX_CPU_TIME (1000)


class Task {
private:
    std::string name;
    int8_t nice;
    uint64_t vruntime;
public:
    // Specificly signed for negative values
    int64_t time;

    Task(std::string name, int8_t nice, uint64_t time = 0, uint64_t vruntime = 0)
        : name(name), nice(nice), vruntime(vruntime), time(time) {}
    ~Task() = default;

    std::string getName() const noexcept { return this->name; }
    int8_t getNice() const noexcept { return this->nice; }
    uint64_t getVruntime() const noexcept { return this->vruntime; }

    void setNice(int8_t nice) { this->nice = nice; }
    void setVruntime(uint64_t vrt) { this->vruntime = vrt; }
    void addVruntime(uint64_t vrt) { this->vruntime += vrt; }
};

struct TimeCmp {
    bool operator()(const Task* a, const Task* b) const {
        return a->getVruntime() < b->getVruntime();
    }
};