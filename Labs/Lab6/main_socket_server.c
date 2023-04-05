// main_socket_server.c
// Description:
//      This program is the server side of the socket and copies the binary
//      data of the file that a user sends to a new file in the current
//      directory of the same or similar name. The limitations for reading and
//      writing size is adjusted in socket_packet_info.h to ensure srever
//      and client match sizes. This program takes in no parameters from the
//      command line and is exited with "^\" or "^C". If socket is successfuly
//      created, the program will not exit until either signal is given. The
//      program will only allow 1 connection at a time and will kick other
//      connection attempts until there are no active connections.

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

#include "socket_packet_info.h"

void catcher(int signo);             // function handles SIGINT and SIGUSR1
int createFileName(char *filename);  // creates file to ensure that file name does not already exist in directory

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
    if ((socket_desc = socket(AF_INET, SOCK_STREAM, 0)) < 0) {  // create socket connection
        perror("socket call failed");
        exit(1);
    }

    srv.sin_family = AF_INET;                 // type of address
    srv.sin_port = htons(CONNECT_PORT);       // formats port to universal value
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
        newfd = accept(socket_desc, (struct sockaddr *__restrict__)&cli, (socklen_t *)&cli_len);  // accept new client connections
        upload_complete = 0;                                                                      // set flag for receiving client data
        int i;
        for (i = 0; i < sizeof(filename); i++) {  // clear filename for next file
            filename[i] = '\0';
        }
        connections++;                     // increment number of active connections
        if (connections <= MAX_CONNECT) {  // check if we are at limit of connections
            printf("\n***New Made: %d\n", newfd);
            printf("***Current connections: %d\n\n", connections);
            if (newfd < 0) {  // check if newfd was created
                perror("Accept call failed");
                close(socket_desc);  // colse connection
                done = 1;            // exit loop
            } else {
                recv(newfd, &recv_packet, sizeof(recv_packet), 0);                               // gets entire packet
                strncpy(filename, recv_packet.data, recv_packet.nbytes);                         // saves filename to variable
                printf("Receiving file '%s'...\n", recv_packet.data);   // server prompt of filename
                copy_file = createFileName(filename);                                            // create file with name of sent file
                while (!upload_complete && recv(newfd, &recv_packet, sizeof(recv_packet), 0)) {  // while data is being received
                    write(copy_file, recv_packet.data, recv_packet.nbytes);                      // write to copy file from received data
                    if (recv_packet.nbytes < N_BTYTES) {                                         // exit this loop if at the end of file
                        upload_complete = 1;
                    }
                }
                strcpy(send_packet.data, "\nSERVER MESSAGE:\n\t***FILE UPLOAD SUCCESSFUL***\n\n\0");
                send_packet.nbytes = strlen(send_packet.data);      // save size of message to nbytes
                send(newfd, &send_packet, sizeof(send_packet), 0);  // send packet
                close(newfd);                                       // close connection to client
                printf("***File upload success. Disconnected from client.\n\n");
                printf("***Current connections: %d\n\n", --connections);
            }
        } else {  // close connection if number of connections is exceeded
            printf("***Connection with %d aborted, server full!\n", newfd);
            printf("***Current connections: %d\n\n", --connections);
            close(newfd);   // close connection
            connections--;  // decrement connections counter
        }
    }
    close(socket_desc);  // close socket

    return 0;
}

// void catcher(int signo)
// Parameters:
//      int signo: the int representtion of the signal that was given
// Description:
//      This function catches the SIGINT and SIGUSR1 signals to ensure that the
//      file descriptors are closed before the program exits and displays an
//      appropriate message.

void catcher(int signo) {
    if (signo == SIGUSR1) {  // catches SIGUSR1
        printf("***Closing connection with %d\n", newfd);
        printf("***Current connections: %d\n\n", --connections);
        close(socket_desc);  // closes socket
        close(newfd);        // close read/write file
        exit(0);             // exits program
    } else {
        printf(" Terminating Server\n");  // catches SIGINT
        close(socket_desc);               // closes socket
        close(newfd);                     // closes read/write file
        exit(0);                          // exits program
    }
}

// int createFileName(char filename[])
// Parameters:
//      char filename[]: a char array that contains the filename given by the client program
// Description:
//      This function ensures that the filename created for the copied file does not already exist.
//      If the file being copied is found in the directory that you are copying to, filename.ext
//      becomes filename_1.ext to ensure no overlap and allow multiple copies.
int createFileName(char filename[]) {
    if (access(filename, F_OK)) {  // if filename doesnt exist in current directory
        printf("%s copied\n", filename);
        return open(filename, O_CREAT | O_RDWR, 00744);  // return filedescriptor
    } else {
        char new_filename[500];                                             // used to hold altered filename
        int i = 1;                                                          // counter for do-while loop
        int file_ext = 0;                                                   // index of '.' in filename
        while (filename[file_ext] != '.' && file_ext < strlen(filename)) {  // if current char == '.' or exceeded filename bounds
            file_ext++;                                                     // go to next index
        }
        char filename_header[500];                     // used to hold part of filename up to '.' and extension
        strncpy(filename_header, filename, file_ext);  // adds filename until first '.'
        do {
            i++;
            sprintf(new_filename, "%s_%d", filename_header, i);  // new_filename becomes original filename with added number for differentiation
            strcat(new_filename, filename + file_ext);           // add on original file extension
        } while (!access(new_filename, F_OK));

        printf("%s created\n", new_filename);                // server prompt for file creation
        return open(new_filename, O_CREAT | O_RDWR, 00744);  // creates file and returns filedescriptor
    }
}