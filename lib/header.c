#include "networking.h"
#include "sys/common.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char* generate_headers(char* dest, HTTP_STATUS_CODES status, const char* body)
{
    sprintf(dest,
        "HTTP/1.1 %u OK\r\n"
        "Content-Type: text/HTML\r\n"
        "Content-Length: %ld\r\n\r\n"
        "%s",
        status, strlen(body), body);

    return dest;
}

char* headers_raw_new(HTTP_STATUS_CODES status, const char* body)
{
    /*
    size_t __length = 0;
    const char* __static = "HTTP/1.1 %u OK\r\n"
                           "Content-Type: text/HTML\r\n"
                           "Content-Length: %ld\r\n\r\n"
                           "%s",
                           status, strlen(body), body);
    */
    return NULL;
}

void headers_destroy(void* headers)
{
    free(headers);
}
