#include "server.h"
#include "networking.h"
#include "sys/common.h"
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>

static void __free_server(server_t* server)
{
    if (server) {
        if (server->config)
            free(server->config);
        if (server->sock) {
            connection_close(server->sock);
            if (server->sock->su)
                free(server->sock->su);
            if (server->sock->sa)
                free(server->sock->sa);
            free(server->sock);
        }
        free(server);
    }
}

server_t* server_init(const server_config_t* config)
{
    server_t* server = NULL;
    if (!config)
        goto err;

    server = (server_t*)malloc(sizeof(server_t));
    if (!server)
        goto err;

    server->sock = (socket_t*)malloc(sizeof(socket_t));
    if (!(server->sock))
        goto err;

    if (config->type == SERVER_TYPE_LOCAL) {
        server->sock->su = (struct sockaddr_un*)malloc(sizeof(struct sockaddr_un));
        if (!server->sock->su)
            goto err;
    } else {
        server->sock->sa = (struct sockaddr_in*)malloc(sizeof(struct sockaddr_in));
        if (!server->sock->sa)
            goto err;
    }

    server->config = (server_config_t*)malloc(sizeof(server_config_t));
    if (!(server->config))
        goto err;

    memcpy(server->config, config, sizeof(server_config_t));

    return server;

err:
    __free_server(server);

    return NULL;
}

int server_listen(const server_t* server, int protocol, unsigned short port, const char* path)
{
    // TODO fill all socket_t fields
    socket_t new_sock;
    int status;

    if ((new_sock.descriptor = connection_open(server->sock, protocol, port, path, server->config->backlog)) < 0)
        return -1;

    status = connection_polling(&new_sock, &(server->config->poll));

    connection_close(&new_sock);

    return status;
}

void server_destroy(server_t* server)
{
    __free_server(server);
}
