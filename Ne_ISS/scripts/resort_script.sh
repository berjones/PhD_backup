#!/bin/bash

DATADIR="/Volumes/ad/work/iss/iss_data/9294Kr_IS711/data/92Kr"
CONFIGDIR="/Volumes/ad/work/iss/iss_data/9294Kr_IS711/sortfiles"
SETTINGSFILE="settings_Kr_20221024.dat"
CALIBFILE179="calibration_20221117.cal"
REACTIONFILE109="reaction_92Kr_20221026_109.dat"
REACTIONFILE135="reaction_92Kr_20221026_135.dat"
REACTIONFILE137="reaction_92Kr_20221026_137.dat"
REACTIONFILE179="reaction_92Kr_20221026_179.dat"

CURTIME=$(date +%Y%m%d_%H%M%S)

shopt -s extglob

# Target 109 runs
DATAFILES109+=" $DATADIR/R@(37|38|39|40|41|44|45|46|47|49|50|51|52|53|55|56|57|58|59|60|61|62|63|64|65|66|67|69|70|71|72|73|74|75|76|77|78|79|80)_+([0-12])"

# Target 135 runs
DATAFILES135+=" $DATADIR/R@(81|82|83|85|86|87)_+([0-5])"

## Target 137 runs
DATAFILES137+=" $DATADIR/R@(129|130|131|132|133|134|135|136|137|138|139|141|142)_+([0-15])"

## Target 179 runs
DATAFILES179+=" $DATADIR/R@(24|25|26|27|28|29|30|31|32|33|34|35|36)_+([0-9])"

# trigger validation on for 92Kr runs
DATAFILESTRIGVAL92+=" $DATADIR/R@(129|131|132|133|137)_+([0-9])"

#recoil finder test
DATAFILE+=" $DATADIR/R137_0"

## Histogram them all together

~/Documents/physics/ISSSort/bin/iss_sort -s $CONFIGDIR/$SETTINGSFILE -c $CONFIGDIR/$CALIBFILE -r $CONFIGDIR/$REACTIONFILE109 -i $DATAFILES109 -o $DATADIR/92Kr_109_sum_hists_${CURTIME}.root -e

~/Documents/physics/ISSSort/bin/iss_sort -s $CONFIGDIR/$SETTINGSFILE -c $CONFIGDIR/$CALIBFILE -r $CONFIGDIR/$REACTIONFILE135 -i $DATAFILES135 -o $DATADIR/92Kr_135_sum_hists_${CURTIME}.root -e

~/Documents/physics/ISSSort/bin/iss_sort -s $CONFIGDIR/$SETTINGSFILE -c $CONFIGDIR/$CALIBFILE -r $CONFIGDIR/$REACTIONFILE135 -i $DATAFILES137 -o $DATADIR/92Kr_137_sum_hists_${CURTIME}.root -e

~/Documents/physics/ISSSort/bin/iss_sort -s $CONFIGDIR/$SETTINGSFILE -c $CONFIGDIR/$CALIBFILE -r $CONFIGDIR/$REACTIONFILE179 -i $DATAFILES179 -o $DATADIR/92Kr_179_sum_hists_${CURTIME}.root -e

hadd $DATADIR/92Kr_sumhists_alltargets${CURTIME}_annierecoil.root $DATADIR/92Kr_*_sum_hists_${CURTIME}.root

root -l $DATADIR/92Kr_sumhists_alltargets${CURTIME}_annierecoil.root

#trigger val on 92Kr runs
#~/Documents/physics/ISSSort/bin/iss_sort -s $CONFIGDIR/$SETTINGSFILE -c $CONFIGDIR/$CALIBFILE -r $CONFIGDIR/$REACTIONFILE137 -i $DATAFILESTRIGVAL92 -o $DATADIR/92Kr_trigval-ON_sum_hists_${CURTIME}.root -e

#recoil finder tests

#~/Documents/physics/ISSSort/bin/iss_sort -s $CONFIGDIR/$SETTINGSFILE -c $CONFIGDIR/$CALIBFILE -r $CONFIGDIR/$REACTIONFILE137 -i $DATAFILESTRIGVAL92 -o $DATADIR/recoilfinder/92Kr_trigval-ON_sum_hists_${CURTIME}.root -e

#~/Documents/physics/ISSSort/bin/iss_sort -s $CONFIGDIR/$SETTINGSFILE -c $CONFIGDIR/$CALIBFILE -r $CONFIGDIR/$REACTIONFILE137 -i $DATAFILE -o $DATADIR/recoilfinder/recoilfindertest_${CURTIME}.root -e

#~/Documents/physics/ISSSort/bin/iss_sort -s $CONFIGDIR/$SETTINGSFILE -c $CONFIGDIR/$CALIBFILE -r $CONFIGDIR/$REACTIONFILE109 -i $DATAFILE -o $DATADIR/recoilfinder/recoilfindertest_R137_0_annietest_${CURTIME}.root

#cp $DATADIR/R137_0_events.root $DATADIR/recoilfinder/R137_0_events_${CURTIME}.root
#cp $DATADIR/R137_0_events.log $DATADIR/recoilfinder/R137_0_events_${CURTIME}.log

#root -l $DATADIR/recoilfinder/R137_0_events_${CURTIME}.root
#root -l $DATADIR/recoilfinder/recoilfindertest_R137_0_annietest_${CURTIME}.root
#root -l $DATADIR/recoilfinder/recoilfindertest_R137_0_${CURTIME}.root

