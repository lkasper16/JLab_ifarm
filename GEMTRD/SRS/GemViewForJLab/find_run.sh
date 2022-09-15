#!/bin/bash

RUNNUM=${1-000000} 
FILNUM=${2-000} 

FN=$(printf '%03d' $FILNUM)

#find /gluonraid3/data?/rawdata/volatile/RunPeriod-201[8,9]-0?/rawdata/  -name hd_rawdata_0${RUNNUM}_000.evio

FILE=`find /gluonraid5/data?/rawdata/volatile/RunPeriod-2019-11/rawdata/  -name hd_rawdata_0${RUNNUM}_${FN}.evio`

if [[ x$FILE = "x" ]] ; then
FILE=`find /gluonraid5/data?/rawdata/active/RunPeriod-2019-11/rawdata/  -name hd_rawdata_0${RUNNUM}_${FN}.evio`
fi

echo $FILE
