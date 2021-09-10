#!/bin/bash
read -p 'enter date 1: ' d1
read -p 'enter date 2: ' d2

a=`date -j -f '%d/%m/%Y' "$d1" +'%A'`
b=`date -j -f '%d/%m/%Y' "$d2" +'%A'`

if [ "$a" = "$b" ]
then
    echo 'found a match'
else
    echo 'better luck next time'
fi