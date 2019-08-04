#!/bin/bash
find -iname "*.[ch]"  \
    -o -iname "*.[ch]pp" \
    | xargs wc -l 2> /dev/null \
    | tail -n1

