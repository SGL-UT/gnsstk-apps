#!/bin/bash

#
# Checks for duplicate header files in the source tree.
# Lists any duplicates found, and returns a non-zero exit code.
#

find -type f -name "*.hpp" | sed -e "s#.*\/##g" | sort > names_all.txt
find -type f -name "*.hpp" | sed -e "s#.*\/##g" | sort --unique > names_unique.txt
comm -23 names_all.txt names_unique.txt > names_duplicate.txt
if (( $(cat names_duplicate.txt | wc -l) != 0 )); then
    echo "FAIL: duplicate header file names (*.hpp) detected in source tree: ";
    cat names_duplicate.txt;
    exit 1; 
fi;

rm names_all.txt names_unique.txt names_duplicate.txt
echo "PASS: No duplicate header file names (*.hpp) detected in source tree."
exit 0
