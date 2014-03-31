#!/bin/bash -e

if [ "$1" == "" ]; then
  echo "Usage: ./print-label-range label.txt"
  exit -1
fi

LABEL=$1
LABEL_RANGE=$(cut -f 2- -d ' ' $LABEL | tr ' ' '\n' | sed '/^\s*$/d' | sort -n -u | sed -n '1p;$p')

FROM=$(echo $LABEL_RANGE | cut -f 1 -d ' ')
TO=$(echo $LABEL_RANGE | cut -f 2 -d ' ')
printf "label range: \33[34m$FROM ~ $TO\33[0m\n"
