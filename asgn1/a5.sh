recycleBin="$HOME/my-deleted-files"
if [ -e "$recycleBin" ] ; then #Trash location exists
    if ! [ -d $recycleBin ] ; then #Trash is not a directory
        echo "$recycleBin exists and is already a file!"
        exit 1
    fi
else #Trash location does not exist
    mkdir "$recycleBin"
fi

usage() {
    local progname
    progname=$1
    echo "Usage: $progname <filename1> <filename2> ..."
    echo -e "\tSafely removes files"
    echo "Usage: $progname -c"
    echo -e "\tClears $recycleBin directory"
}

while getopts ":c" opt; do
    case $opt in
        "c")
            #Clear recycleBin
            echo -n "Are you sure you want to clear ${recycleBin}?[Y/N] "
            read answer
            answer=${answer##+( )} #Remove leading whitespace
            answer=${answer:0:1} #Get first char
            if [ $answer = 'Y' -o $answer = 'y' ] ; then
                for file in "$recycleBin"/* ; do
                    if ! [ -e "$file" ] ; do
                        continue
                    fi
                    rm -rf "$file"
                done
            fi
            exit #No other option considered
            ;;
        *)
            usage "$0"
            exit 1
            ;;
    esac
done
shift $(($OPTIND - 1))

if [ $# -lt 2 ] ; then #Missing filenames
    usage "$0"
    exit 1
fi

for file in $* ; do
    if ! [ -e "$file" ] ; then 
        echo "Cannot delete; $file does not exist; sorry"
        continue
    elif [ -d "$file" ] ; then
        echo "Cannot delete; $file is a directory; sorry"
        continue
    fi
    fileName=${file##*/} #Extract just the file name at the end
    if ! [ -e "$recycleBin/$fileName" -o -e "$recycleBin/$fileName.0" -o -e "$recycleBin/$fileName.1" ] ; then #we need no version number
        mv "$file" "$recycleBin/$fileName"
        continue
    elif ! [ -e "$recycleBin/$fileName.0" -o -e "$recycleBin/$fileName.1" ] ; then #version numbers 0 and 1 would suffice
        mv "$recycleBin/$fileName" "$recycleBin/$fileName.0"
        mv "$file" "$recycleBin/$fileName.1"
        continue
    fi
    versionNumber=2 #new version number supposedly
    while [ -e "$recycleBin/$fileName.$versionNumber" ] ; do
        versionNumber=$((versionNumber+1))
    done
    mv "$file" "$recycleBin/$fileName.$versionNumber"
done