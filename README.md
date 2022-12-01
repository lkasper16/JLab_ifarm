# JLab_ifarm

## Repository for accessing/analyzing prototype raw data via JLab compute nodes

Raw data linked in `/DATA` directory.  
To write data from tape to cache:  
(From home directory)  
```
jcache get /mss/halld/TRD/BeamTest_2021/hd_rawdata_002038_*.evio  
ls  /cache/halld/TRD/BeamTest_2021/hd_rawdata_002038_*.evio  
cd  /cache/halld/TRD/BeamTest_2021/  
ln -s /cache/halld/TRD/BeamTest_2021/hd_rawdata_002038_*.evio ~/GEMTRD/DATA  
```
Do this process for all runs with the MMG prototype, [listed in this sheet](https://docs.google.com/spreadsheets/d/1ST8_Ad5RuxpX7Dcehvn0Dpna4XBqGHkMB1DQ_2oZaDo/edit#gid=0)  
Check status of cache writing with  
```
jcache pendingRequest -u $USERNAME
```  
[jcache documentation](https://scicomp.jlab.org/docs/node/586)

Afterwards, make Analyzer file executable and execute over desired data run number:  
```
cd GEMTRD/SRS/  
source env_ifarm.csh  
make DAQCrateAnalyzer  
./DAQCrateAnalyzer 2038  
```
... And view info in the root tree that comes out
```
root -l hd_rawdata_002038.evio.disproot  
```  
See also: [JLab SLURM batch job documentation](https://scicomp.jlab.org/docs/farm_slurm_batch)
