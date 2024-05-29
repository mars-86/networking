#include "../networking.h"
#include <string.h>

#define END_OF_HEADERS(headers) (*headers == '\r' && *(headers + 1) == '\n')
#define GOTO_NEXT_LINE(text)    \
    do {                        \
        while (*text++ != '\n') \
            ;                   \
        text++;                 \
    } while (0)

void parse_headers(header_raw_t headers, const char* rheaders)
{
    const char* __sheaders;
    for (__sheaders = rheaders; *__sheaders != '\0'; __sheaders++)
        ;
}

char* header_value(char* dest, header_raw_t headers, const char* key)
{
    char* __dest = dest;
    int i, j;

    for (i = 0; headers[i] != NULL; ++i) {
        for (j = 0; headers[i][j] != ':'; ++j) {
            if (headers[i][j] != key[j])
                break;
        }
        if (headers[i][j] == ':') {
            // move j forward to avoid copying white space
            for (j = j + 1; headers[i][j] != '\r'; ++j)
                *__dest = headers[i][j];
        }
    }

    return dest;
}

char* header_value2(char* dest, const char* headers, const char* key)
{
    char* __dest = dest;
    int match = 0;

    // skip version protocol line
    GOTO_NEXT_LINE(headers);

    const char* __key = NULL;
    while (!(END_OF_HEADERS(headers))) {
        for (__key = key; *headers == *__key; headers++, __key++) {
            if (*headers == ':') {
                match = 1;
                break;
            }
        }
        if (match)
            break;

        GOTO_NEXT_LINE(headers);
    }

    while (*headers != '\r')
        *__dest++ = *headers++;
    *__dest = '\0';

    return dest;
}
