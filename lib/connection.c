#include "networking.h"

int connection_open(socket_t *sock, unsigned short port, int backlog)
{
    int sd;
	if ((sd = socket_create(TCP_SOCKET, sock)) < 0)
        return -1;

	if (socket_listen(sd, port, backlog, sock) < 0)
        return -1;

	return sd;
}

int connection_accept(int socket, struct sockaddr *addr)
{
    int sd;
    if ((sd = socket_accept(socket, addr)) < 0)
        return -1;
    return sd;
}

int connection_to(int socket, struct sockaddr *addr, struct sockaddr_in *raddr)
{
    int sd;
    if ((sd = socket_connect(socket, (struct sockaddr *)&raddr)) < 0)
        return 1;
    return sd;
}

int connection_recv(int socket, char *buff)
{
    char buff_temp[64];
    int bytes, total = 0;
    do {
        bytes = socket_recv(socket, buff_temp, 64, 0);
        if (bytes) {
            printf("Received: %d, total: %d\n", bytes, total += bytes);
        } else if (bytes == 0) {
            printf("Connection closed\n");
            break;
        } else {
            printf("ERROR");
            break;
        }
    } while (bytes > 0);

    return bytes;
}

int connection_polling(int socket, const poll_config_t *config)
{
    socket_poll(socket, config);
}

void connection_close(int socket)
{
    socket_close(socket);
}
