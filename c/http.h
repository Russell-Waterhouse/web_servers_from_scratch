#ifndef HTTP_H
#define HTTP_H

#include <stdlib.h>
#include "fatchar.h"
#include "result.h"

typedef enum RequestMethod {
    GET,
    HEAD,
    POST,
    PUT,
    DELETE,
    CONNECT,
    OPTIONS,
    TRACE
} RequestMethod;

typedef struct Header {
    fat_char* key;
    fat_char* value;
} Header;

typedef struct Request {
    Header** headers;
    unsigned short int num_headers;
    fat_char* url;
    fat_char* destination;
    RequestMethod method;
    fat_char* mode;
    fat_char* body;
} Request;

typedef struct Response {
    fat_char* type;
    fat_char* url;
    fat_char* headers;
    int numHeaders;
    fat_char* body;
    int status;
    fat_char* statusText;
} Response;

typedef struct request_result {
    enum Result result;
    Request* request;
} RequestResult;

RequestResult* request_from_bytes(char* bytes, size_t bytes_len);

Result deleteRequest(Request* request);
Result delete_request_result(RequestResult* rr);

#endif

