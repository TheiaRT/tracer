#include <cassert>

#include "work_queue.h"


int main()
{
    WorkQueue q;
    work_t w = work_t(1, 2, 3, 4);
    q.add(w);
    assert(q.get().bot_y == w.bot_y);
    assert(q.get().id == 0);
    assert(q.remove(0).bot_y == w.bot_y);

    return 0;
}
