#include <functional>

#include "collector.h"


Collector::Collector(std::string scene, size_t width, size_t height)
    : scene(scene),
      pixmap(PnmImage(width, height))
{
    server = new TCPServer([=](std::string req) {
        return this->serve_request(req);
    });
}

bool Collector::start(std::string host, int port)
{
    return server->start(host, port);
}

void Collector::stop()
{
    server->stop();
}

std::string Collector::serve_request(std::string req)
{
    if (req == "HAVE WORK") {
        return process_work(req) ? "true" : "false";
    }
    else if (req == "NEED WORK") {
        return generate_work();
    }
    else {
        return "BAD REQUEST";
    }
}

/* TODO: remove sample */
std::string Collector::generate_work()
{
    return "{\"coords\": [{\"x\": 4, \"y\": 5}]}";
}

/* parse work, store in pixmap */
/* TODO: remove sample */
bool Collector::process_work(std::string work)
{
    return true;
}
