#!/bin/bash
# Write a shell script that accepts a file name as an input and performs the following activities
# on the given file. 
# The program asks for a string of characters (that is, any word)
#  to be provided by the user. The file will be searched to find whether 
# it contains the given word. If the file contains the given word, 
# the program will display (a) the number of occurrences of the word. 
# The program is also required to display (b) the line number in 
# which the word has occurred. 
# (Note: the word may occur more than once in a given line) 
# If the file does not contain the word, an appropriate error message will be displayed.
read -p 'Enter file path: ' path
read -p 'Enter word to search: ' word
occ=`grep -oiw $word $path | wc -l | xargs` # xargs is used removing leading spaces
if [ $occ -eq 0 ]
then 
    echo "Word not present in file"
else
    echo "Word occurs $occ times in file"
    grep -iwn $word $path | grep -oE "^[0-9]+"
fi