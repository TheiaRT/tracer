#ifndef COLLECTOR_H
#define COLLECTOR_H

#include <iostream>
#include <vector>
#include <thread>
#include <queue>

#include "pnm_image.h"


class Collector {
public:
    Collector(std::string scene);
    bool start(std::string host, int port, int nthreads);
    void stop();

private:
    std::string host;
    int port;
    std::vector<std::thread> threads;
    std::string scene;
    PnmImage pixmap;
    std::queue<vector2_t> work_queue;

    /*
      in a thread, listens and accepts connections in order to send workers work
    */
    void serve_work();
    std::string generate_work();
    bool process_work(std::string work);
};

#endif
