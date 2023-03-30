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

#include "socket_packet_struct.h"

void catcher(int signo);

const int CONNECT_PORT = 51614;
int socket_desc;

int main(int argc, char const *argv[]) {
    struct sockaddr_in srv;
    socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    int done = 0;
    packet recv_packet;
    packet send_packet;

    // SIGNAL HANDLING
    signal(SIGPIPE, catcher);
    signal(SIGINT, catcher);

    // SOCKET CONNECTION
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

    char filename[450];
    strcpy(filename, argv[1]);
    int readfile;
    if ((readfile = open(filename, O_RDONLY))) {
        send_packet.nbytes = strlen(filename);
        strcpy(send_packet.data, filename);
        send(socket_desc, &send_packet, sizeof(send_packet), 0);
        printf("Sending file %s=%d\n", send_packet.data, send_packet.nbytes);
    } else {
        printf("%s could not be opened...\n", filename);
        exit(1);
    }

    // READING/WRITING
    do {
        send_packet.nbytes = read(readfile, send_packet.data, N_BTYTES);  // read N_BYTES from file
        // printf("%s\n", send_packet);
        send_packet.data[send_packet.nbytes] = '\0';
        if (send_packet.nbytes > 0) {
            // printf("sending %d bytes...\n", sizeof(send_packet));
            // printf("%s=%d\n", send_packet.data, send_packet.nbytes);
            send(socket_desc, &send_packet, sizeof(send_packet), 0);  // send read filedata to server
        } else {
            done = 1;
        }
    } while (send_packet.nbytes == N_BTYTES && !done);
    printf("done sending data\n");
    recv(socket_desc, &recv_packet, sizeof(recv_packet), 0);
    printf("%s", recv_packet.data);
    close(socket_desc);
    printf("Goodbye!\n\n");
    return 0;
}

void catcher(int signo) {
    printf("Connection to server lost!\n");
    close(socket_desc);
    exit(1);
}