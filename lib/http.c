#include "networking.h"

http_server_config_t *sconfig = NULL;

http_server_t *http_server_init(const http_server_config_t *config)
{
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

    server->config->poll = (poll_config_t *)malloc(sizeof(poll_config_t));
    if (!(server->config->poll)) {
        free(server->sock);
        free(server->config);
        free(server);
        return NULL;
    }

    if (!config) {
        // default config
        server->config->backlog = 5;
        server->config->poll->events = POLLIN;
        server->config->poll->nfds = 32;
        server->config->poll->handler = NULL;
    } else {
        server->config = config;
    }

    return server;
}

int http_server_listen(const http_server_t *server, unsigned short port)
{
    int sd = connection_open(server->sock, port, server->config);
    connection_polling(sd, NULL);
}

void http_server_destroy(const http_server_t *server)
{
    connection_close(server->sock->descriptor);
    free(server->sock);
    free(server->config->poll);
    free(server->config);
    free(server);
}
