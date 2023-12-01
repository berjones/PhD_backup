#!/bin/bash

DATADIR="."

CURTIME=$(date +%Y%m%d_%H%M%S)

shopt -s extglob

# Target R8 to R15
DATAFILES+=" $DATADIR/R@(123|124|129|130)_+([0-9][0-9]|[0-9])" 
echo $DATAFILES > tmp.txt 

tr ' ' '\n' < tmp.txt > runFiles.txt
rm tmp.txt

# Run the event builder
parallel -j3 iss_sort -i {} < runFiles.txt -source

rm runFiles.txt

echo "data sorted" 


## Add the sub-runs together

hadd R123_subruns_added.root R123*.root
hadd R124_subruns_added.root R124*.root
hadd R129_subruns_added.root R129*.root
hadd R130_subruns_added.root R130*.root


echo "sub-runs added"
