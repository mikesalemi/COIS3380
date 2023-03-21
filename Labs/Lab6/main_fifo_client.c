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
    char filename[400];
    char *fifo_toserver = "FIFO_TO_SERVER";
    transfer send;
    transfer get;
    int pid = getpid();

    if (argc != 2) {
        printf("Must specify 1 fliename in function call.");
        return 1;
    }

    strcpy(filename, argv[1]);

    if (!(filenum = open(filename, O_RDONLY))) {
        printf("File could not be read or does not exist.\n");
        return 1;
    }

    fifo_desc = mkfifo(fifo_toserver, 0777);

    if (fifo_desc == -1) {
        if (errno != EEXIST) {
            printf("Could not create FIFO!\n");
            return 1;
        }
        printf("FIFO already exists\n");
    }

    printf("Sending filename\n");
    strcyp(send.data, filename);
    send.nbytes = sizeof(filename);
    write(fifonum, send.data, send.nbytes);

    printf("Writing to FIFO\n");
    charcount = lseek(filenum, 0, SEEK_CUR);
    fifonum = open(fifo_toserver, O_WRONLY);
    while ((send.nbytes = read(filenum, send.data, 1024))) {
        write(fifonum, send.data, send.nbytes);
        lseek(filenum, send.nbytes, SEEK_CUR);
        printf("%s\n", send.data);
    }
    printf("Done writing to FIFO\n");

    remove(fifo_toserver);

    return 0;
}
