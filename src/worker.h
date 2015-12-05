#ifndef WORKER_H
#define WORKER_H

#include <iostream>

#include "pnm_image.h"
#include "tcp_client.h"
#include "ray_tracer.h"


class Worker {
public:
    Worker(std::string host, int port);
    ~Worker();
    bool get_work();
    bool trace_and_send_work();

private:
    std::string collector_host;
    int collector_port;
    TCPClient client;
    bool have_work;
    work_t work;
    RayTracer *tracer;

    std::string trace();
    bool parse_work_message(std::string json_message,
                            work_t &work,
                            RayTracer *&tracer);
    bool send_and_receive(std::string req, std::string &resp);
    std::string generate_work_request();
};

#endif
