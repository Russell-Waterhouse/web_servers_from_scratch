#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "http.h"
#include "result.h"
#include "fatchar.h"

#define NULL_TERMINATOR '\0'
#define SINGLE_SPACE ' '

RequestResult* request_from_bytes(char* bytes, size_t bytes_len){
    RequestResult* rr = calloc(1, sizeof(RequestResult));
    if (rr == NULL) {
        printf("calloc failed in request_from_bytes\n");
        fflush(stdout);
        return NULL;
    }

    Request* req = calloc(1, sizeof(Request));
    if (req == NULL) {
        printf("calloc failed in request_from_bytes\n");
        fflush(stdout);
        rr->result = FAILURE;
        return rr;
    }
    rr->request = req;

    if (bytes == NULL) {
        printf("#request_from_bytes bytes is null\n");
        fflush(stdout);
        rr->result = FAILURE;
        return rr;
    }

    if (bytes_len < 1) {
        printf("#request_from_bytes bytes_len is less than 1\n");
        rr->result = FAILURE;
        return rr;
    }

    unsigned int i = 0;
    unsigned int buffer_len = 0;
    unsigned short int num_headers = 0;

    // parsing the request method
    while(bytes[i] != SINGLE_SPACE && i < bytes_len) {
        i++;
    }
    i++;

    // parsing the rest of the first line (request PATH, HTTP version)
    while(bytes[i] != '\n' && i > 0 && bytes[i-1] != '\r') {
        i++;
    }
    i++;

    // parsing the request headers
    while(bytes[i] != NULL_TERMINATOR && i < bytes_len) {
        if(bytes[i] == '\n' && i > 0 && bytes[i-1] == '\r') {
            // reached the end of the line in the request
            num_headers++;
        }

        i++;
    }
    req->num_headers = num_headers;

    rr->result = SUCCESS;
    return rr;
}

enum Result deleteRequest(Request* request) {
    if(request == NULL) {
        printf("#deleteRequest request is null\n");
        fflush(stdout);
        return FAILURE;
    }

    free(request->url);
    request->url = NULL;

    free(request->destination);
    request->destination = NULL;

    free(request->mode);
    request->mode = NULL;

    free(request->body);
    request->body = NULL;

    free(request);
    request = NULL;

    return SUCCESS;
}

Result deleteHeaders(Request* request) {
    if (request == NULL) {
        printf("#deleteHeaders request is null\n");
        fflush(stdout);
        return FAILURE;
    }

    int num_headers = request->num_headers;
    if (request->headers == NULL && request->num_headers == 0) {
        return SUCCESS;
    }

    for(int i = 0; i < num_headers; i++) {
        if(request->headers[i] == NULL) {
            printf("#deleteHeaders requestHeader number %d is not initialized\n", i);
            fflush(stdout);
            free(request->headers);
            return FAILURE;
        }
        delete_fat_char(request->headers[i]->key);
        request->headers[i]->key = NULL;
        delete_fat_char(request->headers[i]->value);
        request->headers[i]->value = NULL;
        free(request->headers[i]);
        request->headers[i] = NULL;
    }

    free(request->headers);
    request->headers = NULL;

    return SUCCESS;
}


Result delete_request_result(RequestResult* rr) {
    if(rr == NULL) {
        printf("rr is null\n");
        fflush(stdout);
        return FAILURE;
    }

    if(rr->request == NULL) {
        printf("rr->request is null\n");
        fflush(stdout);
        return FAILURE;
    }

    deleteRequest(rr->request);
    free(rr);
    rr = NULL;

    return SUCCESS;
}

