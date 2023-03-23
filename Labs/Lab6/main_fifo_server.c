#include <ctype.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdio.h>
#include <sys/socket.h>

#include <>

typedef struct {
    int nbytes;
    char data[1024];
} transfer;
int newfd;

int main(int argc, char const *argv[]) {
    int socket_desc;
    struct sockaddr_in srv;
    struct sockaddr_in cli;
    int cli_len = sizeof(cli);
    int done = 0;

    signal(SIGPIPE, catcher);

    if ((socket_desc = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket call failed");
        exit(1);
    }
    srv.sin_family = AF_INET;
    srv.sin_port = htons(51614);
    srv.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(socket_desc, (struct sockaddr *)&srv), sizeof(srv) < 0) {
        perror("bind call failed");
        exit(1);
    }
    if (listen(socket_desc,1)<0) {
        perror("listen call failed");
        exit(1);
    }

    while (!done) {
        newfd = accept(socket_desc, (struct sockaddr *) &cli, &cli_len);
        if (newfd < 0)
        {
            perror("Accept call failed");
            done = 1;
        }
        
    }

    return 0;
}
