#include "networking.h"
#include "server.h"
#include <stdlib.h>

struct https_server {
    server_t* info;
    struct sockaddr_in si;
    server_type_t type;
    const char* certs;
    int protocol;
    int port;
};

static void __free_https_server(https_server_t* server)
{
    if (server) {
        server_destroy(server->info);
        free(server);
    }
}

https_server_t* server_https_init(const server_config_t* config)
{
    https_server_t* server = NULL;
    if (!config)
        goto err;

    server = (https_server_t*)malloc(sizeof(https_server_t));
    if (!server)
        goto err;

    server->type = SERVER_TYPE_REMOTE;
    server->info = server_init(server->type, config);

    if (!server->info)
        goto err;

    return server;

err:
    __free_https_server(server);

    return NULL;
}

int server_https_listen(const https_server_t* server, unsigned short port, void (*on_listen)(void* usr))
{
    return server_listen(server->type, server->info, TCP_SOCKET, port, NULL, (struct sockaddr_in*)&server->si, on_listen);
}

void server_https_destroy(https_server_t* server)
{
    connection_close_remote(server->info->sock);
    __free_https_server(server);
}
