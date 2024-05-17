#include "server.h"
#include "networking.h"
#include <stdlib.h>
#include <string.h>

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
    connection_close(server->sock);
    __free_server(server);
}
