#include "myshell_functions.h"
/*
printHeader()
Description: prints header for shell
*/
void printHeader() {
    clear();
    // super cool header for MiSh (Mike Shell)
    printf("############################\n");
    printf("##------------------------##\n");
    printf("##--Welcome to MikeShell--##\n");
    printf("##------------------------##\n");
    printf("############################\n\n");
}
/*
clear()
Description: creates child and then clears screen
*/
void clear() {
    if (!fork()) {
        execlp("clear", "clear", NULL);  // clear screen
        printf("bad\n");
    } else {
        wait(NULL);
    }
}
/*
getinput(char *command, char *option)
Description: reads from standard input the user's prompt and sepseates fields by spaces
             first space seperates command and option afterwards. Maximum 200 chars.
*/
void getinput(char *command, char *option) {
    char input[200], sep[] = " ";
    char *cur_input;
    fgets(input, 100, stdin);         // saves entire line of input including whitespace
    input[strlen(input) - 1] = '\0';  // change last char from \n to null terminator

    cur_input = strtok(input, sep);  // get first part of input seperated by space
    strcpy(command, cur_input);      // saves this input as command
    cur_input = strtok(NULL, sep);   // gets optional second part of input
    if (cur_input) {                 // will be null if only no options given
        strcpy(option, cur_input);   // saves option
    } else {
        option[0] = '\0';
    }
}
/*
la()
Description: calls the ls function with -la option for long listing of all files
*/
void la() {
    execlp("ls", "ls", "-la", NULL);  // run ls command with -la
}
/*
mention(char *option)
Description: calls grep on all files recursively in current directory and returns
             lines and files with the option text found within.
*/
void mention(char *option) {
    if (strlen(option) != 0) {  // if option is not an empty string
        printf("Finding: %s\n", option);
        char text[200];                                                                        // buffer for grep search text
        text[0] = '\"';                                                                        // first quotation
        strcat(text, option);                                                                  // add user input to search text
        strcat(text, "\"");                                                                    // end quotation
        execlp("grep", "grep", "-r", option, NULL);                                            // run grep command recursively for files containing specified text
    } else {                                                                                   // option is empty string
        printf("\'mention\' must include a parameter. Try \'help\' for more information.\n");  // error prompt
        exit(1);                                                                               // kill child :(
    }
}
/*
urun(char *option)
Description: Displays all running processes running by user, and if given by specified user option.
*/
void urun(char *option) {
    if (strlen(option) == 0) {  // if option is an empty string
        printf("Showing processes for: %s\n", username);
        execlp("ps", "ps", "-u", username, NULL);  // run ps command with option -u and current user
    } else {
        printf("Showing processes for: %s\n", option);
        execlp("ps", "ps", "-u", option, NULL);  // run ps command with -u and given username
    }
}
/*
help()
Description: Displays all commands and what they do.
*/
void help() {
    printf("la                  -Prints data of all files in current directory\n");
    printf("mention [option]    -Searches all files in directory recursively for option.\n");
    printf("                        \\Mandatory option must be a directory path.\n");
    printf("urun [option]       -Displays all processes currently running by user or self.\n");
    printf("                        \\If given, option must be username. Otherwise, current\n");
    printf("                        \\user is used.\n");
    printf("time                -Displays current time.\n");
    printf("clear               -Clears screen\n");
    printf("usize [option]      -Gives the size of the users home directory.\n");
    printf("                        \\If given, option must be username. Otherwise, current\n");
    printf("                        \\user is used.\n");
    printf("help                -Displays description of all commands.\n");
    printf("bye                 -Terminates shell.\n");
}
/*
time()
Description: Uses the date function to return only the current time.
*/
void time() {
    execlp("date", "date", "+%T", NULL);  // display current time only
    printf("zombie\n");
}
/*
usize(char *option)
Description: prints size of home directory of either currentuser or username given in option.
*/
void usize(char *option) {
    if (strlen(option) == 0) {       // if option is an empty string
        char userhome[30];           // buffer for specified users home directory
        strcpy(userhome, "/home/");  // start with "/home/"
        strcat(userhome, username);    // then add specified user for home directory
        printf("Showing home directory size for: %s\n", username);
        execlp("du", "du", "-sh", "/home/michaelsalemi", NULL);  // du command with -sh option and current user's home
    } else {
        printf("Showing home directory size for: %s\n", option);
        char userhome[30];                          // buffer for specified users home directory
        strcpy(userhome, "/home/");                 // start with "/home/"
        strcat(userhome, option);                   // then add specified user for home directory
        execlp("du", "du", "-sh", userhome, NULL);  // du command with -sh for specified user's home directory
    }
}
/*
bye()
Description: process called for killing program. returns 0 to end program loop and
             displays exit message.
*/
int bye() {
    printf("Exiting shell...\n\n");  // good bye prompt
    return 0;
}