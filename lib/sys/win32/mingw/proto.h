#ifndef __LIB_SYS_WIN32_MINGW_PROTO_INCLUDED_H__
#define __LIB_SYS_WIN32_MINGW_PROTO_INCLUDED_H__

#include "../../common.h"
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

int socket_create(socket_type_t type, socket_t* sock);
int socket_listen(int socket, unsigned short port, const char* path, int backlog, socket_t* sock);
int socket_accept(int socket, struct sockaddr* addr);
int socket_connect(int socket, struct sockaddr* addr);
int socket_recv(int socket, void* buff, size_t len, int flags);
int socket_send(int socket, const void* buff, size_t len, int flags);
void socket_close(int socket);
int socket_poll(int listen_sock, const poll_config_t* poll);

int connection_open(socket_t* sock, unsigned short port, int backlog);
int connection_accept(int socket, struct sockaddr* addr);
int connection_to(int socket, struct sockaddr* addr, struct sockaddr_in* raddr);
int connection_polling(int socket, const poll_config_t* config);
void connection_close(int socket);

http_server_t* http_server_init(const http_server_config_t* config);
int http_server_listen(const http_server_t* server, unsigned short port);
void http_server_destroy(http_server_t* server);

const char* http_response(char* dest, HTTP_STATUS_CODES status, const char** headers, const char* body);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif // __LIB_SYS_WIN32_MINGW_PROTO_INCLUDED_H__
