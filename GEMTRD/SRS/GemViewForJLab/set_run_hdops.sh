#!/bin/bash

RUN=${1-1069}
FIL=${2-0}

FN=$(printf '%03d' $FIL) 

if [[ $RUN = "cur" ]] ; then
    evio_file=`find /media/ramdisk/active/rawdata/active/RunPeriod-2019-11/rawdata/ -name hd_rawdata_\*_${FN}.evio | tail -1`
    RUN=`echo $evio_file | cut -f9 -d/ | cut -f2 -dn | cut -c2- `
else
    evio_file=`./find_run.sh $RUN $FIL `
fi

[[ x$evio_file = x ]] && echo " No file " && exit 1 

echo "RUN=$RUN file= `ls -h $evio_file`"


cat db/Config_GEM_TRD_hdops.templ |  sed s\#EVIO_FILE\#${evio_file}\#g  |  sed s/RUNNUM/${RUN}/g  >  db/Config_GEM_TRD.cfg

./GemView

