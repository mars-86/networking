#include "networking.h"
#include "sys/common.h"

int connection_open(socket_t* sock, int type, unsigned short port, const char* path, int backlog)
{
    int sd;
    if ((sd = socket_create(type, sock)) < 0)
        return -1;

    if (socket_listen(sd, port, path, backlog, sock) < 0)
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
    bytes = socket_recv(socket->descriptor, (unsigned char*)buff, len, 0);
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
    bytes = socket_send(socket->descriptor, (unsigned char*)buff, len, 0);
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
    return socket_poll(socket->descriptor, config);
}

void connection_close(socket_t* socket)
{
    if (socket)
        socket_close(socket->descriptor, NULL);
}
