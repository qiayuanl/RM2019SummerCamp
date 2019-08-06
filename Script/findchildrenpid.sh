#!/bin/bash

[ "$#" -eq 1 -a -d "/proc/$1/task" ] || exit 1

PID_LIST=
findpids() {
        for pid in /proc/$1/task/* ; do
                pid="$(basename "$pid")"
                PID_LIST="$PID_LIST$pid "
                for cpid in $(cat /proc/$1/task/$pid/children) ; do
                        findpids $cpid
                done
        done
}

findpids $1
echo $PID_LIST