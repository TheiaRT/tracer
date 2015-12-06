#include "collector.h"
#include "worker.h"
using namespace std;
int main()
{
    std::string host = "0.0.0.0";
    int port = 8000;

    Collector c("snowman.json", 400, 300, 12);
    c.start(host, port);
    c.finish();

    c.stop();
    c.write_image("snowman.pnm");
}
