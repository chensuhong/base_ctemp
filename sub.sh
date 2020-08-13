#!/bin/bash
#filedir='pwd'

for iday in `seq 3 31`
do

#iday=$1
iday=`printf %02d $iday`
year=2020
Date=03$iday

filedir=/scratchfs/ybj/chensuh/2020/EventShow/${year}/${Date}
for i in $filedir/*.sh
do
	echo $i
	hep_sub -g lhaaso $i
done
done




