#ifndef COLLECTOR_H
#define COLLECTOR_H

#include <iostream>
#include "dist/json/json.h"

#include "pnm_image.h"
#include "vector.h"
#include "work_queue.h"
#include "tcp_server.h"


class Collector {
public:
    Collector(std::string filename, size_t width, size_t height);
    ~Collector();
    bool start(std::string host, int port);
    void stop();
    void write_image(std::string filename);
    bool done();

private:
    Json::Value scene;
    PnmImage pixmap;
    WorkQueue queue;
    TCPServer *server;
    size_t vp_width, vp_height;

    /* In a thread, listens and accepts connections in order to send workers
       work. */
    std::string serve_request(std::string req);
    std::string generate_work();
    std::string generate_error(std::string type);
    void process_work(Json::Value work, Json::Value pixels);
};

#endif
