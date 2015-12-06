#include <unistd.h>

#include "tcp_client.h"

TCPClient::TCPClient() : sock(-1)
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

#if defined(__unix__) || (defined(__APPLE__) && defined(__MACH__))
        int set = 1;
        setsockopt(sock, SOL_SOCKET, SO_NOSIGPIPE, (void *)&set, sizeof(set));
#endif
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

bool TCPClient::send_data(std::string data, int size)
{
#if 0
    int length = data.length(), total_sent = 0;
    const char *datas = data.c_str();
    while (total_sent < length) {
        int to_send = length - total_sent;
        int this_length = to_send < size ? to_send : size;
        std::cerr << "sending " << this_length << " bytes...";
        int nwritten;
#if defined(__linux__)
        int flags = MSG_NOSIGNAL;
#else
        int flags = 0;
#endif
        if ((nwritten = send(sock, datas + total_sent, this_length, 0)) < 0) {
            perror("Send failed.");
            return false;
        }
        std::cerr << " done" << std::endl;
        if (nwritten != this_length) {
            perror("Could not send message.");
            return false;
        }

        total_sent += this_length;
    }

    return true;
#endif

    int ntries = 10;
    int sent;
    std::cerr << "sending...";
    while (ntries-- > 0 && (sent = send(sock, data.c_str(), data.length(), 0)) < 0) {
        if (sent < 0 && ntries == 0) {
            perror("Send failed.");
            return false;
        }
    }

    std::cerr << " done" << std::endl;
    return true;
}

std::string TCPClient::receive(int size)
{
    std::string buffer = "";
    char cbuffer[size];
    bzero(cbuffer, size);
    int recvd = 0;
    while((recvd = recv(sock, cbuffer, size - 1, 0)) > 0) {
        buffer += cbuffer;
        bzero(cbuffer, size);
    }

    if (recvd < 0) {
        perror("Receive failed.");
        return std::string();
    }

    close(sock);
    return buffer;
}
