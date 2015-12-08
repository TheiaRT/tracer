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

    int vp_width = 2000, vp_height = 1500;
    size_t splits = 4;

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
            port = atoi(argv[++i]);
        }
        else if (eq(arg, "-o") || eq(arg, "--outfile")) {
            out_file = string(argv[++i]);
        }
        else if (eq(arg, "-vpw") || eq(arg, "--viewport-width")) {
            vp_width = atoi(argv[++i]);
        }
        else if (eq(arg, "-vph") || eq(arg, "--viewport-height")) {
            vp_height = atoi(argv[++i]);
        }
        else if (eq(arg, "-spl") || eq(arg, "--split")) {
            splits = atoi(argv[++i]);
        }
    }

    Collector c(scene_name, vp_width, vp_height, splits);
    c.start(host, port);
    c.finish();

    c.stop();
    c.write_image(out_file);
}
