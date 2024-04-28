#include <stdio.h>
#include <stdlib.h>
#include "networking.h"

void print(int s, struct sockaddr *addr)
{

}

int main(void)
{
    /*
    int sock = socket_create(TCP_SOCKET, NULL);

    struct sockaddr_in sa;
    sa.sin_family = AF_INET6;
    sa.sin_addr.s_addr = inet_addr("2a01:7e00:e000:2fc::4");
    int nsock = connection_to(sock, NULL, &sa);

    printf("%d\n", nsock);
    socket_close(sock);
    socket_close(nsock);
    */

    http_server_config_t sconf;
    sconf.backlog = 32;
    sconf.poll.nfds = 32;
    sconf.poll.levents = POLLIN;
    sconf.poll.lev_handler = connection_accept;
    sconf.poll.events = 0;
    sconf.poll.ev_handler = print;
    sconf.poll.timeout = 10;

    http_server_t *server = http_server_init(&sconf);
    printf("server created\n");
    
    http_server_destroy(server);
    printf("server destroyed\n");

    return 0;
}
