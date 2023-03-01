#include <dirent.h>

#include "ls_helpers.h"

int main(int argc, char *argv[]) {
    char d_path[PATH_MAX];       // directory path
    char f_path[PATH_MAX];       // file path
    DIR *dir;                    // pointer to cwd
    struct dirent *currentfile;  // filepointer to a file in dir

    ls_fields fields;  // fields given as arguments

    set_opts(argc, argv, &fields);  // saves and validates arguments given as search fields

    if (argc % 2 == 0) {
        strcpy(d_path, argv[argc - 1]);  // if path given, save argument as directory path
    } else {
        getcwd(d_path, sizeof(d_path));  // if path not given, get cwd
    }

    dir = opendir(d_path);  // saves pointer to specified directory

    if (!dir)
    {
        printf("Directory \"%s\" does not exist.\n", d_path);
        return 1;
    }
    

    while ((currentfile = readdir(dir))) {  // counts how many files would be returned by specified fields
        if (argc % 2 == 0) {                // if path is given as argument, append each files name to given path
            sprintf(f_path, "%s/%s", d_path, currentfile->d_name);
        } else {  // if path is not given, use cwd
            strcpy(f_path, d_path);
        }
        get_filematch_count(f_path, currentfile->d_name, fields);  // increments filecount for each matching file
    }

    printf("filecount=%d\n\n", filecount);  // number of files in specified directory that match fields

    if (filecount) {  // executes if at least 1 file matches
        rewinddir(dir);
        printf("%10s %8s %18s %15s %13s %8s %10s %s\n",  // prints header for function
               "Permission", "inode", "UID", "GID", "Mod-Date", "Time", "Size", "Filename");
        while ((currentfile = readdir(dir))) {  // iterates through all files in dir
            if (argc % 2 == 0) {                // if path is given as argument, append each files name to given path
                sprintf(f_path, "%s/%s", d_path, currentfile->d_name);
            } else {  // if path is not given, append to cwd
                sprintf(f_path, "%s/%s", d_path, currentfile->d_name);
            }
            print_filedata(f_path, currentfile->d_name, fields);  // prints filedata for each matching file to console
        }

        printf("\n");
    }

    return 0;  // great success
}
