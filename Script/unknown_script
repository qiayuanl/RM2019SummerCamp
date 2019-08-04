#!/bin/bash
find -iname "*.[ch]" -ctime $1 \
    -o -iname "*.[ch]pp" -ctime $1 \
    | xargs wc -l 2> /dev/null 

