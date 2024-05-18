#ifndef __LIB_SYS_COMMON_INCLUDED_H__
#define __LIB_SYS_COMMON_INCLUDED_H__

#include <signal.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#ifdef __WIN32
#include <winsock2.h>
#ifdef _MSC_VER
#pragma comment(lib, "Ws2_32.lib")
#endif // _MSC_VER
#else
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#endif // __WIN32

#define SOCKET_NAME_MAX 32
#define SET_HEADER(key, value) key ": " value "\r\n"

typedef struct local_server local_server_t;
typedef struct http_server http_server_t;
typedef struct https_server https_server_t;

typedef enum socket_type {
    TCP_SOCKET,
    UDP_SOCKET,
    UNIX_SOCKET,
    RAW_SOCKET
} socket_type_t;

struct socket {
    char name[SOCKET_NAME_MAX];
    int descriptor;
    int domain;
    socket_type_t type;
    int protocol;
    union {
        struct sockaddr_in* sa;
        struct sockaddr_un* su;
    };
};

typedef struct socket socket_t;

typedef enum server_type {
    SERVER_TYPE_LOCAL = 1,
    SERVER_TYPE_REMOTE
} server_type_t;

typedef int (*fd0_ev_handler_t)(int socket, int ev, struct sockaddr* addr);
typedef int (*inc_ev_handler_t)(int socket, int ev, struct sockaddr* addr);
typedef int (*int_handler_t)(void);

struct poll_config {
    // number of file descriptors
    int nfds;
    // events to listen in fd 0
    int fd0_events;
    // events to listen when fd 0 is already listening
    int events;
    // timeout in ms to check for events
    int timeout;
    // handler for events in fd 0
    fd0_ev_handler_t fd0_ev_handler;
    // handler for events
    inc_ev_handler_t ev_handler;
    // hadler for interrupts
    int_handler_t int_handler;
};

typedef struct poll_config poll_config_t;

struct server_config {
    poll_config_t poll;
    int backlog;
};

typedef struct server_config server_config_t;

struct https_server_config {
    server_config_t common;
};

typedef struct https_server_config https_server_config_t;

typedef const char** header_raw_t;

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
    TOO_MANY_REQUESTS,
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
