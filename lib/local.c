#include "networking.h"
#include <stdlib.h>
#include <string.h>

struct http_server {
    socket_t* sock;
    http_server_config_t* config;
};

http_server_t* local_server_init(const http_server_config_t* config)
{
    http_server_t* server = NULL;
    if (!config)
        goto err;

    server = (http_server_t*)malloc(sizeof(http_server_t));
    if (!server)
        goto err;

    server->sock = (socket_t*)malloc(sizeof(socket_t));
    if (!(server->sock))
        goto err;

    server->config = (http_server_config_t*)malloc(sizeof(http_server_config_t));
    if (!(server->config))
        goto err;

    memcpy(server->config, config, sizeof(http_server_config_t));

    return server;

err:
    if (server && server->sock)
        free(server->sock);
    if (server)
        free(server);

    return NULL;
}

int local_server_listen(const http_server_t* server, const char* path)
{
    // TODO fill all socket_t fields
    socket_t new_sock;
    int status;

    if ((new_sock.descriptor = connection_open(server->sock, UNIX_SOCKET, 0, path, server->config->backlog)) == -1)
        return -1;

    status = connection_polling(&new_sock, &(server->config->poll));

    return status;
}

void local_server_destroy(http_server_t* server)
{
    connection_close(server->sock);
    free(server->sock);
    free(server->config);
    free(server);
}
