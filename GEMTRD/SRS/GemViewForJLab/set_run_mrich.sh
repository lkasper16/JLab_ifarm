#!/bin/bash

RUN=${1-1069}

cat db/Config_GEM_mRICH.templ |  sed s/RUNNUM/${RUN}/g  >  db/Config_GEM_TRD.cfg

./GemView

