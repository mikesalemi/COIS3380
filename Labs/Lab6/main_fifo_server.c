#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

typedef struct {
    int nbytes;
    char data[1024];
} transfer;

int main(int argc, char const *argv[]) {
    int filenum, fifo_desc, charcount, fifonum;
    char filename[400], buffer[1024];
    char *fifo_toclient = "FIFO_TO_CLIENT";
    transfer send;
    transfer get;
    int pid = getpid();

    if (!(filenum = open(filename, O_CREAT | O_RDONLY, 777))) {
        printf("File could not be read or does not exist.\n");
        return 1;
    }

    fifo_desc = mkfifo(fifo_toclient, 0777);

    if (fifo_desc == -1) {
        if (errno != EEXIST) {
            printf("Could not create FIFO!\n");
            return 1;
        }
        printf("FIFO already exists\n");
    }

    // get filename
    open("FIFO_TO_SERVER", O_RDONLY);
    

    printf("Writing to FIFO\n");
    charcount = lseek(filenum, 0, SEEK_CUR);
    fifonum = open(fifo_toclient, O_WRONLY);
    while (read(filenum, buffer, 1024)) {
        write(fifonum, buffer, 1024);
        lseek(filenum, 1024, SEEK_CUR);
        printf("%s\n", buffer);
    }
    printf("Done writing to FIFO\n");

    remove(fifo_toserver);

    return 0;
}
