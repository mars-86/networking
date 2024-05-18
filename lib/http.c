#include "networking.h"
#include "server.h"
#include <stdlib.h>

struct http_server {
    server_t* info;
    struct sockaddr_in si;
    server_type_t type;
    int protocol;
    int port;
};

static void __free_http_server(http_server_t* server)
{
    if (server) {
        server_destroy(server->info);
        free(server);
    }
}

http_server_t* server_http_init(const server_config_t* config)
{
    http_server_t* server = NULL;
    if (!config)
        goto err;

    server = (http_server_t*)malloc(sizeof(http_server_t));
    if (!server)
        goto err;

    server->type = SERVER_TYPE_REMOTE;
    server->info = server_init(server->type, config);

    if (!server->info)
        goto err;

    return server;

err:
    __free_http_server(server);

    return NULL;
}

int server_http_listen(const http_server_t* server, unsigned short port, void (*on_listen)(void* usr))
{
    return server_listen(server->type, server->info, TCP_SOCKET, port, NULL, (struct sockaddr_in*)&server->si, on_listen);
}

void server_http_destroy(http_server_t* server)
{
    connection_close_remote(server->info->sock);
    __free_http_server(server);
}
