#include "networking.h"
#include "sys/common.h"
#include <stdio.h>
#include <string.h>

#define SUN_MAX_PATH_LEN 108

#ifdef __WIN32
#include <winsock2.h>
#define _handle_err(msg) msg // perror_win(msg)
#define _handle_err_and_clean(msg) _handle_err(msg), WSACleanup()
#define _close_socket(socket) closesocket(socket), WSACleanup()
#else
#include <arpa/inet.h>
#include <netinet/in.h>
#include <poll.h>
#include <signal.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#define _handle_err(msg) perror(msg)
#define _handle_err_and_clean(msg) _handle_err(msg)
#define _close_socket(socket) close(socket)
#endif // __WIN32

// TODO fix this. Not Thread Safe
static int __listening = 1;

void sighand(int s)
{
    __listening = 0;
}

static void __set_sighandler(void)
{
    struct sigaction sa = { 0 };

    sa.sa_handler = sighand;

    sigaction(SIGINT, &sa, NULL);
}

static int __create_unix_socket(struct sockaddr_un* su, const char* path)
{
    struct sockaddr_un __su;

    size_t __path_len = strlen(path);
    if (!su || __path_len > SUN_MAX_PATH_LEN)
        return -1;

    __su.sun_family = AF_UNIX;
    // __path_len + 1 copy NULL byte
    memcpy(__su.sun_path, path, __path_len + 1);

    memcpy(su, &__su, sizeof(struct sockaddr_un));

    return 0;
}

static int __create_tcp_udp_socket(struct sockaddr_in* sa, unsigned short port)
{
    struct sockaddr_in __sa;
    if (!sa)
        return -1;

    __sa.sin_family = AF_INET;
    __sa.sin_port = htons(port);
    __sa.sin_addr.s_addr = inet_addr("127.0.0.1");

    memcpy(sa, &__sa, sizeof(struct sockaddr));

    return 0;
}

int socket_create(socket_type_t type, socket_t* sock)
{
#ifdef __WIN32
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        _handle_err("WSAStartup failed");
        return -1;
    }
#endif // __WIN32

    int sd, sdomain, stype, sproto;
    switch (type) {
    case TCP_SOCKET:
        sdomain = AF_INET;
        stype = SOCK_STREAM;
        sproto = IPPROTO_TCP;
        break;
    case UDP_SOCKET:
        sdomain = AF_INET;
        stype = SOCK_DGRAM;
        sproto = IPPROTO_UDP;
        break;
    case UNIX_SOCKET:
        sdomain = AF_UNIX;
        stype = SOCK_STREAM;
        sproto = 0;
        break;
    default:
        _handle_err_and_clean("Unsupported socket type");
        return -1;
    }

    if ((sd = socket(sdomain, stype, sproto)) == 0) {
        _handle_err_and_clean("create socket failed");
        return -1;
    }

    if (sock != NULL) {
        sock->descriptor = sd;
        sock->domain = sdomain;
        sock->type = stype;
        sock->protocol = sproto;
    }

    return sd;
}

int socket_listen(int socket, unsigned short port, const char* path, int backlog, socket_t* sock)
{
    struct sockaddr_in sa;
    struct sockaddr_un su;

    if (path) {
        if (__create_unix_socket(&su, path) < 0)
            return -1;

        if (bind(socket, (struct sockaddr*)&su, sizeof(su)) < 0) {
            _handle_err_and_clean("bind socket");
            return -1;
        }
    } else {
        if (__create_tcp_udp_socket(&sa, port) < 0)
            return -1;

        if (bind(socket, (struct sockaddr*)&sa, sizeof(sa)) < 0) {
            _handle_err_and_clean("bind socket");
            return -1;
        }
    }

    if (listen(socket, backlog) < 0) {
        _handle_err_and_clean("listen failed");
        return -1;
    }

    if (sock != NULL) {
        if (path) {
            sock->su->sun_family = su.sun_family;
            memcpy(sock->su->sun_path, su.sun_path, strlen(su.sun_path) + 1);
        } else {
            sock->sa->sin_family = sa.sin_family;
            sock->sa->sin_addr.s_addr = sa.sin_addr.s_addr;
            sock->sa->sin_port = port;
        }
    }

    return 0;
}

int socket_accept(int socket, struct sockaddr* addr)
{
    struct sockaddr_in sa;
    unsigned int addr_len = sizeof(sa);
    unsigned int* addr_len_p = &addr_len;

    int newsd;
    if ((newsd = accept(socket, (struct sockaddr*)&sa, addr_len_p)) < 0) {
        _handle_err_and_clean("accept failed");
        return -1;
    }

    if (addr)
        sprintf(addr->sa_data, "%s", inet_ntoa(sa.sin_addr));

    return newsd;
}

int socket_connect(int socket, struct sockaddr* addr)
{
    struct sockaddr_in sa = { 0 };
    int addr_len = sizeof(sa);

    int newsd;
    if ((newsd = connect(socket, (struct sockaddr*)&sa, addr_len)) < 0) {
        _handle_err_and_clean("connect failed");
        return -1;
    }

    if (addr)
        memcpy(addr->sa_data, inet_ntoa(sa.sin_addr), sizeof(in_addr_t));

    return newsd;
}

int socket_recv(int socket, void* buff, size_t len, int flags)
{
    return recvfrom(socket, buff, len, flags, NULL, NULL);
}

int socket_send(int socket, const void* buff, size_t len, int flags)
{
    return sendto(socket, buff, len, flags, NULL, 0);
}

void socket_close(int socket, const char* path)
{
    _close_socket(socket);

    if (path)
        unlink(path);
}

int socket_poll(int listen_sock, const poll_config_t* config)
{
#ifdef __WIN32
    WSAPOLLFD fds[config->nfds];
    int (*evpoll)(WSAPOLLFD*, unsigned long, int) = WSAPoll;
#else
    // TODO change for dynamic allocation
    struct pollfd fds[config->nfds];
    int (*evpoll)(struct pollfd*, nfds_t, int) = poll;
#endif
    struct sockaddr addr;
    int events = 0, n = 1, ret, i;
    fds[0].fd = listen_sock;
    fds[0].events = config->levents;

    __set_sighandler();

    do {
        events = evpoll(fds, n, config->timeout);
        if (events) {
            if (fds[0].revents) {
                fds[n].fd = config->lev_handler(fds[0].fd, fds[0].revents, &addr);
                fds[n].events = config->events;
                ++n;
            } else {
                for (i = 1; i < n; ++i)
                    if (fds[i].revents) {
                        ret = config->ev_handler(fds[i].fd, fds[i].revents, &addr);
                        if (!ret) {
                            close(fds[i].fd);
                            --n;
                        }
                    }
            }
        } else {
            //
        }
    } while (__listening);

    return 0;
}
