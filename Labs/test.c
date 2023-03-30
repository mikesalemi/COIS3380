#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main() {
    char filename[50] = "thisisatest.txt";
    char new_filename[500] = "\0";
    int file_ext = 0;
    while (filename[file_ext] != '.' && file_ext < (int)strlen(filename)) {
        file_ext++;
    }
    strncat(new_filename, filename, file_ext);
    printf("%s\n", new_filename);
    return 0;
}
