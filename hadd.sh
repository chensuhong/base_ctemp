#!/bin/bash
#Date1=$1
#Date2=$2
iTel="6"
iTel=`printf %02d $iTel`
#iTel=$3
#tels="6"

for iday in `seq 1 30`
do
iday=`printf %02d $iday`
nextday=$((10#$iday+1))
nextday=`printf %02d $nextday`
year=2020
Date1=03$iday
Date2=03$nextday
echo ${Date1}
echo ${Date2}
#tels="1 6"
#for iTel in $tels
filedir=/scratchfs/ybj/chensuh/2020/EventShow/${year}

hadd ${filedir}/${Date1}.WFCTA${iTel}.root ${filedir}/$Date1/ES.*WFCTA${iTel}*2020${Date1}[12]*.root ${filedir}/$Date2/ES.*WFCTA${iTel}*2020${Date2}0*.root


done
