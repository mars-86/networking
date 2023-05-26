#include <stdio.h>
#include <string.h>
#include "networking.h"

const char *generate_headers(char *dest, HTTP_STATUS_CODES status, const char *body)
{
    sprintf(dest,
            "HTTP/1.1 %llu OK\r\n"
            "Content-Type: text/HTML\r\n"
            "Content-Length: %u\r\n\r\n"
            "%s", status, strlen(body), body);
    return dest;
}
