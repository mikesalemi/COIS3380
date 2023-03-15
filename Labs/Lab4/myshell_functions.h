#ifndef MYSHELL_FUNCTIONS_H
#define MYSHELL_FUNCTIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

char username[30];  // global variable for username
/*
printHeader()
Description: prints header for shell
*/
void printHeader();
/*
clear()
Description: creates child and then clears screen
*/
void clear();
/*
getinput(char *command, char *option)
Description: reads from standard input the user's prompt and sepseates fields by spaces
             first space seperates command and option afterwards. Maximum 200 chars.
*/
void getinput(char *command, char *option);
/*
la()
Description: calls the ls function with -la option for long listing of all files
*/
void la();
/*
mention(char *option)
Description: calls grep on all files recursively in current directory and returns
             lines and files with the option text found within.
*/
void mention(char *option);
/*
urun(char *option)
Description: Displays all running processes running by user, and if given by specified user option.
*/
void urun(char *option);
/*
help()
Description: Displays all commands and what they do.
*/
void help();
/*
time()
Description: Uses the date function to return only the current time.
*/
void time();
/*
usize(char *option)
Description: prints size of home directory of either currentuser or username given in option.
*/
void usize(char *option);
/*
bye()
Description: process called for killing program. returns 0 to end program loop and
             displays exit message.
*/
int bye();

#endif  // MYSHELL_FUNCTIONS_H