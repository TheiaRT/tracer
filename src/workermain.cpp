#include "collector.h"
#include "worker.h"


using namespace std;

static bool eq(char *a, char *b)
{
    return strcmp(a, b) == 0;
}

int main(int argc, char **argv)
{
    std::string host = "0.0.0.0";
    int port = 8000;
    for (int i = 0; i < argc; i++) {
        char *arg = argv[i];
        if (eq(arg, "-h") || eq(arg, "--host")) {
            host = string(argv[++i]);
        }
        else if (eq(arg, "-p") || eq(arg, "--port")) {
            port = atoi(argv[++i]);
        }
    }

    Worker w(host, port);
    while (w.get_work() == true) {
        if (w.trace_and_send_work() == false) {
            std::cerr << "Could not send work.\n";
        }
    }

    return 0;
}
