#ifndef WORK_QUEUE_H
#define WORK_QUEUE_H

#include <map>
#include <stdlib.h>


struct work_t {
    static const int INVALID_ID = -1;
    static const int NEW_ID = -2;

    int id;
    int top_x, top_y;
    int bot_x, bot_y;
    bool done;

    work_t() : id(INVALID_ID)
    {
    }

    work_t(int top_x, int top_y, int bot_x, int bot_y)
        : id(NEW_ID),
          top_x(top_x), top_y(top_y), bot_x(bot_x), bot_y(bot_y),
          done(false)
    {
    }

    static work_t invalid()
    {
        return work_t();
    }

    bool validp()
    {
        return id != INVALID_ID;
    }

    bool newp()
    {
        return id == NEW_ID;
    }

    bool donep()
    {
        return done;
    }
};

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
