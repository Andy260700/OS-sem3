#!/bin/bash
i=1
for i in {1..10}
do 
echo "$1 x $i = $(($i*$1))"
done