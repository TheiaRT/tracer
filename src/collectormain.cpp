#include "collector.h"
#include "worker.h"

int main()
{
    Collector c("");
    /* this does not block */
    c.start("localhost", 8000, 5);

    Worker w("localhost", 8000);
    if (w.get_work()) {
        w.trace_and_send_work();
    }

    c.stop();
}
