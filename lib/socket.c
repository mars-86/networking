#include <stdio.h>
#include "networking.h"

#ifdef __WIN32
#include <winsock2.h>
#define _handle_err(msg) msg // perror_win(msg)
#define _handle_err_and_clean(msg) _handle_err(msg), WSACleanup()
#define _close_socket(socket) closesocket(socket), WSACleanup()
#else
#include <sys/socket.h>
#include <poll.h>
#define _handle_err(msg) perror(msg)
#define _handle_err_and_clean(msg) _handle_err(msg)
#define _close_socket(socket) close(socket)
#endif // __WIN32

int socket_create(socket_type_t type, socket_t *sock)
{
#ifdef __WIN32
    WSADATA wsa;
    if(WSAStartup(MAKEWORD(2,2), &wsa) != 0) {
        _handle_err("WSAStartup failed");
        return -1;
    }
#endif // __WIN32

    int sd, sdomain, stype, sproto;
    switch (type) {
        case TCP_SOCKET:
            sdomain = AF_INET;
            stype = TCP_SOCKET;
            sproto = 0;
        break;
        case UDP_SOCKET:
            sdomain = AF_INET;
            stype = UDP_SOCKET;
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

int socket_listen(int socket, unsigned short port, int backlog, socket_t *sock)
{
    struct sockaddr_in sa;
    sa.sin_family = AF_INET;
    sa.sin_port = htons(port);
    sa.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (bind(socket, (struct sockaddr *)&sa, sizeof(sa)) < 0) {
        _handle_err_and_clean("bind socket");
        return -1;
    }

    if (listen(socket, backlog) < 0) {
		_handle_err_and_clean("listen failed");
        return -1;
	}

    if (sock != NULL) sock->sa = sa;

	return 0;
}

int socket_accept(int socket, struct sockaddr *addr)
{
    struct sockaddr_in sa;
    int addr_len = sizeof(sa);
    int *addr_len_p = &addr_len;

    int newsd;
	if ((newsd = accept(socket, (struct sockaddr *)&sa, addr_len_p)) < 0) {
        _handle_err_and_clean("accept failed");
        return -1;
	}

    if (addr) sprintf(addr->sa_data, "%s", inet_ntoa(sa.sin_addr));

    return newsd;
}

int socket_connect(int socket, struct sockaddr *addr)
{
    struct sockaddr_in sa;
    int addr_len = sizeof(sa);

    int newsd;
    if ((newsd = connect(socket, (struct sockaddr *)&sa, addr_len)) < 0) {
        _handle_err_and_clean("connect failed");
        return -1;
    }

    if (addr) sprintf(addr->sa_data, "%s", inet_ntoa(sa.sin_addr));

    return newsd;
}

int socket_recv(int socket, char *buff, size_t len, int flags)
{
    return recv(socket, buff, len, flags);
}

int socket_send()
{

}

void socket_close(int socket)
{
    _close_socket(socket);
}

int socket_poll(int listen_sock, const poll_config_t *poll)
{
#ifdef __WIN32
    WSAPOLLFD fds[poll->nfds];
    int (*evpoll)(WSAPOLLFD *, unsigned long, int) = WSAPoll;    
#else
    struct pollfd fds[poll->nfds];
    int (*evpoll)(pollfd *, nfds_t, int) = poll;
#endif
    struct sockaddr addr;
    int events = 0, n = 1, i;
    fds[0].fd = listen_sock;
    fds[0].events = POLLIN;

    do {
        events = evpoll(fds, n, poll->timeout);
        if (events) {
            if (fds[0].revents) {
                fds[n].fd = socket_accept(fds[0].fd, &addr);
                fds[n].events = poll->events, ++n;
            } else {
                for (i = 1; i < n; ++i)
                    if (fds[i].revents)
                        poll->handler(fds[i].fd, &addr), --n;
            }
        } else {
            //
        }
    } while (1);
}
