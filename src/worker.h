#ifndef WORKER_H
#define WORKER_H

#include <iostream>

#include "ray_tracer.h"
#include "pnm_image.h"


class Worker {
public:
    Worker(std::string host, int port);
    bool get_work();
    void trace();
    void send_work();
    void trace_and_send_work();

private:
    std::string collector_host;
    int collector_port;
    /* represent scene or ray tracer, but need updated version */

};

#endif
