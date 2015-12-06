#ifndef TCP_SERVER_H
#define TCP_SERVER_H

#include <iostream> /* cout, string */
#include <thread> /* thread */
#include <atomic> /* atomic_bool */
#include <functional> /* function */
#include <string.h> /* strlen */
#include <sys/socket.h> /* socket, accept */
#include <arpa/inet.h> /* inet_addr */
#include <unistd.h> /* close */


typedef std::function<std::string(std::string)> message_handler;
std::string tcp_server_default_cat(const std::string req);

class TCPServer {
public:
    TCPServer(message_handler handler=&tcp_server_default_cat);
    ~TCPServer();
    bool start(std::string host, int port, int backlog=5);
    void stop();

private:
    int optval;
    int parentsock;
    struct sockaddr_in server_addr, client_addr;
    int client_len;
    message_handler handler;
    std::atomic_bool *running;
    std::thread *loop_thread;

    bool serve_loop();
    bool dispatch_handler(int client);
};

#endif
