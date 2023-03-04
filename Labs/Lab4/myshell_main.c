// myshell_main.c

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "myshell_functions.h"

int main() {
    // char *cmd[] = {"help", "la", "time", "mention", "urun", "usize"};
    // int commands_amount = 6;
    char command[20], option[20];  // option is first part of input, option is rest of input
    int running = 1;               // flag for program exit

    getlogin_r(username, 30);  // gets username of logged in user

    printHeader();  // prints header

    while (running) {                            // program loop
        printf("[%s]-> ", username);             // user prompt with loki username
        getinput(command, option);               // read in input and option
        if (!strcmp(command, "bye")) {           // if user wrote "bye"
            running = bye();                     // program is done running
        } else {                                 // for all input aside from "bye"
            if (!fork()) {                       // create child process
                if (!strcmp(command, "help")) {  // if user wrote help
                    help();
                    exit(0);                               // kill child
                } else if (!strcmp(command, "mention")) {  // if user called mention
                    mention(option);
                } else if (!strcmp(command, "la")) {  // if user called la
                    la();
                } else if (!strcmp(command, "urun")) {  // if user called urun
                    urun(option);
                } else if (!strcmp(command, "time")) {  // if user called time
                    time();
                } else if (!strcmp(command, "usize")) {  // if user called usize
                    usize(option);
                } else if (!strcmp(command, "clear")) {  // if user called clear
                    clear();
                    printHeader();
                    exit(0);
                } else {
                    printf("Invalid command. Try \"help\".\n");
                    exit(0);  // kill child
                }
            } else {
                wait(NULL);
            }
        }
    }
    return 0;
}
