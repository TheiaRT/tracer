#include <unistd.h>
#include <assert.h>

#include "tcp_server.h"
#include "tcp_client.h"

int main()
{
    TCPServer s = TCPServer();
    TCPClient c = TCPClient();

    std::string to_send = "GET /~mberns01/\n";
    std::string host = "0.0.0.0";
    int port = 4555;

    fprintf(stderr, "Starting server.\n");
    s.start(host, port);

    assert(true == c.connect(host, port));
    assert(true == c.send_data(to_send));
    assert(to_send == c.receive());

    sleep(1);
    s.stop();

    fprintf(stderr, "Stopped server.\n");
    return 0;
}
