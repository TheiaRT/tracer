#ifndef WORK_QUEUE_H
#define WORK_QUEUE_H

#include <map>
#include <stdlib.h>

#include "work.h"


class WorkQueue {
public:
    WorkQueue();
    ~WorkQueue();
    void add(work_t work);
    work_t get();
    work_t remove(int id);

private:
    size_t current_id;
    size_t head, tail, size, capacity;
    int *array;
    std::map<int,work_t> work_map;

    int next_id();
    bool emptyp();
    bool fullp();
    work_t dequeue();
    void expand();
};

#endif
