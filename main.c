#include <stdio.h>
// #include <stdlib.h>
#include "networking.h"
#include <poll.h>
#include <string.h>
#include <unistd.h>

#define SERVER_PATH "/home/mars/server.sock"

char buffer[2048];

int event_listener(int s, int e, struct sockaddr* addr)
{
    int sd;
    socket_t sock;

    sock.descriptor = s;
    if ((sd = connection_accept(&sock, addr)) < 0)
        return -1;

    printf("Connection accepted\n");

    return sd;
}

int pong(int s, int e, struct sockaddr* addr)
{
    socket_t sock = { .descriptor = s };
    char request[2048] = { 0 }, response[2048] = { 0 }, buffer[1024] = { 0 };
    int rbytes = connection_recv(&sock, request, 2048);
    const char* headers[] = {
        SET_HEADER("Content-Type", "text/plain"),
        NULL
    };

    // header_value2(buffer, request, "Content-Type");

    printf("%s\n", buffer);

    printf("on sock %d RECV %d\n", s, rbytes);
    printf("%s\n", request);
    if (rbytes > 0) {
        http_response(response, OK, headers, "world!");
        int sbytes = connection_send(&sock, response, strlen(response));

        printf("on sock %d SENT %d\n", s, sbytes);
        printf("%s\n", response);
        if (sbytes <= 0)
            printf("Error on send\n");
    }
    // connection_recv(s, buffer);
    // printf("ev %d recv in socket: %d\n", e, s);
    return rbytes;
}

int main(void)
{
    /*
    int sock = socket_create(TCP_SOCKET, NULL);

    struct sockaddr_in sa;
    sa.sin_family = AF_INET6;
    sa.sin_addr.s_addr = inet_addr("2a01:7e00:e000:2fc::4");
    int nsock = connection_to(sock, NULL, &sa);

    printf("%d\n", nsock);
    socket_close(sock);
    socket_close(nsock);
    */

    server_config_t sconf;
    sconf.backlog = 32;
    sconf.poll.nfds = 32;
    sconf.poll.levents = POLLIN;
    sconf.poll.lev_handler = event_listener;
    sconf.poll.events = POLLIN;
    sconf.poll.ev_handler = pong;
    sconf.poll.timeout = 1000;

    http_server_t* server = http_server_init(&sconf);
    printf("server created\n");

    int status = local_server_listen(server, SERVER_PATH);
    // int status = http_server_listen(server, 5037);

    if (status == -1)
        perror("listen");

    http_server_destroy(server);

    printf("server destroyed\n");

    if (unlink(SERVER_PATH) < 0) {
        perror("remove");
        return -1;
    }

    return 0;
}
