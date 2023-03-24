#include <ctype.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

void catcher(int signo);

typedef struct {
    int nbytes;
    char data[1024];
} transfer;
int newfd;
int socket_desc;  // socket descriptor
int done = 0;

const int MAX_CONNECT = 1, CONNECT_PORT = 51614;

int connections = 0;

int main(int argc, char const *argv[]) {
    struct sockaddr_in srv;
    struct sockaddr_in cli;
    int cli_len = sizeof(cli);
    int bytes_read;
    transfer datastream;
    datastream.nbytes = 1024;

    signal(SIGPIPE, catcher);
    signal(SIGINT, catcher);
    signal(SIGUSR1, catcher);

    if ((socket_desc = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket call failed");
        exit(1);
    }

    srv.sin_family = AF_INET;
    srv.sin_port = htons(CONNECT_PORT);
    srv.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(socket_desc, (struct sockaddr *)&srv, sizeof(srv)) < 0) {
        perror("bind call failed");
        exit(1);
    }
    if (listen(socket_desc, MAX_CONNECT) < 0) {  // handles up to 2 connections
        perror("listen call failed");
        exit(1);
    }

    printf("\n");
    printf("-------------------\n");
    printf("| *SERVER ACTIVE* |\n");
    printf("-------------------\n");
    printf("\n");

    while (!done) {
        newfd = accept(socket_desc, (struct sockaddr *)&cli, &cli_len);
        connections++;
        if (connections <= MAX_CONNECT) {
            printf("\n***New Connection: %d\n", newfd);
            printf("***Current connections: %d\n\n", connections);
            if (newfd < 0) {
                perror("Accept call failed");
                close(socket_desc);
                done = 1;
            } else {
                if (fork() == 0) {
                    while ((bytes_read = read(newfd, datastream.data, datastream.nbytes)) > 0) {
                        datastream.data[bytes_read] = '\0';
                        printf("%s=%d\n", datastream.data, bytes_read);
                    }
                    kill(getppid(), SIGUSR1);
                    kill(getpid(), SIGQUIT);
                } else {
                    // printf("Parent waiting\n");
                    // wait(NULL);
                    close(newfd);
                    // printf("Parent done waiting\n");
                }
            }
        } else {
            printf("***Connection with %d aborted, server full!\n", newfd);
            printf("***Current connections: %d\n\n", --connections);
            close(newfd);
            connections--;
        }
    }
    close(socket_desc);

    return 0;
}

void catcher(int signo) {
    if (signo == SIGUSR1) {
        printf("***Closing connection with %d\n", newfd);
        printf("***Current connections: %d\n\n", --connections);
        close(newfd);
        // exit(0);
    } else {
        printf(" Terminating Server\n");
        close(socket_desc);
        close(newfd);
        exit(0);
    }
}
