/*=================================================================================
| reverseFileData.c
|
| By: Michael Salemi 0751614
| Date: January, 26, 2023
|
| Description:
|   This file contains functions for main function to execute. These functions
|   are only to be called in main function.
|
| Required Includes:
|   reverseFileData.h: (string.h, stdio.h, fcntl.h, unistd.h)
|
=================================================================================*/
#include "reverseFileData.h"

// searches for file in current working directory or at direct path
// creates an object holding the file number and name of file up to 500 chars
tFile openReadFile(char *file) {
    tFile rf; // file to be read is stored as object
    strcpy(rf.fileName, file); // saves file name in object
    if ((rf.fileNum = open(file, O_RDONLY)) >= 0) { // fileNum < 0 means file not found. file is opened in read only mode
        printf("%s opened...\n", file);
    } else {
        printf("ERROR: %s could not be opened.\n", file);
    }
    return rf; // tFile object returned with details of file to be read
}

// creates file with read/write permission for all but others
// file is created with the details of read file rf
tFile createWriteFile(tFile rf) {
    tFile wf = {}; // creates empty object
    wf = getNewFilename(rf, wf); // function call to get new file name from old filename
    wf.fileNum = open(wf.fileName, O_CREAT | O_RDWR, 00744);
    printf("\t%s reversed and saved as %s.\n", rf.fileName, wf.fileName);
    return wf; // new file created to hold reversed data from read file
}

// reverses the data from the read file and saves newly reversed data into the write file
tFile createReversedTextFile(tFile rf) {
    tFile wf; // write file storing reversed data
    int charCount/*total number of characters in read file*/, i/*iterator in for-loop*/;
    char buffer[1]; // string of size 1 to hold current char of read file to be written to write file. must be string because read/write functions require char pointer

    wf = createWriteFile(rf); // write file is created with details of read file

    charCount = lseek(rf.fileNum, 0, SEEK_END); // total number of chars in read file

    for (i = 0; i < charCount; i++) {
        lseek(rf.fileNum, -(i + 1), SEEK_END); // filepointer aims at lastchar - (i+1), with for-loop this reads all chars individually 1 at a time
        read(rf.fileNum, buffer, 1); // saves 1 character to buffer which is 1 byte long. Saves memory as opposed to reading entire file multiple times
        write(wf.fileNum, buffer, 1); // writes the 1 char in buffer to the new reversed file
    }

    return wf; // returns write file object data
}

// uses string manipulation to remove file extension and path to create new file name to be placed in current working directory
// appends file name with "Reversed" and file type
tFile getNewFilename(tFile rf, tFile wf) {
    int removeIndex;
    char *startAddress/*address of char after last "/" char*/, *tempAddress/*used to store address of last "/" char and then address of last "." char*/, fileNameAddon[20]="Reversed"/*appended to end of filename*/;

    tempAddress = (strrchr(rf.fileName, '/') + 1); // address of last "/" +1 = address of start of filename without path (/dir1/dir2/file1.txt becomes file1.txt)
    if (tempAddress >= rf.fileName && tempAddress <= &rf.fileName[strlen(rf.fileName) - 1]) { // checks to see if new address is within bounds of the string
        startAddress = tempAddress; //saves the new address of last "/" +1byte to be new starting address ofr filename
    } else {
        startAddress = rf.fileName; // if "/" not found in file name, uses original filename
    }

    tempAddress = strrchr(startAddress, '.'); // finds address of last "."
    if (tempAddress >= startAddress && tempAddress <= &startAddress[strlen(startAddress) - 1]) { // checks to see if last "." is within string
        removeIndex = tempAddress - startAddress; // subtracting these 2 addresses gives the number of bytes between the start of string and first "."
                                                  // since char is 1 byte, this is the index to be used for strncpy to remove file extension
                                                  // file1.txt becomes file1
        strcat(fileNameAddon, tempAddress); // saves the original file extension to the end of the string "Reversed" to be appended to new file name
    } else {
        removeIndex = strlen(rf.fileName); // if no file extension is present, uses original file name
    }

    strncpy(wf.fileName, startAddress, removeIndex); // saves the file name of read file to the writefile until the first "." char
    strcat(wf.fileName, fileNameAddon); // appends filename with "Reversed" and file extension
                                                    // file1.png becomes file1Reversed.png

    return wf; // returns write file object with new file name based on read file
}