#include "networking.h"

http_server_config_t *sconfig = NULL;

http_server_t *server_init(const http_server_config_t *config)
{
    http_server_t *server = (http_server_t *)malloc(sizeof(http_server_t));
    server->sock = (socket_t *)malloc(sizeof(socket_t));
    server->config = (http_server_config_t *)malloc(sizeof(http_server_config_t));
    server->config->poll = (poll_config_t *)malloc(sizeof(poll_config_t));

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

int server_listen(const http_server_t *server, unsigned short port)
{
    int sd = connection_open(server->sock, port, server->config);
    connection_polling(sd, NULL);
}

void server_destroy(const http_server_t *server)
{
    connection_close(server->sock->descriptor);
    free(server->sock);
    free(server->config->poll);
    free(server->config);
    free(server);
}
