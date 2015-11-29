#include <unistd.h>

#include "tcp_client.h"

TCPClient::TCPClient() : sock(-1), port(0), address("")
{
}

bool TCPClient::connect(std::string address, int port)
{
    /* New client; sock does not exist */
    if (sock == -1) {
        sock = socket(AF_INET, SOCK_STREAM, 0);
        if (sock < 0) {
            perror("Could not create socket.");
            return false;
        }
    }
    else {
        /* Do nothing; sock exists */
    }

    /* Set up address structure */
    if (inet_addr(address.c_str()) == -1) {
        struct hostent *he;
        struct in_addr **addr_list;

        /* Valgrind may incorrectly report this as leaking. */
        he = gethostbyname(address.c_str());
        if (he == NULL) {
            herror("Could not resolve hostname.");
            return false;
        }

        addr_list = (struct in_addr **) he->h_addr_list;
        for (int i = 0; addr_list[i] != NULL; i++) {
            server.sin_addr = *addr_list[i];
            break;
        }
    }
    else {
        server.sin_addr.s_addr = inet_addr(address.c_str());
    }

    server.sin_family = AF_INET;
    server.sin_port = htons(port);

    if (::connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0) {
        perror("Connect failed.");
        return false;
    }

    return true;
}

bool TCPClient::send_data(std::string data)
{
    if (send(sock, data.c_str(), data.length(), 0) < 0) {
        perror("Send failed.");
        return false;
    }

    return true;
}

std::string TCPClient::receive(int size)
{
    std::string buffer = "";
    char cbuffer[size];
    bzero(cbuffer, size);
    int recvd = 0;
    while((recvd = recv(sock, cbuffer, size, 0)) > 0) {
        buffer += cbuffer;
        bzero(cbuffer, size);
    }

    if (recvd < 0) {
        perror("Receive failed.");
        return NULL;
    }

    close(sock);
    return buffer;
}
