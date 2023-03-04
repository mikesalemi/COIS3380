#ifndef MYSHELL_FUNCTIONS_H
#define MYSHELL_FUNCTIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

char username[30];  // global variable for username

void printHeader();

void clear();

void getinput(char *command, char *option);

void la();

void mention(char *option);

void urun(char *option);

void help();

void time();

void usize(char *option);

int bye();

#endif  // MYSHELL_FUNCTIONS_H