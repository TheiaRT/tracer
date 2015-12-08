#include <stdlib.h>
#include <string.h>

#include "collector.h"
#include "worker.h"


using namespace std;

static bool eq(char *a, const char *b)
{
    return strcmp(a, b) == 0;
}

static void error(const char *msg)
{
    std::cerr << msg << std::endl;
    exit(EXIT_FAILURE);
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
            try {
                port = stoi(argv[++i]);
            }
            catch(...) {
                error("Invalid port.");
            }
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
