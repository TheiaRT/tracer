#ifndef WORKER_H
#define WORKER_H

#include <iostream>

#include "pnm_image.h"
#include "tcp_client.h"
#include "ray_tracer.h"


class Worker {
public:
    Worker(std::string host, int port);
    bool get_work();
    bool trace_and_send_work();

private:
    std::string collector_host;
    int collector_port;
    TCPClient client;

    std::string trace();
    bool send_and_receive(std::string req, std::string &resp);
    /* represent scene or ray tracer, but need updated version */
};

#endif
