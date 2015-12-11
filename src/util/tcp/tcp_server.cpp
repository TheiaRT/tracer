#include <vector>

#include "tcp_server.h"

std::string tcp_server_default_cat(const std::string req)
{
    return req;
}

TCPServer::TCPServer(message_handler handler)
    : handler(handler), running(new std::atomic_bool(false))
{
}

TCPServer::~TCPServer()
{
    /* Loop thread should be stopped before delete. */
    if (loop_thread->joinable()) {
        /* TODO: decide later if we actually need to join */
        loop_thread->detach();
    }

    delete running;
    delete loop_thread;
}

bool TCPServer::start(std::string host, int port, int backlog)
{
    /* Open a new socket for the server. */
    if ((parentsock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Could not open socket.");
        return false;
    }

    /* Can re-run immediately after kill, instead of waiting. */
    optval = 1;
    setsockopt(parentsock, SOL_SOCKET, SO_REUSEADDR,
               (const void *)&optval, sizeof(optval));

    /* Zero out the server_addr. */
    bzero((char *) &server_addr, sizeof(server_addr));

    server_addr.sin_family = AF_INET;
    /* Let the system figure out our IP address. */
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons((unsigned short) port);

    /* Bind to the port. */
    if (bind(parentsock,
             (struct sockaddr *) &server_addr,
             sizeof(server_addr)) < 0) {
        perror("Could not bind.");
        return false;
    }

    /* Listen on the port with a backlog of up to backlog connections. */
    if (listen(parentsock, backlog) < 0) {
        perror("Could not listen.");
        return false;
    }

    /* Make the loop thread runnable. */
    *running = true;

    /* Start the loop! Go! */
    loop_thread = new std::thread([=] { serve_loop(); });
    return true;
}

void TCPServer::stop()
{
    /* Since the loop needs to be able to lock the running semaphore, lock
       it. */
    *running = false;
}

bool TCPServer::serve_loop()
{
    client_len = sizeof(client_addr);
    std::vector<std::thread *> threads;

    /* While we haven't called ::stop()... */
    while (*running == true) {
        int child_sock = accept(parentsock,
                                (struct sockaddr *) &client_addr,
                                (socklen_t *) &client_len);
        if (child_sock < 0) {
            perror("Could not accept.");
            /* TODO: Probably should not shut down the whole server. */
            return false;
        }

        std::thread *tp = new std::thread(
            [=](int sock) {
                dispatch_handler(sock);
            },
            child_sock
        );
        threads.push_back(tp);
    }

    for (std::thread *tp : threads) {
        delete tp;
    }

    return true;
}

static bool read_from_sock(int client, std::string &res)
{
    fd_set read_set;
    /* Timeout after 0.01 seconds. */
    struct timeval timeout;
    timeout.tv_sec = 1;
    timeout.tv_usec = 0;

    FD_ZERO(&read_set);
    FD_SET(client, &read_set);

    int size = 1024;
    char *buffer = new char[size];
    bzero(buffer, size);
    int recvd = 0;

    int r;
    do {
        /* See what kind of state the descriptor is in. */
        r = select(client + 1, &read_set, NULL, NULL, &timeout);
        if (r < 0) {
            perror("Select failed.");
            return false;
        }
        else if (r == 0) {
            /* Read timed out, most likely after an initial read. So it's fine
               to just return false. */
            break;
        }
        else {
            if ((recvd = recv(client, buffer, size - 1, 0)) > 0) {
                res += buffer;
                bzero(buffer, size);
            }
            else {
                /* We are not expecting any 0-sized messages. */
                perror("Receive failed.");
                return false;
            }
        }
    } while (r > 0);

    delete [] buffer;
    return true;
}

bool TCPServer::dispatch_handler(int client)
{
    std::string req = "";
    if (read_from_sock(client, req) == false) {
        /* No failure/recovery plan yet. */
        perror("read_from_sock failed.");
        return false;
    }

    /* Handle the client's request. */
    std::string response = handler(req);

    /* Write some response back. */
    int nwritten = send(client, response.c_str(), response.length(), 0);
    if (nwritten < 0 || nwritten != response.length()) {
        perror("Could not write to socket.");
        return false;
    }

    close(client);
    return true;
}
