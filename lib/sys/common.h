#ifndef __LIB_SYS_COMMON_INCLUDED_H__
#define __LIB_SYS_COMMON_INCLUDED_H__

#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#ifdef __WIN32
#include <winsock2.h>
#ifdef _MSC_VER
#pragma comment (lib, "Ws2_32.lib")
#endif // _MSC_VER
#else
#include <sys/socket.h>
#endif // __WIN32

typedef enum socket_type {
	TCP_SOCKET = SOCK_STREAM,
	UDP_SOCKET = SOCK_DGRAM,
	RAW_SOCKET = SOCK_RAW
} socket_type_t;

typedef struct socket {
	char name[32];
	int descriptor;
	int domain;
	socket_type_t type;
	int protocol;
	struct sockaddr_in sa;
} socket_t;

typedef struct poll_config {
	int nfds;
	int timeout;
	void (*handler)(int socket, struct sockaddr *addr);
	int events;
	struct sockaddr_in sa;
} poll_config_t;

typedef struct http_server_config {
    int backlog;
    poll_config_t *poll;
} http_server_config_t;

typedef struct http_server {
    socket_t *sock;
    http_server_config_t *config;
} http_server_t;

typedef enum {
    // 1xx INFORMATIONAL RESPONSE
    CONTINUE = 100,
    SWITCHING_PROTOCOLS,
    PROCESSING,
    EARLY_HINTS,
    // 2xx SUCCESS
    OK = 200,
    CREATED,
    ACCEPTED,
    NON_AUTHORITATIVE_INFORMATION,
    NO_CONTENT,
    RESET_CONTENT,
    PARTIAL_CONTENT,
    MULTI_STATUS,
    ALREADY_REPORTED,
    IM_USED = 226,
    // 3xx REDIRECTION
    MULTIPLE_CHOICES = 300,
    MOVED_PERMANENTLY,
    FOUND,
    SEE_OTHER,
    NOT_MODIFIED,
    USE_PROXY,
    SWITCH_PROXY,
    TEMPORARY_REDIRECT,
    PERMANENT_REDIRECT,
    // 4xx CLIENT ERRORS
    BAD_REQUEST = 400,
    UNAUTHORIZED,
    PAYMENT_REQUIRED,
    FORBIDDEN,
    NOT_FOUND,
    METHOD_NOT_ALLOWED,
    NOT_ACCEPTABLE,
    PROXY_AUTHENTICATION_REQUIRED,
    REQUEST_TIMEOUT,
    CONFLICT,
    GONE,
    LENGTH_REQUIRED,
    PRECONDITION_FAILED,
    PAYLOAD_TOO_LARGE,
    URI_TOO_LONG,
    UNSUPPORTED_MEDIA_TYPE,
    REQUESTED_RANGE_NOT_SATISFIABLE,
    EXPECTATION_FAILED,
    IM_A_TEAPOT,
    MISDIRECTED_REQUEST = 421,
    UNPROCESSABLE_ENTITY,
    LOCKED,
    FAILED_DEPENDENCY,
    TOO_EARLY,
    UPGRADE_REQUIRED,
    PRECONDITION_REQUIRED = 428,
    TOO_MANY_REQUEST,
    REQUEST_HEADER_FIELDS_TOO_LARGE = 431,
    UNAVAILABLE_FOR_LEGAL_REASONS = 451,
    // 5xx SERVER ERRORS
    INTERNAL_SERVER_ERROR = 500,
    NOT_IMPLEMENTED,
    BAD_GATEWAY,
    SERVICE_UNAVAILABLE,
    GATEWAY_TIMEOUT,
    HTTP_VERSION_NOT_SUPPORTED,
    VARIANT_ALSO_NEGOTIATES,
    INSUFFICIENT_STORAGE,
    LOOP_DETECTED,
    NOT_EXTENDED = 510,
    NETWORK_AUTHENTICATION_REQUIRED
} HTTP_STATUS_CODES;

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif // __LIB_SYS_COMMON_INCLUDED_H__
