#ifndef __LIB_SERVER_INCLUDED_H__
#define __LIB_SERVER_INCLUDED_H__

#include "sys/common.h"

struct server {
    socket_t* sock;
    server_config_t* config;
};

typedef struct server server_t;

server_t* server_init(server_type_t type, const server_config_t* config);
int server_listen(server_type_t type, const server_t* server, int protocol, unsigned short port, const char* path, void* saddr);
void server_destroy(server_t* server);

#endif // __LIB_SERVER_INCLUDED_H__
