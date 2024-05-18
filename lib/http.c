#include "networking.h"
#include "server.h"
#include "sys/common.h"
#include <stdlib.h>
#include <string.h>

struct http_server {
    server_t* info;
    int protocol;
    int type;
};

static void __free_http_server(http_server_t* server)
{
    if (server) {
        server_destroy(server->info);
        free(server);
    }
}

http_server_t* http_server_init(const server_config_t* config)
{
    http_server_t* server = NULL;
    if (!config)
        goto err;

    server = (http_server_t*)malloc(sizeof(http_server_t));
    if (!server)
        goto err;

    server->info = server_init(config);

    if (!server->info)
        goto err;

    return server;

err:
    __free_http_server(server);

    return NULL;
}

int http_server_listen(const http_server_t* server, unsigned short port)
{
    return server_listen(server->info, TCP_SOCKET, port, NULL);
}

void http_server_destroy(http_server_t* server)
{
    __free_http_server(server);
}
