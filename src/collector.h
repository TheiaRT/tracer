#ifndef COLLECTOR_H
#define COLLECTOR_H

#include <iostream>
#include <vector>
#include <thread>
#include <queue>

#include "pnm_image.h"
#include "vector.h"
#include "tcp_server.h"


class Collector {
public:
    Collector(std::string scene, size_t width, size_t height);
    bool start(std::string host, int port);
    void stop();

private:
    std::string scene;
    PnmImage pixmap;
    std::queue<vector2_t> work_queue;
    TCPServer *server;

    /* in a thread, listens and accepts connections in order to send workers
       work */
    std::string serve_request(std::string req);
    std::string generate_work();
    bool process_work(std::string work);
};

#endif
