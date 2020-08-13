#!/bin/bash

for iday in `seq 3 31`
do
#iday=$1
iTel="6"
iday=`printf %02d $iday`
iTel=`printf %02d $iTel`
year=2020
Date=03$iday
#Date=0305

jobdir=/scratchfs/ybj/chensuh/2020/EventShow/${year}/${Date}
rootdir=/eos/lhaaso/decode/wfcta

mkdir -p ${jobdir}

for ifile in ${rootdir}/$year/$Date/*.WFCTA${iTel}.*.event.root
do
	ifile=`basename ${ifile}`
	echo ${jobdir}/${ifile}
	echo '#!/bin/bash' >${jobdir}/${ifile}.sh
	echo 'source /cvmfs/lhaaso.ihep.ac.cn/anysw/slc5_ia64_gcc73/external/envb.sh' >>${jobdir}/${ifile}.sh
	echo "" >>${jobdir}/${ifile}.sh
	echo "year=$year" >>${jobdir}/${ifile}.sh
	echo "Date=$Date" >>${jobdir}/${ifile}.sh
	echo "rootdir=${rootdir}/$year/$Date" >>${jobdir}/${ifile}.sh
	echo "jobdir=${jobdir}" >>${jobdir}/${ifile}.sh
	echo "" >>${jobdir}/${ifile}.sh
	echo "ifile=${ifile}" >>${jobdir}/${ifile}.sh
	
	echo 'cd /scratchfs/ybj/chensuh/2020/EventShow' >>${jobdir}/${ifile}.sh
	
	echo './drawCimage.exe $rootdir/${ifile} $jobdir/${ifile}.base.root' >>${jobdir}/${ifile}.sh
	chmod 755 ${jobdir}/${ifile}.sh
done
done
