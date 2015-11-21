/* #include <future>
   #include <chrono> */

#include "collector.h"


Collector::Collector(std::string scene) : scene(scene)
{
}

/* Unchecked runtime error if nthreads < 1. */
bool Collector::start(std::string host, int port, int nthreads=1)
{
    this->host = host;
    this->port = port;
    threads.reserve(nthreads);

    for (int i = 0; i < nthreads; i++) {
        threads[i] = std::thread(serve_work);
    }
}

void Collector::stop()
{
    for (std::thread t : threads) {
        /* TODO: try/wait for join, force kill after timeout */
        t.join();
    }
}

/* listen on socket, wait for connections */
/* TODO: add */
void Collector::serve_work()
{
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
