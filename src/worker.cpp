#include "worker.h"

Worker::Worker(std::string host, int port)
    : collector_host(host), collector_port(port)
{
}

/* returns true if the server had work to send */
bool Worker::get_work()
{
    std::string resp;
    if (send_and_receive("NEED WORK", resp) == false) {
        return false;
    }

    std::cerr << "work: " << resp << std::endl;
    return true;
}

bool Worker::trace_and_send_work()
{
    std::string work = trace();
    std::string resp;
    /* send the json work to the server */
    std::cerr << "sending work..." << std::endl;
    if (send_and_receive(work, resp) == false) {
        return false;
    }

    std::cerr << "resp: " << resp << std::endl;
    return true;
}

/* render the pixels and return json */
std::string Worker::trace()
{
    return "HAVE WORK";
}

bool Worker::send_and_receive(std::string req, std::string &resp)
{
    TCPClient client;
    if (client.connect(collector_host, collector_port) == false) {
        return false;
    }

    if (client.send_data(req) == false) {
        return false;
    }

    resp = client.receive();
    return true;
}
