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

#include "socket_packet_info.h"

void catcher(int signo); // function handles SIGINT and SIGUSR1

const int CONNECT_PORT = 51614;// my port on Loki router
int socket_desc; // socket file descriptor

int main(int argc, char const *argv[]) {
    struct sockaddr_in srv; // server ip info
    socket_desc = socket(AF_INET, SOCK_STREAM, 0); // creates socket connection
    int done = 0; // program exit flag
    packet recv_packet; // holds data received from server
    packet send_packet; // holds data to be sent to server

    // SIGNAL HANDLING
    signal(SIGPIPE, catcher);
    signal(SIGINT, catcher);

    // SOCKET CONNECTION
    if (socket_desc < 0) { // check if socket connection was successful
        perror("Socket creation failed");
        exit(1);
    }

    srv.sin_family = AF_INET; // type of ip
    srv.sin_port = htons(CONNECT_PORT); // format ip to universal format
    srv.sin_addr.s_addr = htonl(INADDR_ANY); // use any available IP

    if ((connect(socket_desc, (struct sockaddr *)&srv, sizeof(srv))) < 0) { // connect to server
        perror("Could not connect to server");
        exit(1);
    }

    char filename[450]; // holds file name given as command line argument
    strcpy(filename, argv[1]); // copys argument to variable
    int readfile; // filedescriptor for file ot be copied
    if ((readfile = open(filename, O_RDONLY))) { // check if file can be opened/exists
        send_packet.nbytes = strlen(filename); // prepare length of filename to be sent 
        strcpy(send_packet.data, filename); // copy filename to struct to be sent
        send(socket_desc, &send_packet, sizeof(send_packet), 0); // send struct containing filename
        printf("Sending file %s...\n", send_packet.data);
    } else { // if file could not be opened or found
        printf("%s could not be opened...\n", filename);
        exit(1);
    }

    // READING/WRITING
    do {
        send_packet.nbytes = read(readfile, send_packet.data, N_BTYTES);  // read N_BYTES from file
        send_packet.data[send_packet.nbytes] = '\0';
        if (send_packet.nbytes > 0) { // check if data was read. should only be used if file size is divisible by N_BYTES macro
            send(socket_desc, &send_packet, sizeof(send_packet), 0);  // send read filedata to server
        } else {
            done = 1; // exit flag activated
        }
    } while (send_packet.nbytes == N_BTYTES && !done); // loops until file is done being copied
    printf("Done sending file.\n");
    recv(socket_desc, &recv_packet, sizeof(recv_packet), 0); // wait for confirmation from the server
    printf("%s", recv_packet.data);
    close(socket_desc); // close socket connection
    printf("Goodbye!\n\n");
    return 0; // exit program
}


// void catcher(int signo)
// Parameters:
//      int signo: the int representtion of the signal that was given
// Description:
//      This function catches the SIGINT and SIGPIPE signals to ensure that the
//      file descriptors are closed before the program exits and displays an
//      appropriate message. Program disconnects if connection to socket is lost. 
void catcher(int signo) {
    printf("Connection to server lost!\n");
    close(socket_desc); // close socket
    exit(1); // exit program
}