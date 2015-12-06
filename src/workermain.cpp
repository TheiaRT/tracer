#include "collector.h"
#include "worker.h"
using namespace std;
int main()
{
    std::string host = "0.0.0.0";
    int port = 8000;
    Worker w(host, port);
    if (w.get_work() == false) {
        std::cerr << "No more work.\n";
        return 1;
    }

    if (w.trace_and_send_work() == false) {
        std::cerr << "Could not send work.\n";
        return 2;
    }

}
