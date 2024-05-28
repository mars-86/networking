#include "../networking.h"
#include "server.h"
#include <stdlib.h>

struct local_server {
    server_t* info;
    struct sockaddr_un su;
    server_type_t type;
    int protocol;
};

static void __free_local_server(local_server_t* server)
{
    if (server) {
        server_destroy(server->info);
        free(server);
    }
}

local_server_t* server_local_init(const server_config_t* config)
{
    local_server_t* server = NULL;
    if (!config)
        goto err;

    server = (local_server_t*)malloc(sizeof(local_server_t));
    if (!server)
        goto err;

    server->type = SERVER_TYPE_LOCAL;
    server->info = server_init(server->type, config);

    if (!server->info)
        goto err;

    return server;

err:
    __free_local_server(server);

    return NULL;
}

int server_local_listen(const local_server_t* server, const char* path, void (*on_listen)(void* usr))
{
    return server_listen(server->type, server->info, UNIX_SOCKET, 0, path, (struct sockaddr_un*)&server->su, on_listen);
}

void server_local_destroy(local_server_t* server)
{
    connection_close_local(server->su.sun_path);
    __free_local_server(server);
}
