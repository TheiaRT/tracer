#ifndef TCP_CLIENT_H
#define TCP_CLIENT_H

#include <iostream> /* cout, string */
#include <string.h> /* strlen */
#include <sys/socket.h> /* socket */
#include <arpa/inet.h> /* inet_addr */
#include <netdb.h> /* hostent */


class TCPClient {
public:
    TCPClient();
    ~TCPClient();
    bool connect(std::string host, int port);
    bool send_data(std::string data);
    std::string receive(int size=512);

private:
    int sock;
    struct sockaddr_in server;
};

#endif
