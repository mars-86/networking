#include "networking.h"
#include "sys/common.h"
#include <stdlib.h>
#include <string.h>

struct http_server {
    socket_t* sock;
    http_server_config_t* config;
};

http_server_config_t* sconfig = NULL;

http_server_t* http_server_init(const http_server_config_t* config)
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

int http_server_listen(const http_server_t* server, unsigned short port)
{
    int sd = connection_open(server->sock, TCP_SOCKET, port, server->config->backlog);
    int status = connection_polling(sd, &(server->config->poll));

    return status;
}

void http_server_destroy(http_server_t* server)
{
    connection_close(server->sock->descriptor);
    free(server->sock);
    free(server->config);
    free(server);
}
