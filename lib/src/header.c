#include "../networking.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HTTP_VERSION "HTTP/1.1"
#define HTTP_STATUS(code, status) HTTP_VERSION " " code " " status "\r\n"

const char* __get_http_status_text(HTTP_STATUS_CODES status)
{
    switch (status) {
    case CONTINUE:
        return HTTP_STATUS("100", "Continue");
    case SWITCHING_PROTOCOLS:
        return HTTP_STATUS("101", "Switching Protocols");
    case PROCESSING:
        return HTTP_STATUS("102", "Processing");
    case EARLY_HINTS:
        return HTTP_STATUS("103", "Early Hints");
        // 2xx SUCCESS
    case OK:
        return HTTP_STATUS("200", "OK");
    case CREATED:
        return HTTP_STATUS("201", "Created");
    case ACCEPTED:
        return HTTP_STATUS("202", "Accepted");
    case NON_AUTHORITATIVE_INFORMATION:
        return HTTP_STATUS("203", "Non-Authoritative Information");
    case NO_CONTENT:
        return HTTP_STATUS("204", "No Content");
    case RESET_CONTENT:
        return HTTP_STATUS("205", "Reset Content");
    case PARTIAL_CONTENT:
        return HTTP_STATUS("206", "Partial Content");
    case MULTI_STATUS:
        return HTTP_STATUS("207", "Multi-Status");
    case ALREADY_REPORTED:
        return HTTP_STATUS("208", "Already Reported");
    case IM_USED:
        return HTTP_STATUS("226", "IM Used");
    // 3xx REDIRECTION
    case MULTIPLE_CHOICES:
        return HTTP_STATUS("300", "Multiple Choices");
    case MOVED_PERMANENTLY:
        return HTTP_STATUS("301", "Moved Permanently");
    case FOUND:
        return HTTP_STATUS("302", "Found");
    case SEE_OTHER:
        return HTTP_STATUS("303", "See Other");
    case NOT_MODIFIED:
        return HTTP_STATUS("304", "Not Modified");
    case USE_PROXY:
        return HTTP_STATUS("305", "Use Proxy");
    case SWITCH_PROXY:
        return HTTP_STATUS("306", "Switch Proxy");
    case TEMPORARY_REDIRECT:
        return HTTP_STATUS("307", "Temporary Redirect");
    case PERMANENT_REDIRECT:
        return HTTP_STATUS("308", "Permanent Redirect");
    // 4xx CLIENT ERRORS
    case BAD_REQUEST:
        return HTTP_STATUS("400", "Bad Request");
    case UNAUTHORIZED:
        return HTTP_STATUS("401", "Unauthorized");
    case PAYMENT_REQUIRED:
        return HTTP_STATUS("402", "Payment Required");
    case FORBIDDEN:
        return HTTP_STATUS("403", "Forbidden");
    case NOT_FOUND:
        return HTTP_STATUS("404", "Not Found");
    case METHOD_NOT_ALLOWED:
        return HTTP_STATUS("405", "Method Not Allowed");
    case NOT_ACCEPTABLE:
        return HTTP_STATUS("406", "Not Acceptable");
    case PROXY_AUTHENTICATION_REQUIRED:
        return HTTP_STATUS("407", "Proxy Authentication Required");
    case REQUEST_TIMEOUT:
        return HTTP_STATUS("408", "Request Timeout");
    case CONFLICT:
        return HTTP_STATUS("409", "Conflict");
    case GONE:
        return HTTP_STATUS("410", "Gone");
    case LENGTH_REQUIRED:
        return HTTP_STATUS("411", "Length Required");
    case PRECONDITION_FAILED:
        return HTTP_STATUS("412", "Precondition Failed");
    case PAYLOAD_TOO_LARGE:
        return HTTP_STATUS("413", "Payload Too Large");
    case URI_TOO_LONG:
        return HTTP_STATUS("414", "URI Too Large");
    case UNSUPPORTED_MEDIA_TYPE:
        return HTTP_STATUS("415", "Unsupported Media Type");
    case REQUESTED_RANGE_NOT_SATISFIABLE:
        return HTTP_STATUS("416", "Range Not Satisfiable");
    case EXPECTATION_FAILED:
        return HTTP_STATUS("417", "Expectation Failed");
    case IM_A_TEAPOT:
        return HTTP_STATUS("418", "I'm a teapot");
    case MISDIRECTED_REQUEST:
        return HTTP_STATUS("421", "Misdirected Request");
    case UNPROCESSABLE_ENTITY:
        return HTTP_STATUS("422", "Unprocessable Entity");
    case LOCKED:
        return HTTP_STATUS("423", "Locked");
    case FAILED_DEPENDENCY:
        return HTTP_STATUS("424", "Failed Dependency");
    case TOO_EARLY:
        return HTTP_STATUS("425", "Too Early");
    case UPGRADE_REQUIRED:
        return HTTP_STATUS("426", "Upgrade Required");
    case PRECONDITION_REQUIRED:
        return HTTP_STATUS("428", "Precondition Required");
    case TOO_MANY_REQUESTS:
        return HTTP_STATUS("429", "Too Many Requests");
    case REQUEST_HEADER_FIELDS_TOO_LARGE:
        return HTTP_STATUS("431", "Request Header Fields Too Large");
    case UNAVAILABLE_FOR_LEGAL_REASONS:
        return HTTP_STATUS("451", "Unavailable For Legar Reasons");
    // 5xx SERVER ERRORS
    case INTERNAL_SERVER_ERROR:
        return HTTP_STATUS("500", "Internal Server Error");
    case NOT_IMPLEMENTED:
        return HTTP_STATUS("501", "Not Implemented");
    case BAD_GATEWAY:
        return HTTP_STATUS("502", "Bad Gateway");
    case SERVICE_UNAVAILABLE:
        return HTTP_STATUS("503", "Service Unavailable");
    case GATEWAY_TIMEOUT:
        return HTTP_STATUS("504", "Gateway Timeout");
    case HTTP_VERSION_NOT_SUPPORTED:
        return HTTP_STATUS("505", "HTTP Version Not Supported");
    case VARIANT_ALSO_NEGOTIATES:
        return HTTP_STATUS("506", "Variant Also Negotiates");
    case INSUFFICIENT_STORAGE:
        return HTTP_STATUS("507", "Insufficient Storage");
    case LOOP_DETECTED:
        return HTTP_STATUS("508", "Loop Detected");
    case NOT_EXTENDED:
        return HTTP_STATUS("510", "Not Extended");
    case NETWORK_AUTHENTICATION_REQUIRED:
        return HTTP_STATUS("511", "Network Authentication Required");
    }
    return "";
}

static const char* content_type[] = {
    "text/plain",
    "application/javascript",
    "application/json",
    "text/html",
    "application/xml"
};

const char* set_http_req(char* dest, const char* path, const char* verb)
{
    return NULL;
}

const char* set_http_res(char* dest, HTTP_STATUS_CODES status)
{
    const char* __http_status = __get_http_status_text(status);
    memcpy(dest, __http_status, strlen(__http_status));

    return dest;
}

const char* set_http_headers(char* dest, const char* headers[])
{
    char* __headers[1024];
    size_t __offset = 0;
    int i;

    if (headers) {
        for (i = 0; headers[i] != NULL; ++i) {
            size_t hlen = strlen(headers[i]);
            memcpy(__headers + __offset, headers[i], hlen);
            __offset += hlen;
        }

        memcpy(dest, __headers, strlen((const char*)__headers));
    }

    return dest;
}

const char* set_http_body(char* dest, const char* body)
{
    // TODO replace for memcpy
    sprintf(dest, "Content-Length: %lu\r\n\r\n%s", strlen(body), body);

    return dest;
}

char* headers_new(HTTP_STATUS_CODES status, const char* headers[])
{
    int i;
    for (i = 0; headers[i] != NULL; ++i)
        ;

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

const char* http_response(char* dest, HTTP_STATUS_CODES status, const char** headers, const char* body)
{
    set_http_res(dest, status);
    set_http_headers(dest + strlen(dest), headers);
    set_http_body(dest + strlen(dest), body);

    return dest;
}
