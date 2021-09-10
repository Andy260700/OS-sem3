#!/bin/bash
read -p 'enter file path : ' path
read -p 'enter word to search : ' word
read -p 'enter pattern to replace : ' pattern

# sed -i "s/$word/$pattern/g" $path
sed -i '' -e "s/$word/$pattern/g" $path