/*=================================================================================
| reverseFileData.h
|
| By: Michael Salemi 0751614
| Date: January, 26, 2023
|
| Description:
|   This file contains function declarations for reverseFileData.c function to
|   execute. These functions are only to be called in main function. File also
|   contains struct for tFile object.
|
| Required Includes:
|   string.h, stdio.h, fcntl.h, unistd.h
|
=================================================================================*/

#ifndef MAIN_C_REVERSETEXTFILE_H
#define MAIN_C_REVERSETEXTFILE_H

#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>

// tFile.Num contains file pointer numbers attained by write and open functions
// tFile.fileName contains the name of it's associated file
typedef struct {
    int fileNum;
    char fileName[550];
} tFile;

// opens read file and creates tFile object to store file number and file name
// takes in argv index as parameter
tFile openReadFile(char *file);

// creates a file to hold reversed file data based on parameter file
// rf = read file
tFile createWriteFile(tFile rf);

// stores the file data of rf into a new tFile object in the reverse order of rf
// returns the new tFile object
tFile createReversedTextFile(tFile rf);

// creates a file name for wf based on rf
// new file name is rf stripped of file path with "Reversed" appended to it
tFile getNewFilename(tFile rf, tFile wf);

#endif //MAIN_C_REVERSETEXTFILE_H
