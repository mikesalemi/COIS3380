#ifndef LS_HELPERS_H
#define LS_HELPERS_H

#include <ctype.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <pwd.h>
#include <grp.h>

int filecount; //global variable for count of files that match the search fields

typedef struct {
    int o_opt;
    int o_val;
    int y_opt;
    int y_val;
    int l_opt;
    int l_val;
    int s_opt;
    int s_val;
} ls_fields;

void set_opts(int argc, char *argv[], ls_fields *fields);

void get_filematch_count(char *f_path, char *f_name, ls_fields fields);

void print_filedata(char *f_path, char *path, ls_fields fields);

int fits_opts(struct stat statBiffer, ls_fields fields);

void print_permissions(struct stat statBuffer);

int print_filesize(struct stat statBuffer);

int print_uid(struct stat statBuffer);

int print_gid(struct stat statBuffer);

int print_inode(struct stat statBuffer);

int print_type(struct stat statBuffer);

int print_date(struct stat statBuffer);

int isNumber(char *input);

#endif  // LS_HELPERS_H