#ifndef COLLECTOR_H
#define COLLECTOR_H

#include <iostream>
#include <mutex>

#include "dist/json/json.h"
#include "util/pnm/pnm_image.h"
#include "util/vector/vector.h"
#include "util/tcp/tcp_server.h"
#include "work_queue.h"


class Collector {
public:
    Collector(std::string filename, size_t width, size_t height, size_t splits);
    ~Collector();
    bool start(std::string host, int port);
    void stop();
    void write_image(std::string filename);
    bool finish();

private:
    Json::Value scene;
    PnmImage pixmap;
    WorkQueue queue;
    TCPServer *server;
    std::mutex finished, queue_lock;
    size_t remaining_work;

    /* In a thread, listens and accepts connections in order to send workers
       work. */
    std::string serve_request(std::string req);
    std::string generate_work(bool has_work);
    std::string generate_error(std::string type);
    void process_work(Json::Value work, Json::Value pixels);
};

#endif
