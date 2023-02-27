// ls_helpers.c

#include "ls_helpers.h"

int filecount = 0;  // global variable for number of files that match fields

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
void set_opts(int argc, char *argv[], ls_fields *fields) {
    int opt; //int value of specified option
    // sets default values to 0
    fields->o_opt = 0;
    fields->y_opt = 0;
    fields->l_opt = 0;
    fields->s_opt = 0;
    while ((opt = getopt(argc, argv, "oyls?:")) != -1) {  // checks if option given is a real option
        // printf("----------------\n");
        switch (opt) {
            case 'o':
                if (optind < argc && isNumber(argv[optind])) {  // check if optind is valid and is a number
                    fields->o_val = atoi(argv[optind]);         // saves option value to fields as int
                    // printf("%c -> %d\n", opt, fields->o_val);
                    fields->o_opt++;  // increments if value was given
                } else {
                    printf("Invalid parameter \"%s\" after -%c option.\n", argv[optind], opt);  // error for invalid value after option
                    exit(1);
                }
                break;
            case 'y':
                if (isNumber(argv[optind]) && optind < argc) {  // check if optind is valid and is a number
                    fields->y_val = atoi(argv[optind]);         // saves option value to fields as int
                    // printf("%c -> %s\n", opt, argv[optind]);
                    fields->y_opt++;  // increments if value was given
                } else {
                    printf("Invalid parameter \"%s\" after -%c option.\n", argv[optind], opt);  // error for invalid value after option
                    exit(1);
                }
                break;

            case 'l':
                if (isNumber(argv[optind]) && optind < argc) {  // check if optind is valid and is a number
                    fields->l_val = atoi(argv[optind]);         // saves option value to fields as int
                    // printf("%c -> %s\n", opt, argv[optind]);
                    fields->l_opt++;  // increments if value was given
                } else {
                    printf("Invalid parameter \"%s\" after -%c option.\n", argv[optind], opt);  // error for invalid value after option
                    exit(1);
                }
                break;
            case 's':
                if (isNumber(argv[optind]) && optind < argc) {  // check if optind is valid and is a number
                    fields->s_val = atoi(argv[optind]);         // saves option value to fields as int
                    // printf("%c -> %d\n", opt, fields->s_val);
                    fields->s_opt++;  // increments if value was given
                } else {
                    printf("Invalid parameter \"%s\" after -%c option.\n", argv[optind], opt);  // error for invalid value after option
                    exit(1);
                }
                break;
            case '?':
                printf("Possible options: -o, -y. -l, -s\n");
                exit(1);
        }
    }
}

////////////////
// int get_filematch_count(char *f_path, char *f_name, ls_fields fields)
////////////////
/*
Description:    increments the global variable "int filecount" if the current file meets that fields criteria

Parameters:     char *f_path: path of file including path as string (used for searching if not in cwd)
                char *f_name: name of file as string for printing
                ls_fields *fields: fields struct passed by value (used for matching file)
*/
void get_filematch_count(char *f_path, char *f_name, ls_fields fields) {
    struct stat statBuffer;
    if (stat(f_path, &statBuffer) == 0 && fits_opts(statBuffer, fields) && strncmp(f_name, ".", 1)) {  // checks of file exists, checks if it meets fields, checks if it is "." or ".." and excludes
        filecount++;                                                                                   // increment golbal variable if match found
    }
}

////////////////
// void print_filedata(char *f_path, char *f_name, ls_fields fields)
////////////////
/*
Description:    Prints file data for a file that matches fields. Calls a function that validates for each stat to be printed.

Parameters:     char *f_path: path of file including path as string (used for searching if not in cwd)
                char *f_name: name of file as string for printing
                ls_fields *fields: fields struct passed by value (used for matching file)
*/
void print_filedata(char *f_path, char *f_name, ls_fields fields) {
    struct stat statBuffer;
    if (stat(f_path, &statBuffer) == 0 && fits_opts(statBuffer, fields) && strncmp(f_name, ".", 1)) {  // checks of file exists, checks if it meets fields, checks if it is "." or ".." and excludes

        print_permissions(statBuffer);  // print permissions
        print_inode(statBuffer);        // print inode
        print_uid(statBuffer);          // print uid
        print_gid(statBuffer);          // print gid
        print_date(statBuffer);         // print modification date
        print_filesize(statBuffer);     // print filesize
        printf("%s ", f_name);          // print filename

        printf("\n");
    }
}

////////////////
// int fits_opts(struct stat statBuffer, ls_fields fields)
////////////////
/*
Description:    checks to see if a file meets the requirements given by search fields

Parameters:     struct stat statBuffer: struct containing data for a file
                ls_fields *fields: fields struct passed by value (used for matching file)
*/
int fits_opts(struct stat statBuffer, ls_fields fields) {
    double dif_t = (difftime(time(0), statBuffer.st_mtime)) / 86400;  // number of days since a file was modified. 86400=seconds in a day

    if (fields.o_opt) {  // if o opt was given

        if (dif_t < fields.o_val) {  // if mod date is older than value of o option, function returns 0
            return 0;
        }
    }
    if (fields.y_opt) {  // if y opt was given

        if (dif_t > fields.y_val) {  // if mod date is younger than value of y option, function returns 0
            return 0;
        }
    }
    if (fields.l_opt) {  // if l opt was given

        if ((int)statBuffer.st_size < fields.l_val) {  // if file size is less than value of l option, return 0
            return 0;
        }
    }
    if (fields.s_opt) {  // if s opt was given

        if ((int)statBuffer.st_size > fields.s_val) {  // if file size is greater than value of s option, return 0
            {
                return 0;
            }
        }
    }
    return 1;  // return 1 if all given options were satisfied by this file
}

////////////////
// int print_date(struct stat statBuffer)
////////////////
/*
Description:    prints the last modification date of given file

Parameters:     struct stat statBuffer: struct containing data for a file
*/
int print_date(struct stat statBuffer) {
    struct tm time;                             // used for time formatting
    time = *(localtime(&statBuffer.st_mtime));  // saves mod date as time object

    printf("   %d-%02d-%02d %02d:%02d:%02d ",  // formatting for date and time
           time.tm_year + 1900, time.tm_mon + 1, time.tm_mday,
           time.tm_hour, time.tm_min, time.tm_sec);
    return 0;
}

////////////////
// void print_permissions(struct stat statBuffer)
////////////////
/*
Description:    prints the user, group, other permissions of the file and whether the file is a directory
                if there is permission, r/w/x is printed. If not, "-" is printed.

Parameters:     struct stat statBuffer: struct containing data for a file
*/
void print_permissions(struct stat statBuffer) {
    printf(S_ISDIR(statBuffer.st_mode) ? "d" : "-");  // file is directory

    printf((statBuffer.st_mode & S_IRUSR) ? "r" : "-");  // user read
    printf((statBuffer.st_mode & S_IWUSR) ? "w" : "-");  // user write
    printf((statBuffer.st_mode & S_IXUSR) ? "x" : "-");  // user execute

    printf((statBuffer.st_mode & S_IRGRP) ? "r" : "-");  // group read
    printf((statBuffer.st_mode & S_IWGRP) ? "w" : "-");  // group write
    printf((statBuffer.st_mode & S_IXGRP) ? "x" : "-");  // group execute

    printf((statBuffer.st_mode & S_IROTH) ? "r" : "-");  // other read
    printf((statBuffer.st_mode & S_IWOTH) ? "w" : "-");  // other write
    printf((statBuffer.st_mode & S_IXOTH) ? "x" : "-");  // other execute

    printf(" ");  // spacing
}

////////////////
// int print_filesize(struct stat statBuffer)
////////////////
/*
Description:    prints the size of the file in bytes.

Parameters:     struct stat statBuffer: struct containing data for a file
*/
int print_filesize(struct stat statBuffer) {
    printf("%10d ", (int)statBuffer.st_size);  // print size of file as an int
    return (int)statBuffer.st_size;            // return size of file
}

////////////////
// int print_uid(struct stat statBuffer)
////////////////
/*
Description:    prints the uid of the file as a string.

Parameters:     struct stat statBuffer: struct containing data for a file
*/
int print_uid(struct stat statBuffer) {
    struct passwd *pwd;
    pwd = getpwuid(statBuffer.st_uid);  // gets user's name based on uid number
    printf("%18s ", pwd->pw_name);      // print uid as string
    return 0;                           // success
}

////////////////
// int print_gid(struct stat statBuffer)
////////////////
/*
Description:    prints the gid of the file as a string.

Parameters:     struct stat statBuffer: struct containing data for a file
*/
int print_gid(struct stat statBuffer) {
    struct group *grp;
    grp = getgrgid(statBuffer.st_gid);  // gets group name based on gid number
    printf("%15s ", grp->gr_name);      // print group name as a string
    return (int)statBuffer.st_gid;      // returns gid number
}

////////////////
// int print_inode(struct stat statBuffer)
////////////////
/*
Description:    prints the inode number of the file

Parameters:     struct stat statBuffer: struct containing data for a file
*/
int print_inode(struct stat statBuffer) {
    printf("%8d ", (int)statBuffer.st_ino);  // print inode number as int
    return (int)statBuffer.st_ino;           // return inode number
}

////////////////
// int isNumber(char *input)
////////////////
/*
Description:    checks to see if a given string consists of only digits and is
                safe for atoi.

Parameters:     char *input: string to be tested for digits
*/
int isNumber(char *input) {
    int length = strlen(input);  // length of char pointer
    int i;
    for (i = 0; i < length; i++) {  // iterate through each letter in string
        if (!isdigit(input[i])) {   // check if given char is a digit
            return 0;               // unsuccessful
        }
    }
    return 1;  // string is all digits
}