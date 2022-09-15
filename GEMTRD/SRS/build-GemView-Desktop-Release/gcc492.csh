setenv MY_GXX_HOME /apps/gcc/4.9.2
setenv MY_PYT_HOME /apps/python/python-2.7.1

setenv PATH "${MY_GXX_HOME}/bin:${PATH}"

if ( $?LD_LIBRARY_PATH ) then
    echo "-- Attention: append  LD_LIBRARY_PATH=$LD_LIBRARY_PATH"
    setenv LD_LIBRARY_PATH "${MY_GXX_HOME}/lib:${MY_GXX_HOME}/lib64:${LD_LIBRARY_PATH}"
else 
    setenv LD_LIBRARY_PATH "${MY_GXX_HOME}/lib:${MY_GXX_HOME}/lib64"
endif

setenv LD_RUN_PATH "${MY_GXX_HOME}/lib:${MY_GXX_HOME}/lib64:${LD_LIBRARY_PATH}"


