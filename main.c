#include "lib/networking.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define SERVER_PATH "/home/server.sock"

char buffer[2048];

void on_listen_local(void* usr)
{
    if (!usr)
        return;

    const char* path = (const char*)usr;

    printf("Server listening on %s\n", path);
}

void on_listen_remote(void* usr)
{
    unsigned int* port = (unsigned int*)usr;

    printf("Server listening on port %d\n", *port);
}

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
        http_response(response, OK, headers, "world!\n");
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
    sconf.poll.fd0_events = POLLIN;
    sconf.poll.fd0_ev_handler = event_listener;
    sconf.poll.events = POLLIN;
    sconf.poll.ev_handler = pong;
    sconf.poll.timeout = 1000;

    local_server_t* server = server_local_init(&sconf);
    printf("server created\n");

    int status = server_local_listen(server, SERVER_PATH, on_listen_local);

    if (status == -1)
        perror("listen");

    server_local_destroy(server);

    printf("server destroyed\n");

    /*
        http_server_t* server = server_http_init(&sconf);
        printf("server created\n");

        int status = server_http_listen(server, 5037, on_listen_remote);

        if (status == -1)
            perror("listen");

        server_http_destroy(server);

        printf("server destroyed\n");
    */
    return 0;
}
