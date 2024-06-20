#!/bin/bash

if ! echo $1 | grep -qE ".*/$" || [[ "$#" != 1 ]]
  then
    echo "bad args"
    exit 1
fi
if ! [[ -d $1 ]]; then 
    echo "not a dir"
    exit 1
fi

scriptStart=$(date +%s.%N)

LINES_NUM=$(($(du $1 | wc -l) - 1))
echo "Total number of folders (including all nested ones) = $LINES_NUM"

echo "TOP 5 folders of maximum size arranged in descending order (path and size):"
LINES_NUM=$(du -h -d 1 $1 | sort -hr | head -n 6 | wc -l)
du -h -d 1 $1 | sort -hr | head -n 6 | tail -n $(($LINES_NUM - 1)) | \
awk '{print NR,"-",$2",",$1}'

LINES_NUM=$(find $1 -type f | wc -l)
echo "Total number of files = $LINES_NUM"

echo "Number of:" 

LINES_NUM=$(find $1 -name *.conf -type f | wc -l)
echo "Configuration files (with the .conf extension) = $LINES_NUM"

LINES_NUM=$(find $1 -name *.txt -type f | wc -l)
echo "Text files = $LINES_NUM"

LINES_NUM=$(find $1 -executable -type f | wc -l)
echo "Executable files = $LINES_NUM"

LINES_NUM=$(find $1 -name *.log -type f | wc -l)
echo "Log files (with the extension .log) = $LINES_NUM"

LINES_NUM=$(find $1 -type f -name "*.tar" -o -name "*.zip" -o -name "*.gz" -o -name "*.tar"\
 -o -name "*.gz" -o -name "*.tar" -o -name "*.bz2" | wc -l)
echo "Archive files = $LINES_NUM"  

LINES_NUM=$(find $1 -type l | wc -l)
echo "Symbolic links = $LINES_NUM"  

OUTPUT_EXT () {
    while read in
    do
        ext=$(echo "$in" | grep -P -o '(?<=\.)(.*?)(?=,)')
        echo "$in $ext"
    done
}
echo "TOP 10 files of maximum size arranged in descending order (path, size and type):"
find $1 -type f -exec du -h {} + | sort -n -r | head -n 10 |\
awk '{print NR,"-",$2",",$1","}' | OUTPUT_EXT

OUTPUT_MD () {
    while read in
    do
        path=$(echo "$in" | awk '{print substr($3, 1, length($3)-1)}')
        md5=$(md5sum "$path" | awk '{print $1}') 
        echo "$in $md5"
    done
}
echo "TOP 10 executable files of the maximum size arranged in descending order\
 (path, size and MD5 hash of file)"
find $1 -type f -executable -exec du -h {} + | sort -n -r | head -n 10 |\
awk '{print NR,"-",$2",",$1","}' | OUTPUT_MD

scriptEnd=$(date +%s.%N) 
scriptExeTime=$(echo "$scriptEnd - $scriptStart" | bc -l | tr . ,)
printf "Script execution time (in seconds) = %.5f\n" $scriptExeTime