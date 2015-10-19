#!/bin/bash

function format-comp40 {
    astyle --style=linux --indent=spaces=4 --convert-tabs --max-code-length=80\
       --mode=c --break-closing-brackets --add-brackets\
       --align-pointer=name --align-reference=name --indent-switches\
       --indent-cases --break-blocks --pad-oper\
       --lineend=linux --dry-run --unpad-paren --pad-header $1

    # --delete-empty-lines
    # --add-one-line-brackets
}

if [ -z $2 ]; then
    format-comp40 < $1
else
    format-comp40 < $1 > $2
fi
