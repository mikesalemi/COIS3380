#! /bin/bash

###################
# Assignment Zip Tool
# Michael Salemi - 0751614
# January 12, 2022
# Creates zip file based on files given as arguments and places zipped file in home directory
# Files must be in current working dirtectory
# Usage: asszipper zipname file1 file2 file3...
#	 created zip will be created in user's home direcrory as $USER_zipname.zip
###################

if [ $# -eq 0 ] # execute if no arguments given to script
then
	echo "EXIT_ERROR: No Arguments given after command call." # descriptive error message
	exit 1 # exit with error code 1
fi

ZIPNAME=~/${USER}_$1 # uses username and first argument for zip file name to be placed in users home directory

if [ $# -eq 1 ] # executes if only 1 argument is given
then
	echo "EXIT_ERROR: Cannot create empty zip file named $ZIPNAME" # descriptive errore code
	echo "            Must specify file names after first argument" # descriptive errore code continued
	exit 1 # exit with error code 1
fi


for file in  "${@:2}" # iterates through all arguments after first given argument
do
	if [ $(ls -d $file 2> /dev/null | wc -l) -eq 0 ] # checks to see if each arguent is a file that exists
	then
		echo "EXIT_ERROR: $file was not found." # descriptive error code for file finding error
	 	exit 1 # exit with error code 1
	else
		echo "FOUND: $file" # confirmation message for finding file
	fi
done
FILELIST=("${@:2}") # adds all files given as arguments to an array
echo Files Included: ${FILELIST[@]} # confirmation prompt of files being zipped
echo Zip Location: $ZIPNAME # prompt for zip location and name
zip -r $ZIPNAME ${FILELIST[@]} # zips all files given and places zip in users home directory

exit 0 # successful exit of program
