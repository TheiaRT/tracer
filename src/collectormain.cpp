#include "collector.h"
#include "worker.h"

int main()
{
    std::string host = "0.0.0.0";
    int port = 8000;

    Collector c("snowman.json", 400, 300);
    c.start(host, port);

    Worker w(host, port);
    if (w.get_work() == false) {
        std::cerr << "No more work.\n";
        return 1;
    }

    if (w.trace_and_send_work() == false) {
        std::cerr << "Could not send work.\n";
        return 2;
    }

    c.stop();
    c.write_image("snowman.pnm");
}
