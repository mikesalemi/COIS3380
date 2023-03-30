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

#include "socket_packet_struct.h"

void catcher(int signo);
int createFileName(char *filename);

int newfd;        // read/write file between srv and cli
int socket_desc;  // socket descriptor
int done = 0;     // global progran cycle flag. needs scope in sig handler

const int MAX_CONNECT = 1,  // maximum number of allowable connections
    CONNECT_PORT = 51614;   // my port on Loki router

int connections = 0;  // global variable to disallow too many connections

int main(int argc, char const *argv[]) {
    struct sockaddr_in srv;  // server ip info
    struct sockaddr_in cli;  // client ip info
    int cli_len = sizeof(cli);
    int copy_file;        // copied file
    char filename[450];   // filename buffer
    packet send_packet;   // holds data being sent to client
    packet recv_packet;   // holds data received from client
    int upload_complete;  // flag to disconnect from client

    // SIGNAL HANDLING
    signal(SIGINT, catcher);
    signal(SIGUSR1, catcher);

    // SOCKET CREATION
    if ((socket_desc = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket call failed");
        exit(1);
    }

    srv.sin_family = AF_INET;
    srv.sin_port = htons(CONNECT_PORT);
    srv.sin_addr.s_addr = htonl(INADDR_ANY);  // use any ip address available

    if (bind(socket_desc, (struct sockaddr *)&srv, sizeof(srv)) < 0) {  // bind socket to port
        perror("bind call failed");
        exit(1);
    }
    if (listen(socket_desc, MAX_CONNECT) < 0) {  // handles up to 1 connection
        perror("listen call failed");
        exit(1);
    }

    printf("\n");
    printf("-------------------\n");
    printf("| *SERVER ACTIVE* |\n");
    printf("-------------------\n");
    printf("\n");

    // READING/WRITING
    while (!done) {
        newfd = accept(socket_desc, (struct sockaddr *)&cli, &cli_len);  // accept new client connections
        upload_complete = 0;                                             // set flag for receiving client data
        connections++;                                                   // increment number of active connections
        if (connections <= MAX_CONNECT) {                                // check if we are at limit of connections
            printf("\n***New Connection: %d\n", newfd);
            printf("***Current connections: %d\n\n", connections);
            if (newfd < 0) {  // check if newfd was created
                perror("Accept call failed");
                close(socket_desc);  // colse connection
                done = 1;            // exit loop
            } else {
                /////////////////////////////
                recv(newfd, &recv_packet, sizeof(recv_packet), 0);                               // gets entire packet
                strncpy(filename, recv_packet.data, recv_packet.nbytes);                         // saves filename to variable
                copy_file = createFileName(filename);                             // create file with name of sent file
                printf("filename=%s\n", filename);                                               // server prompt of filename
                while (!upload_complete && recv(newfd, &recv_packet, sizeof(recv_packet), 0)) {  // while data is being received
                    write(copy_file, recv_packet.data, recv_packet.nbytes);                      // write to copy file from received data
                    if (recv_packet.nbytes < N_BTYTES) {                                         // exit this loop if at the end of file
                        upload_complete = 1;
                    }
                }
                strcpy(send_packet.data, "\nSERVER MESSAGE:\n\t***FILE UPLOAD SUCCESSFUL***\n\n\0");
                send_packet.nbytes = strlen(send_packet.data);
                send(newfd, &send_packet, sizeof(send_packet), 0);
                close(newfd);
                printf("***File upload success. Disconnected from client.\n\n");
                connections--;
            }
            /////////////////////////////////////

        } else {  // close connection if number of connections is exceeded
            printf("***Connection with %d aborted, server full!\n", newfd);
            printf("***Current connections: %d\n\n", --connections);
            close(newfd);   // close connection
            connections--;  // decrement connections counter
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

int createFileName(char filename[]) {
    if (access(filename, F_OK)) {                        // if filename doesnt exist in current directory
        printf("%s created\n", filename);
        return open(filename, O_CREAT | O_RDWR, 00744);  // return filedescriptor
    } else {
        char new_filename[500];
        int i = 0;
        int file_ext = 0;
        while (filename[file_ext] != '.' && file_ext < strlen(filename)) {
            file_ext++;
        }
        char filename_header[500];
        strncpy(filename_header, filename, file_ext);
        do {
            // new_filename[0] = '\0';
            i++;
            sprintf(new_filename, "%s_%d", filename_header, i);
            strcat(new_filename, filename + file_ext);
        } while (!access(new_filename, F_OK));

        printf("%s created\n", new_filename);
        return open(new_filename, O_CREAT | O_RDWR, 00744);
    }
}