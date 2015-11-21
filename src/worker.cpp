#include "worker.h"

Worker::Worker(std::string host, int port) : host(host), port(port)
{
}

/* returns true if the server had work to send */
bool Worker::get_work()
{
}

/* render the pixels and return json */
std::string Worker::trace()
{
    return "";
}

/* send the json work to the server */
void Worker::send_work(std::string work)
{
}

void Worker::trace_and_send_work()
{
    std::string work = trace();
    send_work(work);
}
