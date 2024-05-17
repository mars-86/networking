#include "networking.h"
#include "server.h"
#include <stdlib.h>
#include <string.h>

// TODO change server type
struct http_server {
    server_t* info;
    int protocol;
    int type;
};

static void __free_local_server(http_server_t* server)
{
    if (server) {
        if (server->info)
            server_destroy(server->info);
        free(server);
    }
}

http_server_t* local_server_init(const server_config_t* config)
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
    __free_local_server(server);

    return NULL;
}

int local_server_listen(const http_server_t* server, const char* path)
{
    return server_listen(server->info, UNIX_SOCKET, 0, path);
}

void local_server_destroy(http_server_t* server)
{
    connection_close(server->info->sock);
    __free_local_server(server);
}
