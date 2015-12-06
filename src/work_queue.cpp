#include "work_queue.h"


WorkQueue::WorkQueue()
    : current_id(0), head(0), tail(0), size(0), capacity(0),
      array(NULL)
{
}

WorkQueue::~WorkQueue()
{
    delete [] array;
}

void WorkQueue::add(work_t work)
{
    if (fullp()) {
        expand();
    }

    if (work.newp()) {
        int id = next_id();
        work.id = id;
        work_map.insert(std::make_pair(id, work));
    }

    array[tail] = work.id;
    tail = (tail + 1) % capacity;
    size++;
}

work_t WorkQueue::dequeue()
{
    if (emptyp()) {
        return work_t::invalid();
    }

    int id = array[head];
    head = (head + 1) % capacity;
    size--;

    if (work_map.find(id) == work_map.end()) {
        return work_t::invalid();
    }
    else {
        return work_map[id];
    }
}

work_t WorkQueue::get()
{
    work_t w;
    while ((w = dequeue()).validp()) {
        if (!w.donep()) {
            /* Got a work_t that has not yet been completed. */
            add(w);
            return w;
        }
    }

    /* Empty, so return invalid work_t. */
    return w;
}

work_t WorkQueue::remove(int id)
{
    work_map[id].done = true;
    return work_map[id];
}

int WorkQueue::next_id()
{
    return current_id++;
}

bool WorkQueue::emptyp()
{
    return size == 0;
}

bool WorkQueue::fullp()
{
    return size == capacity;
}

void WorkQueue::expand()
{
    size_t to_add = size;
    int *bigger_array = new int[capacity * 2 + 1];

    for (size_t i = head, j = 0; to_add > 0; to_add--, j++) {
        bigger_array[j] = array[i];
        i = (i + 1) % capacity;
    }

    capacity = capacity * 2 + 1;
    head = 0;
    tail = size;

    delete [] array;
    array = bigger_array;
}

bool WorkQueue::isdone(int id)
{
    return work_map[id].done;
}

void WorkQueue::split(work_t overall, size_t chunks)
{
    work_t *chunked_work = overall.split(chunks);

    for (size_t i = 0; i < chunks; i++) {
        add(chunked_work[i]);
    }
}
