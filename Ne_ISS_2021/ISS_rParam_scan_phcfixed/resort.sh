#!/bin/bash

DATADIR="../my_data/"
CONFIGDIR="../sort-files"
SETTINGSFILE="settings_22Ne_20210910.dat"
HISTDIR="histfiles"

REACTIONDIR="$1"
REACTIONFILE="$2"
CALIBFILE="$3"


CURTIME=$(date +%Y%m%d_%H%M%S)

shopt -s extglob

# Target R8 to R15
DATAFILES_R8_to_R15+=" $DATADIR/R@(8|9|10|11|12|13|15)_+([0-9][0-9]|[0-9])" 


## Histogram the grouped runs

iss_sort -s $CONFIGDIR/$SETTINGSFILE -c $REACTIONDIR/$CALIBFILE -r $REACTIONDIR/$REACTIONFILE -i $DATAFILES_R8_to_R15 -o $HISTDIR/$REACTIONFILE.root -e

echo "Hist file produced"

sleep 1