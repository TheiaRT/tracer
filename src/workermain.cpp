#include "collector.h"
#include "worker.h"


using namespace std;

int main(int argc, char **argv)
{
    std::string host = "0.0.0.0";
    int port = 8000;
    for (int i = 0; i < argc; i++) {
        if (strcmp(argv[i], "-h") == 0) {
            host = string(argv[++i]);
        }
        else if (strcmp(argv[i], "-p") == 0) {
            port = atoi(argv[++i]);
        }
    }
    Worker w(host, port);
    while (w.get_work() == true) {
        if (w.trace_and_send_work() == false) {
            std::cerr << "Could not send work.\n";
            //return 2;
        }
    }

    return 0;
}
