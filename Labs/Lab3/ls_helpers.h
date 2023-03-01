#ifndef LS_HELPERS_H
#define LS_HELPERS_H

#include <ctype.h>
#include <dirent.h>
#include <grp.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

int filecount;  // global variable for count of files that match the search fields

//struct that holds flags and values for possible options
typedef struct {
    int o_opt;  // flag for -o
    int o_val;  // value foor -o
    int y_opt;  // flag for -y
    int y_val;  // value foor -y
    int l_opt;  // flag for -l
    int l_val;  // value foor -l
    int s_opt;  // flag for -s
    int s_val;  // value foor -s
} ls_fields;

////////////////
// void set_opts(int argc, char *argv[], ls_fields *fields)
////////////////
/*
Description:    saves and validates comandline arguments value after each given option
                in a struct that holds whether option was used and what the following
                value was.

Parameters:     int argc: count of command line arguments
                char *argv[]: commandline arguments given as array of char pointers
                ls_fields *fields: fields struct passed by value
*/
void set_opts(int argc, char *argv[], ls_fields *fields);

////////////////
// int get_filematch_count(char *f_path, char *f_name, ls_fields fields)
////////////////
/*
Description:    increments the global variable "int filecount" if the current file meets that fields criteria

Parameters:     char *f_path: path of file including path as string (used for searching if not in cwd)
                char *f_name: name of file as string for printing
                ls_fields *fields: fields struct passed by value (used for matching file)
*/
void get_filematch_count(char *f_path, char *f_name, ls_fields fields);

////////////////
// void print_filedata(char *f_path, char *f_name, ls_fields fields)
////////////////
/*
Description:    Prints file data for a file that matches fields. Calls a function that validates for each stat to be printed.

Parameters:     char *f_path: path of file including path as string (used for searching if not in cwd)
                char *f_name: name of file as string for printing
                ls_fields *fields: fields struct passed by value (used for matching file)
*/
void print_filedata(char *f_path, char *path, ls_fields fields);

////////////////
// int fits_opts(struct stat statBuffer, ls_fields fields)
////////////////
/*
Description:    checks to see if a file meets the requirements given by search fields

Parameters:     struct stat statBuffer: struct containing data for a file
                ls_fields *fields: fields struct passed by value (used for matching file)
*/
int fits_opts(struct stat statBiffer, ls_fields fields);

////////////////
// void print_permissions(struct stat statBuffer)
////////////////
/*
Description:    prints the user, group, other permissions of the file and whether the file is a directory
                if there is permission, r/w/x is printed. If not, "-" is printed.

Parameters:     struct stat statBuffer: struct containing data for a file
*/
void print_permissions(struct stat statBuffer);

////////////////
// int print_filesize(struct stat statBuffer)
////////////////
/*
Description:    prints the size of the file in bytes.

Parameters:     struct stat statBuffer: struct containing data for a file
*/
int print_filesize(struct stat statBuffer);

////////////////
// int print_uid(struct stat statBuffer)
////////////////
/*
Description:    prints the uid of the file as a string.

Parameters:     struct stat statBuffer: struct containing data for a file
*/
int print_uid(struct stat statBuffer);

////////////////
// int print_gid(struct stat statBuffer)
////////////////
/*
Description:    prints the gid of the file as a string.

Parameters:     struct stat statBuffer: struct containing data for a file
*/
int print_gid(struct stat statBuffer);

////////////////
// int print_inode(struct stat statBuffer)
////////////////
/*
Description:    prints the inode number of the file

Parameters:     struct stat statBuffer: struct containing data for a file
*/
int print_inode(struct stat statBuffer);

////////////////
// int print_date(struct stat statBuffer)
////////////////
/*
Description:    prints the last modification date of given file

Parameters:     struct stat statBuffer: struct containing data for a file
*/
int print_date(struct stat statBuffer);

////////////////
// int isNumber(char *input)
////////////////
/*
Description:    checks to see if a given string consists of only digits and is
                safe for atoi.

Parameters:     char *input: string to be tested for digits
*/
int isNumber(char *input);

#endif  // LS_HELPERS_H