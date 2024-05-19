#include "networking.h"
#include <sys/un.h>

int connection_open_local(socket_t* sock, const char* path, int backlog, struct sockaddr_un* su)
{
    int sd;
    if ((sd = socket_create(UNIX_SOCKET, sock)) < 0)
        return -1;

    if (!path)
        return -1;

    if (socket_listen(sd, backlog, 0, NULL, path, su) < 0)
        return -1;

    return sd;
}

int connection_open_remote(socket_t* sock, int type, unsigned short port, int backlog, struct sockaddr_in* si)
{
    int sd;
    if ((sd = socket_create(type, sock)) < 0)
        return -1;
    /*
        if (socket_inet_listen(sd, port, backlog, si) < 0)
            return -1;
    */
    if (socket_listen(sd, backlog, port, si, NULL, NULL) < 0)
        return -1;

    return sd;
}

int connection_accept(socket_t* socket, struct sockaddr* addr)
{
    int sd;
    if ((sd = socket_accept(socket->descriptor, addr)) < 0)
        return -1;
    return sd;
}

int connection_to(int socket, struct sockaddr* addr, struct sockaddr_in* raddr)
{
    int sd;
    if ((sd = socket_connect(socket, (struct sockaddr*)&raddr)) < 0)
        return 1;
    return sd;
}

int connection_recv(socket_t* socket, void* buff, size_t len)
{
    int bytes = 0, total = 0;
    // do {
    bytes = socket_recv(socket->descriptor, (unsigned char*)buff, len, 0, NULL, NULL);
    if (bytes) {
        printf("Received: %d, total: %d\n", bytes, total += bytes);
    } else if (bytes == 0) {
        printf("Connection closed\n");
        // break;
    } else {
        printf("ERROR");
        // break;
    }
    // } while (bytes > 0);

    return bytes;
}

int connection_send(socket_t* socket, const void* buff, size_t len)
{
    int bytes = 0, total = 0;
    // do {
    bytes = socket_send(socket->descriptor, (unsigned char*)buff, len, 0, NULL, 0);
    if (bytes) {
        printf("Sent: %d, total: %d\n", bytes, total += bytes);
    } else if (bytes == 0) {
        printf("Connection closed\n");
        // break;
    } else {
        printf("ERROR");
        // break;
    }
    // TODO handle large payloads
    // } while (bytes > 0);

    return bytes;
}

int connection_polling(socket_t* socket, const poll_config_t* config)
{
    // TODO check config params
    if (socket && config)
        return socket_poll(
            socket->descriptor,
            config->nfds,
            config->fd0_events,
            config->events,
            config->timeout,
            config->fd0_ev_handler,
            config->ev_handler,
            config->int_handler);
    return -1;
}

int connection_close_local(const char* path)
{
    if (path)
        if (socket_close(0, path) < 0)
            return -1;
    return 0;
}

int connection_close_remote(socket_t* socket)
{
    if (socket)
        if (socket_close(socket->descriptor, NULL) < 0)
            return -1;
    return 0;
}
