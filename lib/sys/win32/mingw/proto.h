#ifndef __LIB_SYS_WIN32_MINGW_PROTO_INCLUDED_H__
#define __LIB_SYS_WIN32_MINGW_PROTO_INCLUDED_H__

#include <stdio.h>
#include "../../common.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

int socket_create(socket_type_t type, socket_t *sock);
int socket_listen(int socket, unsigned short port, int backlog, socket_t *sock);
int socket_accept(int socket, struct sockaddr *addr);
int socket_connect(int socket, struct sockaddr *addr);
int socket_recv(int socket, char *buff, size_t len, int flags);
void socket_close(int socket);
int socket_poll(int listen_sock, const poll_config_t *poll);

int connection_open(socket_t *sock, unsigned short port, int backlog);
int connection_accept(int socket, struct sockaddr *addr);
int connection_to(int socket, struct sockaddr *addr, struct sockaddr_in *raddr);
int connection_polling(int socket, const poll_config_t *config);
void connection_close(int socket);

http_server_t *server_init(const http_server_config_t *config);
int server_listen(const http_server_t *server, unsigned short port);
void server_destroy(const http_server_t *server);

const char *generate_headers(char *dest, HTTP_STATUS_CODES status, const char *body);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif // __LIB_SYS_WIN32_MINGW_PROTO_INCLUDED_H__
