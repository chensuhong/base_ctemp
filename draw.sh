#!/bin/bash

iTel="6"
month="3"
month=`printf %02d $month`
iTel=`printf %02d $iTel`
#ich="528"
for iday in `seq 23 30`
do
iday=`printf %02d $iday`
#for ich in `seq 0 1022`
#do
cd /scratchfs/ybj/chensuh/2020/EventShow 
./draw.exe ${month} ${iday} ${iTel} #${ich}
done
#done

