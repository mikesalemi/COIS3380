#! /bin/bash

######################
# System Log Checker
# Michael Salemi - 0751614
# January 13, 2022
# Scans system logs for possible signs of hacing and creates log files for:
#       1) all log files that are assumed hacking attempts (404 Errors)
#       2) the top 10 requested invalid pages and
# Usage: syslogscan.sh file1 file2 file3... <-- scans specific file or list of files
#        syslogscan.sh <-- scans entire log directory (/home/COIS/3380/lab1)
######################

# change these values to adjust default search directory when no arguments given or when searching for different patterns
DEFAULT_DIR="/home/COIS/3380/lab1/system_logs*"
PATTERN=" 404 "

# output file names
LOG_FILENAME="all_404_results.txt"
TOP_INVALID_PAGES="top_ten_404_pages.txt"
TOP_IP_ADDRESSES="top_10_IPs.txt"

# do NOT adjust these values
TOTAL_LINES=0 # number of lines scanned
FOUND_MATCHES=0 # number of lines that match the 404 error pattern above
TEMP="temp.txt" # temporary file
touch "$TEMP" # temporary file created for saving found matches. Needed in order to append to proper file later.

# if no argument is give, program scans entire default directory listed above
# otherwise, program scans specific file(s) passed as arguments
if [ $# -eq 0 ]
then
    echo "Scanning directory..."
    for file in $DEFAULT_DIR # scans all files in default directory
    do
        TOTAL_LINES=$((TOTAL_LINES+`wc -l < $file`)) # total lines in all files
        FOUND_MATCHES=$((FOUND_MATCHES+`grep -c $PATTERN $file`)) # total lines with 404 error
    done
# following executes if there were 1 or more arguments given
else    
    for file in "$@" # scans all files passed as arguments
    do
        if [  $(ls -d $file 2> /dev/null | wc -l) -eq 0 ] # checks to see if file exists and redirects stderr to void
        then
            echo "EXIT_ERROR: $file not found." # custom descriptive error message
            exit 1 # exit with error code 1
        else # file was found
            echo "Scanning $file..." # basic user prompt
            TOTAL_LINES=$((TOTAL_LINES+`wc -l < $file`)) # total lines in all files
            FOUND_MATCHES=$((FOUND_MATCHES+`grep -c $PATTERN $file`)) # total lines with 404 error
        fi
    done
fi

echo "      Complete" # user prompt for completed file validation

echo -n "Found 404 Matches: " > $LOG_FILENAME # writes to and creates file for all found unique matches
echo "$FOUND_MATCHES / $TOTAL_LINES" >> $LOG_FILENAME # displays number of 404 hits over total hits
echo "" >> $LOG_FILENAME

if [ $# -eq 0 ] # executes if no arguments given
then
    echo Saving pattern matches... # user prompt for extracting invalid pages
    for file in $DEFAULT_DIR # iterates through all files in default directory
    do    
        grep -r $PATTERN $file >> $TEMP # saves all found 404s
    done
else
    echo Saving pattern matches... # user prompt for extracting invalid pages
    for file in "$@" # iterates through all files given as arguments
    do
        grep $PATTERN $file >> $TEMP # saves all found 404s
    done
fi

echo "      Complete" # user prompt for completion of 404 scan

###################
# Second file
# scans through scan results and finds the 10 most commin ip addresses as well as their counts
###################
echo Finding top 10 IP addresses... # user prompt for starting IP address scan
awk '{ print $1 }' $TEMP | sort | uniq -c | sort -nro $TOP_IP_ADDRESSES # all unique ip addresses and aorts them based on number of occurrences
# sed -i '11,$d' $TOP_IP_ADDRESSES # removes all ip addresses not in top 10 of frequency

echo "      Complete" # user prompt for completion of IPs scan


###################
# Third File
# looks through the results of first scan and finds most common invalid pages
# writes the 10 most common invalid pages to new file
###################

echo Finding top 10 invalid pages...
grep -o 'GET.*\|POST.*' $TEMP | echo "$(awk '{print $1,$2}')" | sort | uniq -c | sort -nro $TEMP # finds all matches along with unique counts and then sorts
cat $TEMP >> $LOG_FILENAME # appends temp file containing unique matches to "all_404_results.txt"
sed -n '3,12 p' $LOG_FILENAME > $TOP_INVALID_PAGES # saves top 10 unique invalid pages
                                                   # (first line is skipped because it includes total count)


rm $TEMP # removes temp file
echo "      Complete" # prompt for completion of pattern matching
echo "Program complete. Files written:" # prompt for completion of program
echo "      $TOP_INVALID_PAGES" # displays name of first file for all unique matches
echo "      $LOG_FILENAME" # displays name of second file for top 10 invalid pages
echo "      $TOP_IP_ADDRESSES" # displays name of third file for top 10 IP addresses requesting invalid pages

exit 0
