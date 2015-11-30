#include "collector.h"
#include "worker.h"

int main()
{
    std::string host = "0.0.0.0";
    int port = 8000;

    Collector c("", 100, 100);
    c.start(host, port);

    Worker w(host, port);
    if (w.get_work()) {
        w.trace_and_send_work();
    }

    c.stop();
}
