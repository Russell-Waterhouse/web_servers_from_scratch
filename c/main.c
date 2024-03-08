#include <stdio.h>
#include <stdlib.h>
#include <sys/_types/_socklen_t.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>
#include <netinet/in.h>
#include <unistd.h>

#define LISTEN_BACKLOG 50
#define SUCCESS 1
#define FAIL -1

typedef struct Header {
    char* key;
    char* value;
} Header;

typedef struct Request {
    char* headers;
    char* numHeaders;
    char* url;
    char* credentials;
    char* destination;
    char* integrity;
    char* method;
    char* mode;
    char* body;
} Request;

typedef struct Response {
    char* type;
    char* url;
    char* headers;
    int numHeaders;
    char* body;
    int status;
    char* statusText;
} Response;


int socket_to_request(char* request_data, size_t request_data_len, Request* r);
int socket_to_request(char* request_data, size_t request_data_len, Request* r) {
    if (r == NULL || request_data == NULL) {
        printf("Received NULL params");
        return FAIL;
    }

    memset(r, 0, sizeof(Request));
    r->method = "GET";

    return SUCCESS;
}


int main(void);

int main(void) {

    // Creates a socket for IPv4, TCP protocol
    int socket_file_descriptor = socket(AF_INET, SOCK_STREAM, 0);
    if(socket_file_descriptor == -1) {
        printf("Error creating the socket");
        fflush(stdout);
        return -1;
    }

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET; // IPv4
    addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK); // 127.0.0.1
    addr.sin_port = htons(9999); //port 9999

    int bind_result = bind(socket_file_descriptor, (struct sockaddr *) &addr, sizeof(addr));
    if(bind_result == -1) {
        printf("Error binding socket\n");
        printf("Errno: %d\n", errno);
        fflush(stdout);
        return -1;
    }

    int listen_result = listen(socket_file_descriptor, LISTEN_BACKLOG);
    if (listen_result == -1) {
        printf("Error listening to socket\n");
        printf("Errno: %d\n", errno);
        fflush(stdout);
        return -1;
    }

    struct sockaddr peer_addr;
    socklen_t peer_addr_size = sizeof(peer_addr);

    while (1) {
        int cfd = accept(
            socket_file_descriptor,
            (struct sockaddr *) &peer_addr,
            &peer_addr_size
        );

        if (cfd == -1) {
            printf("Error accepting a connection");
            fflush(stdout);
            return -1;
        }
        printf("got connection %d\n", cfd);
        fflush(stdout);

        char request[255];
        int msg_len = recv(cfd, (void *)request, sizeof(request), MSG_WAITALL);
        if(msg_len == -1) {
            printf("Error receiving message from socket\n");
            fflush(stdout);
            return -1;
        }
        printf("received %s\n", request);
        Request r;
        r.
        fflush(stdout);

        char* reply = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!";
        send(cfd, reply, strlen(reply), 0);
    }

    if(close(socket_file_descriptor) == -1) {
        printf("Error closing socket");
        fflush(stdout);
        return -1;
    }

    return 0;
}

