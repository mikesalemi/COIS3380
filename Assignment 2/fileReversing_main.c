/*=================================================================================
| File Reverser
|
| fileReversing_main.c
|
| By: Michael Salemi 0751614
|
| Description:
|    This file houses the main function and performs program functions for each
|    command line argument. Error handling in function depending on scope of error.
|    Regardless of source file destination, reversed and re-reversed files will be
|    placed in working directory. Program works to reverse any file type, including
|    txt, png, jpg, pdf...
|
| Compilation: gcc -o revarsalTool fileReversing_main.c reverseFileData.c
|
| Program Call: reversalTool file1 file2 file3
|
| Required Includes:
|   reverseFileData.h
|
| Errors:
|    If a file argument is not found, error code will display but program will
|    continue with other arguments.
|    If no arguments are given, program will terminate.
=================================================================================*/

#include "reverseFileData.h"

int main(int argc, char *argv[]) {
    tFile fileRead /*file to be read for reversing*/, reversedFile /*reversed version of fileRead*/;

    int i; // iterator in for-loop
    if (argc > 1) {
        // executes program for each file given as argument
        for (i = 1; i < argc; ++i) {
            fileRead = openReadFile(argv[i]); // sets fileRead.fileNum < 0 if not found
            if (fileRead.fileNum > 0) {
                reversedFile = createReversedTextFile(fileRead); // reverses original file
                createReversedTextFile(reversedFile); // reverses reversed file, should match original file
            }
        }
    } else {
        // executes if no arguments given
        printf("EXIT_ERROR: No command line arguments given to program call.\n");
    }

    return 0;
}
