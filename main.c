#include <stdio.h>
#include "networking.h"

int main(void)
{
    int sock = socket_create(TCP_SOCKET, NULL);

    struct sockaddr_in sa;
    sa.sin_family = AF_INET6;
    sa.sin_addr.s_addr = inet_addr("2a01:7e00:e000:2fc::4");
    int nsock = connection_to(sock, NULL, &sa);
    
    printf("%d\n", nsock);
    socket_close(sock);
    socket_close(nsock);

    return 0;
}
