#ifndef __LIB_SYS_LINUX_PROTO_INCLUDED_H__
#define __LIB_SYS_LINUX_PROTO_INCLUDED_H__

#include "../common.h"
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

int socket_create(socket_type_t type, socket_t* sock);
int socket_listen(int socket, unsigned short port, const char* path, int backlog, socket_t* sock);
int socket_inet_listen(int socket, unsigned short port, int backlog, struct sockaddr_in* si);
int socket_unix_listen(int socket, const char* path, int backlog, struct sockaddr_un* su);
int socket_accept(int socket, struct sockaddr* addr);
int socket_connect(int socket, struct sockaddr* addr);
int socket_recv(int socket, void* __restrict__ buff, size_t len, int flags, struct sockaddr* __restrict__ addr, socklen_t* __restrict__ addr_len);
int socket_send(int socket, const void* buff, size_t len, int flags, const struct sockaddr* sa, socklen_t addr_len);
int socket_poll(int socket, int nfds, int fd0ev, int events, int timeout, fd0_ev_handler_t fd0_ev_handler, inc_ev_handler_t ev_handler, int_handler_t int_handler);
int socket_close(int socket, const char* path);

int connection_open(socket_t* socket, int type, unsigned short port, const char* path, int backlog);
int connection_open_local(socket_t* sock, const char* path, int backlog, struct sockaddr_un* su);
int connection_open_remote(socket_t* sock, int type, unsigned short port, int backlog, struct sockaddr_in* si);
int connection_accept(socket_t* socket, struct sockaddr* addr);
int connection_recv(socket_t* socket, void* buff, size_t len);
int connection_send(socket_t* socket, const void* buff, size_t len);
int connection_polling(socket_t* socket, const poll_config_t* config);
int connection_close_local(const char* path);
int connection_close_remote(socket_t* socket);

local_server_t* server_local_init(const server_config_t* config);
int server_local_listen(const local_server_t* server, const char* path, void (*on_listen)(void* usr));
void server_local_destroy(local_server_t* server);

http_server_t* server_http_init(const server_config_t* config);
int server_http_listen(const http_server_t* server, unsigned short port, void (*on_listen)(void* usr));
void server_http_destroy(http_server_t* server);

https_server_t* server_https_init(const server_config_t* config);
int server_https_listen(const https_server_t* server, unsigned short port, void (*on_listen)(void* usr));
void server_https_destroy(https_server_t* server);

char* header_value2(char* dest, const char* headers, const char* key);

const char* http_response(char* dest, HTTP_STATUS_CODES status, const char** headers, const char* body);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif // __LIB_SYS_LINUX_PROTO_INCLUDED_H__
