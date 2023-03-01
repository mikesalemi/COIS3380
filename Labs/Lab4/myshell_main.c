// myshell_main.c

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
int main() {
    char *cmd[] = {"who", "ls", "date"};
    int i, done = 0;
    while (!done) {
        printf("0 = who 1 = ls 2 = date 3 = terminate: " );
        scanf( "%d", &i);
        if (i != 3) {
            if (fork() == 0) {
                printf( "child starting\n");
                execlp(cmd[i], cmd[i], (char *)NULL);
                printf( "execlp failed\n");
                exit(1);
            }
            else {
                wait((int *)0);
                printf( "child finished\n");
            }
        } else
            done = 1;
    }
    return 0;
}
