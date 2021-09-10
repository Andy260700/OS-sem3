#!/bin/bash
read -p 'enter file path : ' path
read -p 'enter word to search : ' word
occ=`grep -oiw $word $path | wc -l`
if [ $occ -eq 0 ]
then 
    echo "word not present in file"
else
    echo "word occurs $occ times in file"
    grep -iwn $word $path | grep -oE "^[0-9]+"
fi