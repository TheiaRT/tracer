#include "collector.h"
#include "worker.h"
using namespace std;
int main(int argc, char **argv)
{
    std::string host = "0.0.0.0";
    int port = 8000;

    int vp_width = 2000, vp_height = 1500;
    size_t splits = 4;

    std::string scene_name;
    std::string out_file;
    for (int i = 0; i < argc; i++) {
        if (strcmp(argv[i], "-s") == 0) {
            scene_name = string(argv[++i]);
        }
        else if (strcmp(argv[i], "-o") == 0) {
            out_file = argv[++i];
        }
        else if (strcmp(argv[i], "-vpw") == 0) {
            vp_width = atoi(argv[++i]);
        }
        else if (strcmp(argv[i], "-vph") == 0) {
            vp_height = atoi(argv[++i]);
        }
        else if (strcmp(argv[i], "-spl") == 0) {
            splits = atoi(argv[++i]);
        }
    }

    Collector c(scene_name, vp_width, vp_height, splits);
    c.start(host, port);
    c.finish();

    c.stop();
    c.write_image(out_file);
}
