#include <stdlib.h>
#include <string.h>
#include "networking.h"

http_server_config_t *sconfig = NULL;

http_server_t *http_server_init(const http_server_config_t *config)
{
    if (!config)
        return NULL;

    http_server_t *server = (http_server_t *)malloc(sizeof(http_server_t));
    if (!server)
        return NULL;

    server->sock = (socket_t *)malloc(sizeof(socket_t));
    if (!(server->sock)) {
        free(server);
        return NULL;
    }

    server->config = (http_server_config_t *)malloc(sizeof(http_server_config_t));
    if (!(server->config)) {
        free(server->sock);
        free(server);
        return NULL;
    }

    memcpy(server->config, config, sizeof(http_server_config_t));

    return server;
}

int http_server_listen(const http_server_t *server, unsigned short port)
{
    int sd = connection_open(server->sock, port, server->config->backlog);
    int status = connection_polling(sd, &(server->config->poll));

    return status;
}

void http_server_destroy(http_server_t *server)
{
    connection_close(server->sock->descriptor);
    free(server->sock);
    free(server->config);
    free(server);
}
