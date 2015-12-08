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

    int vp_width = 2000, vp_height = 1500;
    size_t num_splits = 4;

    std::string scene_name;
    std::string out_file;
    for (int i = 0; i < argc; i++) {
        char *arg = argv[i];
        if (eq(arg, "-s") || eq(arg, "--scene")) {
            scene_name = string(argv[++i]);
        }
        else if (eq(arg, "-h") || eq(arg, "--host")) {
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
        else if (eq(arg, "-o") || eq(arg, "--outfile")) {
            out_file = string(argv[++i]);
        }
        else if (eq(arg, "-vpw") || eq(arg, "--viewport-width")) {
            try {
                vp_width = stoi(argv[++i]);
            }
            catch(...) {
                error("Invalid viewport width.");
            }
        }
        else if (eq(arg, "-vph") || eq(arg, "--viewport-height")) {
            try {
                vp_height = stoi(argv[++i]);
            }
            catch(...) {
                error("Invalid viewport height.");
            }
        }
        else if (eq(arg, "-spl") || eq(arg, "--split")) {
            try {
                num_splits = stoi(argv[++i]);
            }
            catch(...) {
                error("Invalid number of splits.");
            }
        }
    }

    Collector c(scene_name, vp_width, vp_height, num_splits);
    c.start(host, port);
    c.finish();

    c.stop();
    c.write_image(out_file);
}
