#include <ctype.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

void catcher(int signo);

typedef struct {
    int nbytes;
    char data[1024];
} transfer;

const int CONNECT_PORT = 51614;
int socket_desc;

int main(int argc, char const *argv[]) {
    struct sockaddr_in srv;
    socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    int done = 0;
    transfer datastream;

    signal(SIGPIPE, catcher);
    signal(SIGINT, catcher);

    if (socket_desc < 0) {
        perror("Socket creation failed");
        exit(1);
    }

    srv.sin_family = AF_INET;
    srv.sin_port = htons(CONNECT_PORT);
    srv.sin_addr.s_addr = htonl(INADDR_ANY);

    if ((connect(socket_desc, (struct sockaddr *)&srv, sizeof(srv))) < 0) {
        perror("Could not connect to server");
        exit(1);
    }

    while (!done) {
        send(socket_desc, "", 0, 0);
        printf("\t> ");
        scanf(" %1023[^\n]", datastream.data);
        // datastream.data[strlen(datastream.data) - 1] = '\0';
        datastream.nbytes = strlen(datastream.data);
        printf("%s=%d\n", datastream.data, datastream.nbytes);
        if (strncmp(datastream.data, "bye", 3)) {
            printf("sending...\n");
            send(socket_desc, &datastream.data, datastream.nbytes, 0);
        } else {
            done = 1;
        }
    }
    close(socket_desc);
    printf("Goodbye!\n\n");
    return 0;
}

void catcher(int signo) {
    printf("Connection to server lost!\n");
    close(socket_desc);
    exit(1);
}