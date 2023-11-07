#!/bin/bash
DATADIR="../my_data/"
CONFIGDIR="../sort-files"
SETTINGSFILE="settings_22Ne_20210910.dat"
AUTOCALFILE="autocal.dat"
HISTDIR="histfiles"

REACTIONDIR="$1"
REACTIONFILE="$2"

CURTIME=$(date +%Y%m%d_%H%M%S)

shopt -s extglob

# Target R8 to R15
DATAFILES_R16_to_R23+=" $DATADIR/R@(16|17|18|19|21|22|23)_+([0-9][0-9]|[0-9])" 


## Histogram the grouped runs

iss_sort -autocal -autocalfile $CONFIGDIR/$AUTOCALFILE -s $CONFIGDIR/$SETTINGSFILE -r $REACTIONDIR/$REACTIONFILE -i $DATAFILES_R16_to_R23 

echo "Autocal complete"

sleep 1