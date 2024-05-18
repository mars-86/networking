#include "server.h"
#include "networking.h"
#include "sys/common.h"
#include "sys/linux/proto.h"
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/un.h>

static void __free_server(server_t* server)
{
    if (server) {
        if (server->config)
            free(server->config);
        if (server->sock)
            free(server->sock);
        free(server);
    }
}

server_t* server_init(server_type_t type, const server_config_t* config)
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

    if (type == SERVER_TYPE_LOCAL) {
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

int server_listen(server_type_t type, const server_t* server, int protocol, unsigned short port, const char* path, void* saddr)
{
    // TODO fill all socket_t fields
    socket_t new_sock;
    int status;

    if (type == SERVER_TYPE_LOCAL) {
        if ((new_sock.descriptor = connection_open_local(server->sock, path, server->config->backlog, (struct sockaddr_un*)saddr)) < 0)
            return -1;
    } else {
        if ((new_sock.descriptor = connection_open_remote(server->sock, protocol, port, server->config->backlog, (struct sockaddr_in*)saddr)) < 0)
            return -1;
    }

    status = connection_polling(&new_sock, &(server->config->poll));

    connection_close_remote(&new_sock);

    return status;
}

void server_destroy(server_t* server)
{
    __free_server(server);
}
