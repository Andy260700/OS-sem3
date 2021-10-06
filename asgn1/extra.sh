#!/bin/bash
while :;
do
    echo "case 1 : find user"
    echo "case 2 : list large files"
    echo "case 3 : disk usage"
    echo "case 4 : view log file"
    echo "case 5 : exit"

    echo "enter choice"
    read num
    case $num in
        1) read -p "enter username" name
        user=`cut -d: -f1 /etc/passwd | grep $name`
        echo "$user"
        if [ $user = "" ];
        then
        echo "invalid username"
        else 
        echo "valid username"
        fi
        echo `whoami`"%find user%"`date` >> ./logfile.txt
        ;;
        2) read -p "enter size in bytes" size
        echo `find . -type f -size +$size\c` 
        echo `whoami` "%list large files%" `date` >> ./logfile.txt
        ;;
        3) du
        echo `whoami` "%disk usage%" `date` >> ./logfile.txt
        ;;
        4) cat logfile.txt
        echo `whoami` "%view log file%" `date` >> ./logfile.txt
        ;;
        5) exit
        ;;
        *) echo "wrong choice"
        ;;
    esac
done

