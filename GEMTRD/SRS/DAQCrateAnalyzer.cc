/* include files */
#include <iostream>
#include <sstream>   
#include <iomanip>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <string>
#include <evioUtil.hxx>
#include <unistd.h>
#include <vector>
#include "evioFileChannel.hxx"

using namespace std;
using namespace evio;

// root include files
#include "TApplication.h"  // needed to display canvas
#include "TSystem.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TTree.h"
#include "TH1.h"
#include "TH1F.h" 
#include "TH2F.h"
#include "TH3F.h"
#include "TF1.h"
#include "TF2.h"
#include "TMath.h"
#include "TGraph.h"
#include "TMultiGraph.h"
#include "TGraphErrors.h"
#include "TPaveStats.h"
#include "TStyle.h"

#define PI 3.14159265
#define ROLLOVER_COUNT       65460    // get this when configure TDC
#define TDC_LOOKBACK         15794    // get this when configure TDC 
#define TDC_v2BIN_SIZE       0.0580   // ns/LSB
#define TDC_v3BIN_SIZE       0.1160   // ns/LSB

float tref;
float ave_time;
float sang=0.*PI/180.; // strip angle w.r.t. wire

int print_flg=0;

int DODisplay = 0;
int VERBOSE=0;
int DOVis = 0;
int ROCSlots[20];
int WindowSize;
int PSWindowSize;
int PSCWindowSize;

float TDCData[4][20][48][200];
int TDChits[4][20][48];
float TDCDataST[1][20][32][200];
int TDChitsST[1][20][32];
int SThits;

float ADCSamples[1][20][72][1000]; //FDC
float ADCPedestal[1][20][72]; // pedestal calculated offline
int slot_special=0;
int ch_special=0;
int samp_special=0;

float PSADCSamples[20][16][2000]; //PS
float PSADCPedestal[20][16]; // pedestal calculated offline
float PSCADCSamples[16][2000]; //PSC
float PSCADCPedestal[16]; // pedestal calculated offline


float fADCPedestal[20][20][72]; //pedestal calculated by the firmware
float fADCpeak[15][15][72][200]; //same as in fADCdata class, but calculated by the firmware
float fADCcharge[15][15][72][200]; //same as in fADCdata class, but calculated by the firmware
int fADCnhit[15][15][72]; //same as in fADCdata class, but calculated by the firmware
float fADCtime[15][15][72][200]; //same as in fADCdata class, but calculated by the firmware
long int trig_trd[10];
int trd_cnt;
long int trig_st[10];
int st_cnt;
long int trig_ps[10];
int ps_cnt;
int evntno_st=0;
int evntno_trd=0;
int evntno_ps=0;
int tgpoint=0;

void analyzeEvent(evioDOMTree &eventTree);
void analyzeBank(evioDOMNodeP bankPtr);
void CountTriggers();

float fADC[20][72][2000];
float Ped[20][72];
TFile* ROOTfile;
TTree* fdcFeTree;

// trigger stuff
int NEventsInBlock;
uint64_t EVENTNUMBER;
uint64_t EVTCounts;
long TheEventNum;
uint64_t TRIGGERTIMES;
uint32_t TRIGGERTYPES;
uint32_t TRIGGER_MASK_GT;
uint32_t TRIGGER_MASK_FP;
int TheRunNumber;

ofstream to_sergey_rad;
ofstream to_sergey_norad;
ofstream to_sergey_rad2;
ofstream to_sergey_norad2;

#define first_slot 0
#define wire_slot 2
#define wire_x_ch0 24
#define wire_y_ch0 24

// GEM-TRD first/ch dsefinition
#define gem_x_slot 0
#define gem_x_ch0 0

// MMG first slot/ch definition
#define gem_y_slot 3
#define gem_y_ch0 24

// pad gem first slot/ch definition
#define gem_p_slot 6
#define gem_p_ch0  0 
//cokcok

// GEM-TRD mapping - it works the way it is
int GetXSlot(int gch){
	return gem_x_slot+(int)(gch+gem_x_ch0)/72+first_slot;
}

int GetXChan(int gch){
	int fch=(gch+gem_x_ch0)%72;
	int card=fch/24;
	int cch=fch%24; 

    	//cokcok for DIRC
    	return 23-cch+card*24;

    	//cokcok for PS arm
    	//return cch+card*24;
}

// MMG mapping !!!!
int GetYSlot(int gch){
	return gem_y_slot+(int)(gch+gem_y_ch0)/72+first_slot;
}
int GetYChan(int gch){
	int fch=(gch+gem_y_ch0)%72;
  	int card=fch/24;
    	int cch=fch%24; 
    	return 23-cch+card*24;
    	//return cch+card*24;
}

int GetPSlot(int gch){

	//cokcok for DIRC
    	if(gch<8)return 8;
      
    	//cokcok for PS 
    	//if(gch>91)return 8;

	return gem_p_slot+(int)(gch+gem_p_ch0)/72+first_slot;
}

int GetPChan(int gch){

    	//cokcok for DIRC
    	if(gch<8)return 71;
    	int fch=(gch-8+gem_p_ch0)%72;

    	//cokcok for PS
    	//if(gch>91)return 71;
    	//int fch=(gch+4+gem_p_ch0)%72;

    	int card=fch/24;
    	int cch=fch%24; 

    	//cokcok for DIRC
    	return cch+card*24;

    	//cokcok for PS
    	//return 23-cch+card*24;
}

//cok #define gem_p_ch0 48

/*
#define gem_x_slot 1
#define gem_y_slot 4
#define gem_y_ch0 24

int GetXSlot(int gch){
return gem_x_slot+(int)gch/72;
}

int GetXChan(int gch){
    int fch=gch%72; //inversed fADC chan
    int card=fch/24;
    int cch=fch%24; //inversed card chan
    return 23-cch+card*24;
}

int GetYSlot(int gch){
return gem_y_slot+(int)gch/72;
}
int GetYChan(int gch){
    int fch=gch%72; //inversed fADC chan
    int card=fch/24;
    int cch=fch%24; //inversed card chan
    return 23-cch+card*24+gem_y_ch0;
}
*/

TH2F *crate;
int main(int argc, char *argv[]) {  
  
  char CrateName[128] = "ROCFDC";
  int version = 1;
  int ROC = 2;
  int SLOT = 3;
  int CHANNEL = 1;
  int DISPLAY = 0;

  char InputFile[128];
  int ROCNum[10] = {2,3,5,6,7,8,9,10,11,12};

  for (int i=0;i<10;i++){
  trig_trd[i]=0; 
  trig_st[i]=0; 
  trig_ps[i]=0; 
  }
  
  trd_cnt=0; 
  st_cnt=0; 
  ps_cnt=0; 

 to_sergey_rad.open("to_sergey_dedx_rad.dat",ios::out);
 to_sergey_norad.open("to_sergey_dedx_norad.dat",ios::out);
 to_sergey_rad2.open("to_sergey_dedx_rad2.dat",ios::out);
 to_sergey_norad2.open("to_sergey_dedx_norad2.dat",ios::out);

  TApplication theApp("App", &argc, argv);
  if (argc>1){
    for (int i=1;i<argc;i++){
      	if (!strcmp(argv[i],"-ND")){
	DISPLAY = 0;
      }

     	if (!strcmp(argv[i],"-V")){
        version = atoi(argv[i+1]);
        i++;
      }

      	if (!strcmp(argv[i],"-R")){
        ROC = atoi(argv[i+1]);
        i++;
      }

      	if (!strcmp(argv[i],"-S")){
        SLOT = atoi(argv[i+1]);
        i++;
      }

      	if (!strcmp(argv[i],"-C")){
        CHANNEL = atoi(argv[i+1]);
        i++;
      }

      	if (!strcmp(argv[i],"-F")){
        sprintf(CrateName,"%s",argv[i+1]);
        i++;
      }

      	if (!strcmp(argv[i],"h")){
	cout<<"-ND      do not display individual events"<<endl;
        cout<<"-R #     ROC number (default 2)"<<endl;
        cout<<"-S  #    SLOT number (default 3)"<<endl;
        cout<<"-C #     CHANNEL number (default 1)"<<endl;
        cout<<"-V #     data version number (default 1)"<<endl;
	cout<<"-F string  data label (default ROCFDC) \"string\"   "<<endl;
	return 0;
      }   
    }
  }


  sprintf(InputFile,"%s",argv[1]);
  int RunNumber = atoi(argv[1]);

  //cok local
  //char DataDir[128] = ".";
  //cok PS char DataDir[128] = "/gluonraid3/data4/rawdata/trd/DATA";                     
  //char DataDir[128] = "/gluonraid3/data4/rawdata/trd/DATA";                     
  char DataDir[128] = "DATA";                     

  float tmax1=452.;
  float dmax1=40.;
  float tmax2=112.;
  float dmax2=35.;
  char fnam[228];
  char root_fname[228];
  ifstream INF;
  
  char lin[50000];
  int NSlot = -1;
  int NCon = 0;
  int EventNew = 1;
  float x[100000];
  float y[100000];
  float dx[100000];
  float dy[100000];

  //cokXe float thresh=800.;
  //cokforAr? float thresh=500.;
  //cok float thresh=100.;
  float thresh=100.;
  float sthresh=40.;
  float bthresh=40.;
  int Slot=SLOT;
  int EVENT;
  short EvtSize;
  int SlotNum; 
  int chan;
  int samples;
  int isamp=0;

  int trig=0;

  float umax=0.;
  float uped=0.;
  int uchmax=0;
  float ucent=0;
  float ufit=0;
  float umcent=0;
  float umfit=0;
  float usig=0;
  int uwid=0;
  int usmax=0;
  float dmax=0.;

  float dped=0.;
  int dchmax=0;
  float dcent=0;
  float dfit=0;
  float dmcent=0;
  float dmfit=0;
  float dsig=0;
  int dwid=0;
  int dsmax=0;

  float pped=0.;
  int pchmax=0;
  int pxchmax=0;
  int pychmax=0;
  float pcent=0;
  float pxcent=0;
  float pycent=0;
  float pxfit=0;
  float pyfit=0;
  float pmcent=0;
  float pmfit=0;
  float pxsig=0;
  float pysig=0;
  int pwid=0;
  int pxwid=0;
  int pywid=0;
  int psmax=0;

  float wmax=0.;
  float wped=0.;
  int wchmax=0;
  float wcent=0;
  float wfit=0;
  float wmcent=0;
  float wmfit=0;
  float wsig=0;
  int wwid=0;
  int wsmax=0;
  int pssmax=0;
  float w2max=0.;
  float w2ped=0.;
  int w2chmax=0;
  float pschmax=0;
  int pscchmax=0;
  float w2cent=0;
  float w2fit=0;
  float w2mcent=0;
  float w2mfit=0;
  float w2sig=0;
  int w2wid=0;
  int w2smax=0;
  float ucharge=0.;
  float dcharge=0.;
  float pcharge=0.;
  float wcharge=0.;
  float w2charge=0.;
  float pscharge=0.;
  float psccharge=0.;
  int wnumber=0;
  int w2number=0;
  float umcharge=0.;
  float dmcharge=0.;
  float pmcharge=0.;
  float wmcharge=0.;
  float w2mcharge=0.;
  int usize=0;
  int dsize=0;
  int psize=0;
  int wsize=0;
  int w2size=0;

  int wnhit;
  float wthit[1000];
  float wahit[1000];
  float wmhit[1000];
  int wchit[1000];
  int unhit;
  float uthit[1000];
  float uahit[1000];

  int w2nhit;
  float w2thit[1000];
  float w2ahit[1000];
  float w2mhit[1000];
  int w2chit[1000];
  int dnhit;
  float dthit[1000];
  float dahit[1000];
  float dmhit[1000];
  int dchit[1000];

   TCanvas *myc;
   myc = new TCanvas("myc", "Event", 600, 600);

    
    //cok sprintf(root_fname,"%s.%s",InputFile,"psroot");
    sprintf(InputFile,"hd_rawdata_%06d.evio",RunNumber);
    sprintf(root_fname,"%s.%s",InputFile,"disproot");
    cout<<"Opening root file "<<root_fname<<endl;
    ROOTfile = new TFile(root_fname,"RECREATE"," 55Fe root tree results ");
    ROOTfile->cd();
    fdcFeTree = new TTree( "fdctest", "FDC response" );
    fdcFeTree->Branch( "ev", &EVENT, "ev/I" );
    fdcFeTree->Branch( "max_u", &umax, "max_u/F" );
    fdcFeTree->Branch( "max_d", &dmax, "max_d/F" );
    fdcFeTree->Branch( "max_w", &wmax, "max_w/F" );
    fdcFeTree->Branch( "max_w2", &w2max, "max_w2/F" );
    fdcFeTree->Branch( "ped_u", &uped, "ped_u/F" );
    fdcFeTree->Branch( "ped_d", &dped, "ped_d/F" );
    fdcFeTree->Branch( "ped_w", &wped, "ped_w/F" );
    fdcFeTree->Branch( "ped_w2", &w2ped, "ped_w2/F" );
    fdcFeTree->Branch( "q_u", &ucharge, "q_u/F" );
    fdcFeTree->Branch( "q_d", &dcharge, "q_d/F" );
    fdcFeTree->Branch( "q_p", &pcharge, "q_p/F" );
    fdcFeTree->Branch( "q_w", &wcharge, "q_w/F" );
    fdcFeTree->Branch( "q_w2", &w2charge, "q_w2/F" );
    fdcFeTree->Branch( "w2_n", &w2number, "w2_n/I" );
    fdcFeTree->Branch( "qm_u", &umcharge, "qm_u/F" );
    fdcFeTree->Branch( "qm_d", &dmcharge, "qm_d/F" );
    fdcFeTree->Branch( "qm_p", &pmcharge, "qm_p/F" );
    fdcFeTree->Branch( "qm_w", &wmcharge, "qm_w/F" );
    fdcFeTree->Branch( "qm_w2", &w2mcharge, "qm_w2/F" );
    fdcFeTree->Branch( "ucent", &ucent, "ucent/F" );
    fdcFeTree->Branch( "ufit", &ufit, "ufit/F" );
    fdcFeTree->Branch( "umcent", &umcent, "umcent/F" );
    fdcFeTree->Branch( "umfit", &umfit, "umfit/F" );
    fdcFeTree->Branch( "usig", &usig, "usig/F" );
    fdcFeTree->Branch( "uwid", &uwid, "uwid/I" );
    fdcFeTree->Branch( "dcent", &dcent, "dcent/F" );
    fdcFeTree->Branch( "dfit", &dfit, "dfit/F" );
    fdcFeTree->Branch( "dmcent", &dmcent, "dmcent/F" );
    fdcFeTree->Branch( "dmfit", &dmfit, "dmfit/F" );
    fdcFeTree->Branch( "dsig", &dsig, "dsig/F" );
    fdcFeTree->Branch( "dwid", &dwid, "dwid/I" );
    fdcFeTree->Branch( "pcent", &pcent, "pcent/F" );
    fdcFeTree->Branch( "pxcent", &pxcent, "pxcent/F" );
    fdcFeTree->Branch( "pycent", &pycent, "pycent/F" );
    fdcFeTree->Branch( "pxfit", &pxfit, "pxfit/F" );
    fdcFeTree->Branch( "pyfit", &pyfit, "pyfit/F" );
    fdcFeTree->Branch( "pmcent", &pmcent, "pmcent/F" );
    fdcFeTree->Branch( "pmfit", &pmfit, "pmfit/F" );
    fdcFeTree->Branch( "pxsig", &pxsig, "pxsig/F" );
    fdcFeTree->Branch( "pysig", &pysig, "pysig/F" );
    fdcFeTree->Branch( "pwid", &pwid, "pwid/I" );
    fdcFeTree->Branch( "pxwid", &pxwid, "pxwid/I" );
    fdcFeTree->Branch( "pywid", &pywid, "pywid/I" );
    fdcFeTree->Branch( "wcent", &wcent, "wcent/F" );
    fdcFeTree->Branch( "wfit", &wfit, "wfit/F" );
    fdcFeTree->Branch( "wmcent", &wmcent, "wmcent/F" );
    fdcFeTree->Branch( "wmfit", &wmfit, "wmfit/F" );
    fdcFeTree->Branch( "wsig", &wsig, "wsig/F" );
    fdcFeTree->Branch( "wwid", &wwid, "wwid/I" );
    fdcFeTree->Branch( "w2cent", &w2cent, "w2cent/F" );
    fdcFeTree->Branch( "w2fit", &w2fit, "w2fit/F" );
    fdcFeTree->Branch( "w2mcent", &w2mcent, "w2mcent/F" );
    fdcFeTree->Branch( "w2mfit", &w2mfit, "w2mfit/F" );
    fdcFeTree->Branch( "w2sig", &w2sig, "w2sig/F" );
    fdcFeTree->Branch( "w2wid", &w2wid, "w2wid/I" );
    fdcFeTree->Branch( "t_u", &usmax, "t_u/I" );
    fdcFeTree->Branch( "t_d", &dsmax, "t_d/I" );
    fdcFeTree->Branch( "t_p", &psmax, "t_p/I" );
    fdcFeTree->Branch( "t_w", &wsmax, "t_w/I" );
    fdcFeTree->Branch( "t_w2", &w2smax, "t_w2/I" );
    fdcFeTree->Branch( "uch", &uchmax, "uch/I" );
    fdcFeTree->Branch( "dch", &dchmax, "dch/I" );
    fdcFeTree->Branch( "pch", &pchmax, "pch/I" );
    fdcFeTree->Branch( "pxch", &pxchmax, "pxch/I" );
    fdcFeTree->Branch( "pych", &pychmax, "pych/I" );
    fdcFeTree->Branch( "wch", &wchmax, "wch/I" );
    fdcFeTree->Branch( "w2ch", &w2chmax, "w2ch/I" );
    fdcFeTree->Branch( "usz", &usize, "usz/I" );
    fdcFeTree->Branch( "dsz", &dsize, "dsz/I" );
    fdcFeTree->Branch( "wsz", &wsize, "wsz/I" );
    fdcFeTree->Branch( "w2sz", &w2size, "w2sz/I" );
    fdcFeTree->Branch( "q_ps", &pscharge, "q_ps/F" );
    fdcFeTree->Branch( "psch", &pschmax,"psch/F" );
    fdcFeTree->Branch( "t_ps", &pssmax, "t_ps/I" );
    fdcFeTree->Branch( "q_psc", &psccharge, "q_psc/F" );
    fdcFeTree->Branch( "pscch", &pscchmax,"pscch/I" );
    fdcFeTree->Branch( "trig", &trig,"trig/I" );
    fdcFeTree->Branch( "wnhit", &wnhit, "wnhit/I" );
    fdcFeTree->Branch( "wthit", &wthit, "wthit[wnhit]/F" );
    fdcFeTree->Branch( "wahit", &wahit, "wahit[wnhit]/F" );
    fdcFeTree->Branch( "wmhit", &wmhit, "wmhit[wnhit]/F" );
    fdcFeTree->Branch( "wchit", &wchit, "wchit[wnhit]/I" );
    fdcFeTree->Branch( "w2nhit", &w2nhit, "w2nhit/I" );
    fdcFeTree->Branch( "w2thit", &w2thit, "w2thit[w2nhit]/F" );
    fdcFeTree->Branch( "w2ahit", &w2ahit, "w2ahit[w2nhit]/F" );
    fdcFeTree->Branch( "w2mhit", &w2mhit, "w2mhit[w2nhit]/F" );
    fdcFeTree->Branch( "w2chit", &w2chit, "w2chit[w2nhit]/I" );
    fdcFeTree->Branch( "unhit", &unhit, "unhit/I" );
    fdcFeTree->Branch( "uthit", &uthit, "uthit[unhit]/F" );
    fdcFeTree->Branch( "uahit", &uahit, "uahit[unhit]/F" );

    TH1D *FHist = new TH1D("FHist","Histogram for centroid fitting",7,-0.5,6.5);
    TH2D *DHist = new TH2D("DHist","Histogram for centroid fitting",10,-0.5,9.5,10,-0.5,9.5);

    gStyle->SetOptStat(0);
    TH2F *pad_plot = new TH2F("pad_plot","padGEM ",10,-0.5,9.5,10,-0.5,9.5);

    //TH2F *ct_plot = new TH2F("ct_plot","GEM X vs Z",600,-72.5/2.5,479.5/2.5,300,-0.5/10.,299.5/10.);
    //cok TH2F *ct_plot = new TH2F("ct_plot","GEM ",240,-72.5,119.5,300,-0.5/1.,299.5/1.);
    TH2F *ct_plot = new TH2F("ct_plot","GEM Y",240,(-0.5-72.)*0.4,(239.5-120.)*0.4,200,-0.5/1.,199.5/1.);

    crate = new TH2F("crate","",8*72,-0.5,12*100.-0.5,410,0.,4100.);
    TAxis *xaxis = (TAxis*)ct_plot->GetXaxis();
    xaxis->SetTitle("y, mm  ");
    TAxis *yaxis = (TAxis*)ct_plot->GetYaxis();
    yaxis->SetTitle("t, 8ns  ");
    //494 int w2choffset=-180;
    //cok int w2choffset=-90;
    int w2choffset=0;
    w2choffset=-72;
    ct_plot->SetMaximum(4000.);
    ct_plot->SetMinimum(1.);  //-- -40 fsv 
    //TH2F *wct_plot = new TH2F("wct_plot","WC X vs Z",24,-0.5*10.,23.5*10.,300,-0.5/10.,299.5/10.);
    //cok TH2F *wct_plot = new TH2F("wct_plot","WC ",24,-12.05*10.,11.95*10.,300,-0.5/10.,299.5/10.);
    TH2F *wct_plot = new TH2F("wct_plot","WC X",24,-12.05*10.,11.95*10.,200,-0.5/1.,199.5/1.);
    xaxis = (TAxis*)wct_plot->GetXaxis();
    xaxis->SetTitle("x, mm  ");
    yaxis = (TAxis*)wct_plot->GetYaxis();
    yaxis->SetTitle("t, 8ns  ");
    int wchoffset=-12;
    //wchoffset=0;
    wct_plot->SetMaximum(4000.);
    wct_plot->SetMinimum(1.);  // fsv -40
    //TH2F *cty_plot = new TH2F("cty_plot","GEM Y vs Z",600,-324.5/2.5,275.5/2.5,300,-0.5/10.,299.5/10.);
    //TH2F *cty_plot = new TH2F("cty_plot","GEM ",600,-300.5/2.5,299.5/2.5,300,-0.5/10.,299.5/10.);
    TH2F *cty_plot = new TH2F("cty_plot","GEM X",240,(-0.5-72.)*0.4,(239.5-120.)*0.4,200,-0.5/1.,199.5/1.);
    xaxis = (TAxis*)cty_plot->GetXaxis();
    xaxis->SetTitle("y, mm  ");
    yaxis = (TAxis*)cty_plot->GetYaxis();
    yaxis->SetTitle("z, mm  ");
    int dchoffset=12;
    dchoffset=-72.;
    cty_plot->SetMaximum(4000.);
    cty_plot->SetMinimum(1.);  // -30  fsv 
    //TH2F *wcty_plot = new TH2F("wcty_plot","WC Y vs Z",48,-12.5*5.,35.5*5.,300,-0.5/10.,299.5/10.);
    TH2F *wcty_plot = new TH2F("wcty_plot","WC Y",24,-12.05*5.,11.95*5.,200,-0.5/1.,199.5/1.);
    xaxis = (TAxis*)wcty_plot->GetXaxis();
    xaxis->SetTitle("y, mm  ");
    yaxis = (TAxis*)wcty_plot->GetYaxis();
    yaxis->SetTitle("z, mm  ");
    int uchoffset=-12;
    //uchoffset=0;
    wcty_plot->SetMaximum(400.);
    wcty_plot->SetMinimum(1.);

    TH2D *PStest = new TH2D("PStest","PS time vs chmax",100,-0.5,99.5,256,0.5,255.5);
    //TRIGtest= new TH2D("TRIGtest","TRIG time test",1000,-10000000.5,10000000.-0.5,100,0.5,1000.5);
    //cok 3d  TH3D *wAvsT = new TH3D("wAvsT","Amplitude vs Time",400,-0.5,399.5,1024,-0.5,4095.5,48,-0.5,47.5);
    //cok 3d  TH3D *w2AvsT = new TH3D("w2AvsT","Amplitude vs Time",400,-0.5,399.5,1024,-0.5,4095.5,48,-0.5,47.5);
    TH1D *xeff = new TH1D("xeff","X-coordinate of the track",48,-0.5,47.5);
    TH1D *x2eff = new TH1D("x2eff","X-coordinate of the track",48,-0.5,47.5);
    //TH2D *wAvsT = new TH2D("wAvsT","Ch.1 Amplitude vs Time",72,-0.5,119.5,110,-0.5,109.5);
    //cok TH2D *wAvsT = new TH2D("wAvsT","Ch.1 Amplitude vs Time",72,-0.5,119.5,24,-0.5,23.5);
    TH2D *wAvsT = new TH2D("wAvsT","Ch.1 Amplitude vs Time",400,-0.5,399.5,24,-0.5,23.5);
    TH2D *wAvsTnorm = new TH2D("wAvsTnorm","Ch.1 Amplitude vs Time norm",400,-0.5,399.5,24,-0.5,23.5);
    //TH2D *wAvsT = new TH2D("wAvsT","Ch.1 Amplitude vs Time",72,-0.5,119.5,100,9.5,13.5);
    TH2D *wCvsT = new TH2D("wCvsT","Ch.1 Width (transverese) vs Time",400,-0.5,399.5,24,-0.5,23.5);
    TH2D *wCvsL = new TH2D("wCvsL","Ch.1 Width (longitudinal) vs Time",400,-0.5,399.5,24,-0.5,23.5);
    TH2D *wAvsB = new TH2D("wAvsB","Ch.1 Amplitude vs Time (background)",1000,-0.5,999.5,48,-0.5,47.5);
    TH2D *wCvsB = new TH2D("wCvsB","Ch.1 Counts vs Time (background)",1000,-0.5,999.5,48,-0.5,47.5);
    TH2D *wYvsB = new TH2D("wYvsB","Ch.1 XY vs Time (background)",48,-0.5,47.5,96,-48.,48.);
    TH2D *wTYvsB = new TH2D("wTYvsB","Ch.1 TY vs Time (background)",1000,-0.5,999.5,96,-48.,48.);
    TH2D *wAvsD = new TH2D("wAvsD","Ch.1 Amplitude vs Distance",1000,0.,40.,48,-0.5,47.5);
    wAvsT->Sumw2();
    wCvsT->Sumw2();
    wAvsB->Sumw2();
    wCvsB->Sumw2();
    wYvsB->Sumw2();
    wTYvsB->Sumw2();
    wAvsD->Sumw2();
    TH2D *w2AvsT = new TH2D("w2AvsT","Ch.2 Amplitude vs Time",400,-0.5,399.5,240,-0.5,239.5);
    TH2D *w2AvsTnorm = new TH2D("w2AvsTnorm","Ch.2 Amplitude vs Time norm",400,-0.5,399.5,240,-0.5,239.5);
    TH2D *w2CvsT = new TH2D("w2CvsT","Ch.2 Width (transverse) vs Time",400,-0.5,399.5,240,-0.5,239.5);
    TH2D *w2CvsL = new TH2D("w2CvsL","Ch.2 Width (longitudinal) vs Time",400,-0.5,399.5,240,-0.5,239.5);
    TH2D *w2AvsB = new TH2D("w2AvsB","Ch.2 Amplitude vs Time (background)",1000,-0.5,999.5,240,-0.5,249.5);
    TH2D *w2CvsB = new TH2D("w2CvsB","Ch.2 Counts vs Time (background)",1000,-0.5,999.5,240,-0.5,249.5);
    TH2D *w2YvsB = new TH2D("w2YvsB","Ch.2 X vs Y (background)",240,-0.5,249.5,240,-0.5,249.5);
    TH2D *w2TYvsB = new TH2D("w2TYvsB","Ch.1 TY vs Time (background)",1000,-0.5,999.5,240,-0.5,249.5);
    TH2D *w2AvsD = new TH2D("w2AvsD","Ch.2 Amplitude vs Distance",1000,0.,35.,240,-0.5,249.5);
    w2AvsTnorm->Sumw2();
    w2AvsT->Sumw2();
    w2CvsT->Sumw2();
    w2CvsL->Sumw2();
    w2AvsB->Sumw2();
    w2CvsB->Sumw2();
    w2YvsB->Sumw2();
    w2TYvsB->Sumw2();
    w2AvsD->Sumw2();

    TH2D *dAvsT = new TH2D("dAvsT","Ch.2 Amplitude vs Time",400,-0.5,399.5,240,-0.5,239.5);
    TH2D *dAvsTnorm = new TH2D("dAvsTnorm","Ch.2 Amplitude vs Time norm",400,-0.5,399.5,240,-0.5,239.5);
    TH2D *dCvsT = new TH2D("dCvsT","Ch.2 Width (transverse) vs Time",400,-0.5,399.5,240,-0.5,239.5);
    TH2D *dCvsL = new TH2D("dCvsL","Ch.2 Width (longitudinal) vs Time",400,-0.5,399.5,240,-0.5,239.5);
    TH2D *dAvsB = new TH2D("dAvsB","Ch.2 Amplitude vs Time (background)",1000,-0.5,999.5,240,-0.5,249.5);
    TH2D *dCvsB = new TH2D("dCvsB","Ch.2 Counts vs Time (background)",1000,-0.5,999.5,240,-0.5,249.5);
    TH2D *dYvsB = new TH2D("dYvsB","Ch.2 X vs Y (background)",240,-0.5,249.5,240,-0.5,249.5);
    TH2D *dTYvsB = new TH2D("dTYvsB","Ch.1 TY vs Time (background)",1000,-0.5,999.5,240,-0.5,249.5);
    TH2D *dAvsD = new TH2D("dAvsD","Ch.2 Amplitude vs Distance",1000,0.,35.,240,-0.5,249.5);
    dAvsTnorm->Sumw2();
    dAvsT->Sumw2();
    dCvsT->Sumw2();
    dCvsL->Sumw2();
    dAvsB->Sumw2();
    dCvsB->Sumw2();
    dYvsB->Sumw2();
    dTYvsB->Sumw2();
    dAvsD->Sumw2();

    TH2D *uAvsT = new TH2D("uAvsT","Amplitude vs Time",1000,-0.5,999.5,4096,-0.5,4095.5);

    //TH2D *TTime = new TH2D("TTime","Time between peaks vs time",1000,-0.5,999.5,1000,-0.5,999.5);
    TH2D *Timeax = new TH2D("Timeax","Time between peaks vs time",1000,-0.5,999.5,48,-24.5,23.5);
    TH2D *Timebx = new TH2D("Timebx","Time between peaks vs time",1000,-0.5,999.5,48,-24.5,23.5);
    TH2D *DTimeax = new TH2D("DTimeax","Time between peaks vs time",1000,-0.5,999.5,48,-24.5,23.5);
    TH2D *DTimebx = new TH2D("DTimebx","Time between peaks vs time",1000,-0.5,999.5,48,-24.5,23.5);
    TH2D *CluCou = new TH2D("CluCou","cluster no. in ch.1 vs ch2",100,-0.5,99.5,100,-0.5,99.5);
    //TF1 *centroid=new TF1("centroid","[0]*(1-tanh([2]*(x-[1]))*tanh([2]*(x-[1])))",N1Up-0.5,N1Up+Aup-0.5);
    TF1 *centroid=new TF1("centroid","[0]*exp(-(x-[1])^2/(2.*[2]^2))",-0.5,6.5);

    TF2 *dgaus = new TF2("dgaus","[0]*TMath::Gaus(x,[1],[2])*TMath::Gaus(y,[3],[4])",0,10,0,10); 

    for(int slot=0;slot<15;slot++){
      for(int ch=0;ch<72;ch++){
          for (int sm=0;sm<2000;sm++){
	  ADCSamples[0][slot][ch][sm] = -1000.;
          }
       ADCPedestal[0][slot][ch] = 0.;
       }
    }

  long int evtCount=0;
  long int NEVENT=2000000;
  //cedicedi
  int OK=1;
  bool fOK=true;

  while (fOK) { //file loop
    ostringstream ss;
    ss<<setw(5)<<setfill('0')<<RunNumber;
    string str1(ss.str());
  
    sprintf(InputFile,"hd_rawdata_%06d_00%d.evio",RunNumber,OK-1);
    sprintf(fnam,"%s/%s",DataDir,InputFile);
    cout<<"Opening file "<<fnam<<endl;
      if (FILE *file = fopen(fnam,"r")){
        fclose(file);
        OK++;
        fOK=true;
        cout<<"File exists!"<<endl;
      } else {
        OK=0;
        fOK=false;
	cout<<"File not found - closing"<<endl;
      }
      if (fOK){
  	evioFileChannel EvOchan(fnam,"r",8000000);
        cout<<"here 1..."<<endl;
  	EvOchan.open();
        bool ReadBack = EvOchan.read();
        cout<<"here 2..."<<endl;
      if (1==1){
        while(ReadBack && evtCount<NEVENT) {
          evtCount++;
          EVENT=evtCount;
          // begin event loop
          if(evtCount%100==0)
          //if(evtCount%1==0)
          cout<<"[][][][][][][][][][][][] new event "<<evtCount<<" [][][][][][][][][][]"<<endl;
 
          if(11==11) {
          // cout<<"[][][][][][][][][][][][] new event "<<evtCount<<" [][][][][][][][][][]"<<endl;

         try {
   	 TRIGGER_MASK_GT=0;
   	 evioDOMTree eventTree(EvOchan);
   	 //cout<<" start analyze event"<<endl;
   	 analyzeEvent(eventTree);
  	 //cout<<" stop analyze event"<<endl;
   	 trig=TRIGGER_MASK_GT;
  /*
  if (TRIGGER_MASK_GT>0){
      if (TRIGGER_MASK_GT & (1<<3)){
        trig=4;
      } else if (trig==3) {
        trig=100;
      }
  }
  */

         //if(evtCount<NEVENT&&evtCount>0)
         if(true){
         // slot1: cell1_w1, cell1_u1
         int slot1=0;

         int sl_c1w1=wire_slot;
         int ch_c1w1=wire_x_ch0;
         int sl_c1u1=slot1;
         int ch_c1u1=24;

         int sl_c2w1=1;
         int ch_c2w1=0;
         int sl_c2w2=1;
         int ch_c2w2=24;
         int sl_c2w3=1;
         int ch_c2w3=48;

         int sl_c2w4=2;
         int sl_c2w4=3;
         int ch_c2w4=0;
         int sl_c2w5=2;
         int ch_c2w5=24;
         int sl_c2w6=2;
         int ch_c2w6=48;

         int sl_c2w7=3;
         int ch_c2w7=0;
         int sl_c2w8=3;
         int ch_c2w8=24;
         int sl_c2w9=3;
         int ch_c2w9=48;

         int sl_c2w10=4;
         int ch_c2w10=0;
         int sl_c2d1=4;
         int ch_c2d1=24;
         int sl_c2d2=4;
         int ch_c2d2=48;

         float uADCmax[72][400];
         float wADCmax[72][400];
         float uADCsum[72];
         float wADCsumall[72];
         float wADCsum[72][400];

         int uSAMPmax[72][400];
         int wSAMPmax[72][400];
         int wNSAMP[72][400];
         int uNhit[72];
         int wNhit[72];
         int uSmax[72];
         int wSmax[72];
         float uAmax[72];
         float wAmax[72];
         float wAmin[72];

         float dADCmax[240][400];
         float pADCmax[240][400];
         float w2ADCmax[240][400];
         float dADCsum[240][400];
         float pADCsum[240][400];
         float w2ADCsum[240][400];
         float dADCsumall[240];
         float pADCsumall[240];
         float w2ADCsumall[240];

         int pSAMPmax[240][400];
         int dSAMPmax[240][400];
         int w2SAMPmax[240][400];
         int dNSAMP[240][400];
         int pNSAMP[240][400];
         int w2NSAMP[240][400];
         int dNhit[240];
         int pNhit[240];
         int w2Nhit[240];
         int dSmax[240];
         int pSmax[240];
         int w2Smax[240];
         float dAmax[240];
         float pAmax[240];
         float w2Amax[240];
         float dAmin[240];
         float pAmin[240];
         float w2Amin[240];

         float adcval;
         float adcval1;
         float adcval2;

/*
         int pwd[100];
         for (int ch=0;ch<100;ch++){
            pADCsumall[ch]=0.;
            pSmax[ch]=0;
            pAmax[ch]=0.;
            dAmin[ch]=5000.;
            pwd[ch]=0;
            for (int hit=0;hit<10;hit++){
              pADCmax[ch][hit]=0.;
              pSAMPmax[ch][hit]=0;
              pNSAMP[ch][hit]=0;
              pADCsum[ch][hit]=0.;
            }
         }
*/ 

      // fixed pedestal !!!
         for(int ch=0;ch<72;ch++){
	 //  ADCPedestal[0][uslot][ch] = 100.;
         }

         samples=WindowSize;
         //cout<<"samples = "<<samples<<endl;

//
// Cell1 U-strips
//
         int uwd[24];
         for (int ch=0;ch<24;ch++){
            uADCsum[ch]=0.;
            uwd[ch]=0.;
            uSmax[ch]=0;
            uAmax[ch]=0.;
            for (int hit=0;hit<400;hit++){
              uADCmax[ch][hit]=0.;
              uSAMPmax[ch][hit]=0;
            }
         }
         for (int ch=0;ch<24;ch++){
            int slot; int ch0;
            slot=sl_c1u1; ch0=ch_c1u1;
            uNhit[ch]=0;
            float adcmax=0.;
            for (int i=1;i<samples-1;i++){
               adcval=ADCSamples[0][slot][ch0+ch][i]-ADCPedestal[0][slot][ch0+ch];
               adcval1=ADCSamples[0][slot][ch0+ch][i-1]-ADCPedestal[0][slot][ch0+ch];
               adcval2=ADCSamples[0][slot][ch0+ch][i+1]-ADCPedestal[0][slot][ch0+ch];
               if(adcval>thresh/2.){
                  uADCsum[ch]+=adcval;
                  uwd[ch]++;
                  if(adcval>adcval1&&adcval>adcval2){
                     uADCmax[ch][uNhit[ch]]=adcval;
                     uSAMPmax[ch][uNhit[ch]]=i;
                     uNhit[ch]++;
                     if(adcval>adcmax){
                        adcmax=adcval;
                        uAmax[ch]=adcval;
                        uSmax[ch]=i;
                     }
                  }
               }
            }
         }

//
// Cell1 wires
//
         int wwd[72];
         for (int ch=0;ch<72;ch++){
             wADCsumall[ch]=0.;
             wwd[ch]=0.;
             wSmax[ch]=0;
             wAmax[ch]=0.;
             wAmin[ch]=5000.;
             for (int hit=0;hit<400;hit++){
                 wADCmax[ch][hit]=0.;
                 wSAMPmax[ch][hit]=0;
                 wADCsum[ch][hit]=0.;
                 wNSAMP[ch][hit]=0;
             }
         }

         for (int ch=0;ch<72;ch++){
             int slot; int ch0;
             slot=sl_c1w1; ch0=ch_c1w1;
             wNhit[ch]=0;
             float adcmax=0.;
             for (int i=4;i<samples-4;i++){
                 adcval=ADCSamples[0][slot][ch+ch0][i]-ADCPedestal[0][slot][ch+ch0];
                 adcval1=ADCSamples[0][slot][ch+ch0][i-1]-ADCPedestal[0][slot][ch+ch0];
                 adcval2=ADCSamples[0][slot][ch+ch0][i+1]-ADCPedestal[0][slot][ch+ch0];
                 if(adcval<wAmin[ch])wAmin[ch]=adcval;
                 if(adcval>thresh){
                    wADCsumall[ch]+=adcval;
                    wwd[ch]++;
                    if(adcval>adcval1&&adcval>adcval2){
                       wADCmax[ch][wNhit[ch]]=adcval;
                       wSAMPmax[ch][wNhit[ch]]=i;
                       for (int is=-2;is<3;is++){
                          float adcl=ADCSamples[0][slot][ch+ch0][i+is]-ADCPedestal[0][slot][ch+ch0];
                          if(adcl>thresh/2.){
                             wADCsum[ch][wNhit[ch]]+=adcl;
                             wNSAMP[ch][wNhit[ch]]++;
                          }
                       }
                       wNhit[ch]++;
                       if(adcval>adcmax){
                          adcmax=adcval;
                          // first sample cok ????
                          adcmax=8000.;
                          wAmax[ch]=adcval;
                          wSmax[ch]=i;
                          wped=ADCPedestal[0][slot][ch+ch0];
                       }
                    }
                }
             }
         }

//
// Cell1 2D clustering
//

         float wcht[24][400]; // amplitude vs channel and sample
         for (int ch=0;ch<24;ch++){
            for (int sm=0;sm<400;sm++){
               wcht[ch][sm]=0.;
            }
         }

         int wnclust=0;
         float wclustamp[200]; //cluster amplitide
         float wclustchn[200]; //cluster channel no
         float wclustsmp[200]; //cluster time (sample no)
         float wclusttwd[200]; //cluster transverse width (in channels)
         float wclustlwd[200]; //cluster lateral width (in samples)
         for (int i=0;i<200;i++){
             wclustamp[i]=0.;
             wclustchn[i]=0.; 
             wclustsmp[i]=0.;
             wclusttwd[i]=0.;
             wclustlwd[i]=0.;
         }

         for (int ch=0;ch<24;ch++){
             for (int ihit=0;ihit<wNhit[ch];ihit++){
                 int sm=wSAMPmax[ch][ihit];
                 wcht[ch][sm]=wADCsum[ch][ihit];
             } //end hit loop
         } //end channel loop

         for (int ch=2;ch<22;ch++){
             for (int ihit=0;ihit<wNhit[ch];ihit++){
                 int sm=wSAMPmax[ch][ihit];
                 if(sm>400)break;
                 float a=wcht[ch][sm];
                 if((a>wcht[ch-2][sm]&&a>wcht[ch-2][sm-1]&&a>wcht[ch-2][sm+1])
                 &&(a>wcht[ch-1][sm]&&a>wcht[ch-1][sm-1]&&a>wcht[ch-1][sm+1])
                 &&(a>wcht[ch+2][sm]&&a>wcht[ch+2][sm-1]&&a>wcht[ch+2][sm+1])
                 &&(a>wcht[ch+1][sm]&&a>wcht[ch+1][sm-1]&&a>wcht[ch+1][sm+1])) { //2d max simple condition
                    wclustsmp[wnclust]=sm; //take the time at the maximum as cluster time
                    wclustchn[wnclust]=ch; //take the time at the maximum as cluster time
                    wclustlwd[wnclust]=wNSAMP[ch][ihit]; //number of samples for the maximum (longitudinal width)

                    int ch1=ch-2; //now sum all amplitudes around if > amax/5
                    if(ch1<0)ch1=0;
                    int ch2=ch+2;
                    if(ch2>23)ch2=23;
                    int chwid=0;
                    for (int ich=ch1;ich<ch2+1;ich++){
                        bool above_thresh=false;
                        for (int is=-2;is<3;is++){
                            float amp=wcht[ich][sm+is];
                            if(amp>thresh){
                               wclustamp[wnclust]+=amp;
                               above_thresh=true;
                            }
                        }
                        if(above_thresh)wclusttwd[wnclust]++;
                    }
                    wnclust++;
                } //end 2d max condition
            } //end hit loop
         } //end channel loop
// end c1 2d clustering

//
// Cell2 wires
//
         int w2wd[240];
         for (int ch=0;ch<120;ch++){
             w2ADCsumall[ch]=0.;
             w2wd[ch]=0.;
             w2Smax[ch]=0;
             w2Amax[ch]=0.;
             w2Amin[ch]=4000.;
             for (int hit=0;hit<400;hit++){
                 w2ADCmax[ch][hit]=0.;
                 w2SAMPmax[ch][hit]=0;
                 w2NSAMP[ch][hit]=0;
                 w2ADCsum[ch][hit]=0.;
             }
         }
         for (int ch=0;ch<120;ch++){
             int slot=GetXSlot(ch);
             int dch=GetXChan(ch);
             //cout<<" ch, slot, dch= "<<ch<<" "<<slot<<" "<<dch<<endl;
             w2Nhit[ch]=0;
             float adcmax=0.;
             for (int i=10;i<samples-10;i++){
                 adcval=ADCSamples[0][slot][dch][i]-ADCPedestal[0][slot][dch];
                 adcval1=ADCSamples[0][slot][dch][i-1]-ADCPedestal[0][slot][dch];
                 adcval2=ADCSamples[0][slot][dch][i+1]-ADCPedestal[0][slot][dch];
                 if(adcval<w2Amin[ch])w2Amin[ch]=adcval;
                 if(adcval>thresh){
                    w2ADCsumall[ch]+=adcval;
                    w2wd[ch]++;
                    if(adcval>adcval1&&adcval>adcval2){
                       w2ADCmax[ch][w2Nhit[ch]]=adcval;
                       w2SAMPmax[ch][w2Nhit[ch]]=i;
                       for (int is=-10;is<11;is++){
                           float adcl=ADCSamples[0][slot][dch][i+is]-ADCPedestal[0][slot][dch];
                           //if(adcval>200.)crate->Fill((float)slot*100.+dch,adcval);
                           if(adcl>thresh/2.){
                              w2ADCsum[ch][w2Nhit[ch]]+=adcl;
                              w2NSAMP[ch][w2Nhit[ch]]++;
                           }
                       }
                    w2Nhit[ch]++;
                    if(adcval>adcmax){
                       adcmax=adcval;
                       w2Amax[ch]=adcval;
                       w2Smax[ch]=i;
                       w2ped=ADCPedestal[0][slot][dch];
                    }
                    }
                 }
             }
         }

//
// Cell2 2D clustering
//

         float w2cht[240][400]; // amplitude vs channel and sample
         for (int ch=0;ch<240;ch++){
            for (int sm=0;sm<400;sm++){
               w2cht[ch][sm]=0.;
            }
         }

         int w2nclust=0;
         float w2clustamp[200]; //cluster amplitide
         float w2clustchn[200]; //cluster channel no
         float w2clustsmp[200]; //cluster time (sample no)
         float w2clusttwd[200]; //cluster transverse width (in channels)
         float w2clustlwd[200]; //cluster lateral width (in samples)
         for (int i=0;i<200;i++){
             w2clustamp[i]=0.;
             w2clustchn[i]=0.; 
             w2clustsmp[i]=0.;
             w2clusttwd[i]=0.;
             w2clustlwd[i]=0.;
         }

         for (int ch=0;ch<120;ch++){
             for (int ihit=0;ihit<w2Nhit[ch];ihit++){
                 int sm=w2SAMPmax[ch][ihit];
                 w2cht[ch][sm]=w2ADCsum[ch][ihit];
             } //end hit loop
         } //end channel loop

         for (int ch=2;ch<118;ch++){
             for (int ihit=0;ihit<w2Nhit[ch];ihit++){
                 int sm=w2SAMPmax[ch][ihit];
                 if(sm>400)break;
                 float a=w2cht[ch][sm];

                 if((a>w2cht[ch-2][sm]&&a>w2cht[ch-2][sm-1]&&a>w2cht[ch-2][sm+1])
                 &&(a>w2cht[ch-1][sm]&&a>w2cht[ch-1][sm-1]&&a>w2cht[ch-1][sm+1])
                 &&(a>w2cht[ch+2][sm]&&a>w2cht[ch+2][sm-1]&&a>w2cht[ch+2][sm+1])
                 &&(a>w2cht[ch+1][sm]&&a>w2cht[ch+1][sm-1]&&a>w2cht[ch+1][sm+1])) { //2d max simple condition

                    w2clustsmp[w2nclust]=sm; //take the time at the maximum as cluster time
                    w2clustchn[w2nclust]=ch; //take the time at the maximum as cluster time
                    w2clustlwd[w2nclust]=w2NSAMP[ch][ihit]; //number of samples for the maximum (longitudinal width)

                    int ch1=ch-10; //now sum all amplitudes around if > amax/5
                    if(ch1<0)ch1=0;
                    int ch2=ch+10;
                    if(ch2>239)ch2=239;
                    int chwid=0;
                    for (int ich=ch1;ich<ch2+1;ich++){
                        bool above_thresh=false;
                        for (int is=-10;is<11;is++){
                            float amp=w2cht[ich][sm+is];
                            if(amp>thresh/2.){
                               w2clustamp[w2nclust]+=amp;
                               above_thresh=true;
                            }
                        }
                        if(above_thresh)w2clusttwd[w2nclust]++;
                    }
                    w2nclust++;
                 } //end 2d max condition
             } //end hit loop
         } //end channel loop
         
         float clampmax=0.;
         w2mcharge=0.;
         w2smax=-100;
         //w2chmax=-100;
         for (int cl=0;cl<w2nclust;cl++){
             if(w2clustamp[cl]>clampmax){
                //w2mcharge=w2clustamp[cl];
                //w2chmax=w2clustchn[cl];
                w2smax=w2clustsmp[cl];
             }
         }
// end c2 2d clustering
         
//
// Cell2 d strips - MMG detector corresponds to "Cell2 d strips"
//
         int dwd[240];
         // It has 192 channels total
         for (int ch=0;ch<192;ch++){
             dADCsumall[ch]=0.;
             dwd[ch]=0;
             dSmax[ch]=0;
             dAmax[ch]=0.;
             dAmin[ch]=5000.;
             for (int hit=0;hit<400;hit++){
                 dADCmax[ch][hit]=0.;
                 dSAMPmax[ch][hit]=0;
                 dNSAMP[ch][hit]=0;
                 dADCsum[ch][hit]=0.;
             }
         }
         for (int ch=0;ch<192;ch++){
             //  cout<<" ch====================="<<ch<<endl;
             //  GetYSlot gives me the slot number that corresponds to this channel "ch"
             //  Same for GetYChan - gives the channel number within the module
             int slot=GetYSlot(ch);
             int dch=GetYChan(ch);
             dNhit[ch]=0;
             float adcmax=0.;
             // first & last 10 samples are excluded
             for (int i=10;i<samples-10;i++){
                 adcval=ADCSamples[0][slot][dch][i]-ADCPedestal[0][slot][dch];
                 adcval1=ADCSamples[0][slot][dch][i-1]-ADCPedestal[0][slot][dch];
                 adcval2=ADCSamples[0][slot][dch][i+1]-ADCPedestal[0][slot][dch];
                 if(adcval<dAmin[ch])dAmin[ch]=adcval;
                 if(adcval>thresh){
                    dADCsumall[ch]+=adcval;
                    dwd[ch]=dwd[ch]+1;
                    if(adcval>adcval1&&adcval>adcval2){
                       dADCmax[ch][dNhit[ch]]=adcval;
                       dSAMPmax[ch][dNhit[ch]]=i;
                       for (int is=-10;is<11;is++){
                           float adcl=ADCSamples[0][slot][dch][i+is]-ADCPedestal[0][slot][dch];
                           // look if maximum is above threshold "thresh" defined at the beginning of the code
                           if(adcl>thresh/2.){
                              dADCsum[ch][dNhit[ch]]+=adcl;
                              dNSAMP[ch][dNhit[ch]]++;
                           }
                       }
                       dNhit[ch]=dNhit[ch]+1;
                       if(adcval>adcmax){
                          adcmax=adcval;
                          dAmax[ch]=adcval;
                          dSmax[ch]=i;
                          dped=ADCPedestal[0][slot][dch];
                       }
                    }
                 }
             }
         }

//
// Cell2 2D d-strips clustering - MMG detector 2D clustering
//

         float dcht[240][400]; // amplitude vs channel and sample
         for (int ch=0;ch<192;ch++){
             for (int sm=0;sm<400;sm++){
                 dcht[ch][sm]=0.;
             }
         }

         int dnclust=0;
         float dclustamp[400]; //cluster amplitide
         float dclustchn[400]; //cluster channel no
         float dclustsmp[400]; //cluster time (sample no)
         float dclusttwd[400]; //cluster transverse width (in channels)
         float dclustlwd[400]; //cluster lateral width (in samples)
         for (int i=0;i<400;i++){
             dclustamp[i]=0.;
             dclustchn[i]=0.; 
             dclustsmp[i]=0.;
             dclusttwd[i]=0.;
             dclustlwd[i]=0.;
         }

         for (int ch=0;ch<192;ch++){
             for (int ihit=0;ihit<dNhit[ch];ihit++){
                 int sm=dSAMPmax[ch][ihit];
                 dcht[ch][sm]=dADCsum[ch][ihit];
             } //end hit loop
         } //end channel loop

         for (int ch=2;ch<190;ch++){
             for (int ihit=0;ihit<dNhit[ch];ihit++){
                 int sm=dSAMPmax[ch][ihit];
                 if(sm>400)break;
                 float a=dcht[ch][sm];

                 if((a>dcht[ch-2][sm]&&a>dcht[ch-2][sm-1]&&a>dcht[ch-2][sm+1])
                 &&(a>dcht[ch-1][sm]&&a>dcht[ch-1][sm-1]&&a>dcht[ch-1][sm+1])
                 &&(a>dcht[ch+2][sm]&&a>dcht[ch+2][sm-1]&&a>dcht[ch+2][sm+1])
                 &&(a>dcht[ch+1][sm]&&a>dcht[ch+1][sm-1]&&a>dcht[ch+1][sm+1])) { //2d max simple condition

                    dclustsmp[dnclust]=sm; //take the time at the maximum as cluster time
                    dclustchn[dnclust]=ch; //take the time at the maximum as cluster time
                    dclustlwd[dnclust]=dNSAMP[ch][ihit]; //number of samples for the maximum (longitudinal width)

                    int ch1=ch-20; //now sum all amplitudes around if > amax/5
                    if(ch1<0)ch1=0;
                    int ch2=ch+20;
                    if(ch2>239)ch2=239;
                    int chwid=0;
                    for (int ich=ch1;ich<ch2+1;ich++){
                        bool above_thresh=false;
                        for (int is=-10;is<11;is++){
                            float amp=dcht[ich][sm+is];
                            if(amp>thresh/2.){
                               dclustamp[dnclust]+=amp;
                               above_thresh=true;
                            }
                        }
                        if(above_thresh)dclusttwd[dnclust]++;
                    }
                    dnclust++;
                 } //end 2d max condition
             } //end hit loop
         } //end channel loop

// end Cell2 2d clustering

//
// pads
//

         int pwd[100];
         for (int ch=0;ch<100;ch++){
             pADCsumall[ch]=0.;
             pSmax[ch]=0;
             pAmax[ch]=0.;
             dAmin[ch]=5000.;
             pwd[ch]=0;
             for (int hit=0;hit<100;hit++){
                 pADCmax[ch][hit]=0.;
                 pSAMPmax[ch][hit]=0;
                 pNSAMP[ch][hit]=0;
                 pADCsum[ch][hit]=0.;
             }
         }

         for (int ch=0;ch<100;ch++){
             int pxch=ch/10;
             int pych=ch%10;
             //if(pxch>2&&pxch<7){
             //cout<<" ch====================="<<ch<<endl;
             int slot=GetPSlot(ch);
             int pch=GetPChan(ch);
             //cout<<"slot, pch="<<slot<<" "<<pch<<endl;
             pNhit[ch]=0;
             float adcmax=0.;
             for (int i=10;i<samples-10;i++){
                 adcval=ADCSamples[0][slot][pch][i]-ADCPedestal[0][slot][pch];
                 adcval1=ADCSamples[0][slot][pch][i-1]-ADCPedestal[0][slot][pch];
                 adcval2=ADCSamples[0][slot][pch][i+1]-ADCPedestal[0][slot][pch];
                 if(adcval<pAmin[ch])pAmin[ch]=adcval;
                 if(adcval>thresh/2.){
                    pADCsumall[ch]+=adcval;
                    pwd[ch]=pwd[ch]+1;
                    if(adcval>adcval1&&adcval>adcval2){
                       pADCmax[ch][pNhit[ch]]=adcval;
                       pSAMPmax[ch][pNhit[ch]]=i;
                       for (int is=-20;is<21;is++){
                           float adcl=ADCSamples[0][slot][pch][i+is]-ADCPedestal[0][slot][pch];
                           if(adcl>thresh/2.){
                              pADCsum[ch][pNhit[ch]]+=adcl;
                              pNSAMP[ch][pNhit[ch]]++;
                           }
                       }
                       pNhit[ch]=pNhit[ch]+1;
                       if(adcval>adcmax){
                          adcmax=adcval;
                          pAmax[ch]=adcval;
                          pSmax[ch]=i;
                          pped=ADCPedestal[0][slot][pch];
                       }
                    }
                 }
             }
         }

         pmcharge=0.;
         psmax=-100;
         pchmax=-100;
         pxchmax=-100.;
         pychmax=-100.;
         pcharge=0.;
         pwid=0.;
         pxwid=0.;
         pywid=0.;

         float pxyADC[10][10];
         for (int chx=0;chx<10;chx++){
             for (int chy=0;chy<10;chy++){
                  pxyADC[chx][chy]=0.;
             }
         }

         DHist->Reset();
         for (int ch=0;ch<100;ch++){
             int pxch=ch/10;
             int pych=ch%10;
             //if(pxch>2&&pxch<7){
             //cedi timing cut for noise rejection
             //cedi !!! padGEM timing cut  if(abs(pSmax[ch]-38)<3){
             if(pAmax[ch]>thresh){
                int px=ch/10;
                int py=ch%10;
                pxyADC[px][py]=pADCsumall[ch];
                //pxyADC[px][py]=pAmax[ch];
                DHist->SetBinContent(px,py,pADCsumall[ch]);
                DHist->SetBinError(px,py,1.);
                //DHist->Fill(px,py,pADCsumall[ch]);
                psize++;
                pcharge+=pADCsumall[ch];
                //cok including timing peaks pwid+=pwd[ch];
                pwid++;
             }
             if(pAmax[ch]>pmcharge){
                pmcharge=pAmax[ch];
                pchmax=ch;
                psmax=pSmax[ch];
                pxchmax=ch/10;
                pychmax=ch%10;
             }
         }

         pxcent=0.;
         float pxsum=0.;
         if(pxchmax>1&&pxchmax<9){
         for (int i=-2;i<3;i++){
             if(pxyADC[pxchmax+i][pychmax]>thresh)pxwid++;
             pxcent+=pxyADC[pxchmax+i][pychmax]*(pxchmax+i);
             pxsum+=pxyADC[pxchmax+i][pychmax];
         }
         if(pxsum>0)pxcent/=pxsum;
         }

         pycent=0.;
         float pysum=0.;
         if(pychmax>1&&pychmax<9){
            for (int i=-2;i<3;i++){
                if(pxyADC[pxchmax][pychmax+i]>thresh)pywid++;
                pycent+=pxyADC[pxchmax][pychmax+i]*(pychmax+i);
                pysum+=pxyADC[pxchmax][pychmax+i];
            }
            if(pysum>0)pycent/=pysum;
         }
         
/*
         for (int chx=0;chx<10;chx++){
         for (int chy=0;chy<10;chy++){
         }
         }

         pxfit=0.;
         pyfit=0.;
         pxsig=0.;
         pysig=0.;
         dgaus->SetParameters(100.,pxcent,2.,pycent,2.);
         if(pxwid>2&&pywid>2){
         dgaus->FixParameter(1,pxcent);
         dgaus->FixParameter(3,pycent);
         dgaus->SetParLimits(2,0.2,3.);
         dgaus->SetParLimits(4,0.2,3.);
         DHist->Fit(dgaus,"QS","0");
         dgaus->ReleaseParameter(1);
         dgaus->ReleaseParameter(3);
         DHist->Fit(dgaus,"QS","0");
         pxfit=dgaus->GetParameter(1);
         pyfit=dgaus->GetParameter(3);
         pxsig=dgaus->GetParameter(2);
         pysig=dgaus->GetParameter(4);
         }
*/

         umcharge=0.;
         usmax=-100;
         uchmax=-100;
         dmcharge=0.;
         dsmax=-100;
         //dchmax=-100;
         wmcharge=0.;
         wsmax=-100;
         pssmax=-100;
         wchmax=-100;
         //cold w2mcharge=0.;
         //cold w2smax=-100;
         //cold w2chmax=-100;
         pschmax=-100.;
         wsize=0;
         usize=0;
         dsize=0;
         psize=0;
         w2size=0;
         wcharge=0;
         w2charge=0;
         pscharge=0;
         ucharge=0;
         dcharge=0;
         wwid=0.;
         w2wid=0.;
         uwid=0.;
         dwid=0.;

//check
         for (int ch=0;ch<72;ch++){
             if(wAmax[ch]>thresh){
                wsize++;
                wcharge+=wADCsumall[ch];
                wwid+=wwd[ch];
             }
             if(wAmax[ch]>wmcharge){
                wmcharge=wAmax[ch];
                wchmax=ch;
                wsmax=wSmax[ch];
                //cok wwid=wwd[ch];
             }
         }
         for (int ch=0;ch<24;ch++){
             if(uAmax[ch]>thresh/18.){
                usize++;
                ucharge+=uADCsum[ch];
                uwid+=uwd[ch];
             }
             if(uAmax[ch]>umcharge){
                umcharge=uAmax[ch];
                uchmax=ch;
                usmax=uSmax[ch];
                //cok uwid=uwd[ch];
             }
         }

         for (int ch=0;ch<120;ch++){
             //cedi cut
             //if(abs(w2Smax[ch]-42.)<3){
             if(w2Amax[ch]>thresh){
                w2size++;
                w2charge+=w2ADCsumall[ch];
                //w2wid+=w2wd[ch];
             }
             if(w2Amax[ch]>w2mcharge&&ch>0&&ch<120){
                w2mcharge=w2Amax[ch];
                w2chmax=ch;
                w2smax=w2Smax[ch];
                //cok w2wid=w2wd[ch];
             }
          //}
         }
         for (int ch=0;ch<192;ch++){
             if(dAmax[ch]>thresh){
                dsize++;
                dcharge+=dADCsumall[ch];
                dwid+=dwd[ch];
             }
             if(dAmax[ch]>dmcharge){
                dmcharge=dAmax[ch];
// dchmax is the channel with the highest amplitude
// dsmax is the sample no of the channel with the highest amplitude
                dchmax=ch;
                dsmax=dSmax[ch];
                //cok dwid=dwd[ch];
             }
         }

// u-strips centroid
//
         
         if(1==1){
         umax=0.;
         for (int ch=0;ch<24;ch++){
             int slot; int ch0;
             slot=sl_c1u1; ch0=ch_c1u1;
             adcval=uADCmax[ch][0];
             //adcval=uADCsum[ch];
             if(adcval>umax){
                umax=adcval;
                uchmax=ch;
                usmax=uSAMPmax[ch][0];
                uped=ADCPedestal[0][slot][ch+ch0];
                //cok uwid=uwd[ch];
             }
         }

         wmax=0.;
         for (int ch=0;ch<72;ch++){
             int slot; int ch0;
             slot=sl_c1w1; ch0=ch_c1w1;
             adcval=wADCmax[ch][0];
             //adcval=wADCsum[ch];
             if(adcval>wmax){
                wmax=adcval;
                wchmax=ch;
                wsmax=wSAMPmax[ch][0];
                wped=ADCPedestal[0][slot][ch+ch0];
                //cok wwid=wwd[ch];
             }
         }

         ucharge=0.;
         umcharge=0.;
         ucent=0.;
         ufit=0.;
         umcent=0.;
         umfit=0.;

         int ch1=uchmax-3;
         if(ch1<0)ch1=0;
         int ch2=uchmax+3;
         if(ch2>23)ch2=23;

         usig=0.;
         usize=0;
         FHist->Reset();
         for (int ich=ch1;ich<ch2+1;ich++){
             adcval=uADCmax[ich][0];
             if(adcval>thresh){
                umcharge+=adcval;
                umcent+=(float)adcval*ich;
                FHist->Fill((double)ich-ch1,(double)adcval);
                usize++;
             }
         }

         umcent/=umcharge;
         centroid->SetParameter(0,(double)uADCmax[uchmax][0]);
         centroid->SetParameter(1,(double)2.);
         centroid->SetParameter(2,(double)1.);
         if(usize>2){
            TFitResultPtr res=FHist->Fit("centroid","QS");
            int fstatus=res;
            if(fstatus==0){
               umfit=(float)centroid->GetParameter(1)+(float)ch1;
               usig=abs((float)centroid->GetParameter(2));
               // umcharge=centroid->GetParameter(0)*usig*sqrt(2.*PI);
            }
         }

         usize=0;
         FHist->Reset();
         for (int ich=ch1;ich<ch2+1;ich++){
             adcval=uADCsum[ich];
             if(adcval>thresh/18.){
                ucharge+=adcval;
                ucent+=(float)adcval*ich;
                FHist->Fill((double)ich-ch1,(double)adcval);
                usize++;
             }
         }

         ucent/=ucharge;
         centroid->SetParameter(0,(double)uADCsum[uchmax]);
         centroid->SetParameter(1,(double)2.);
         centroid->SetParameter(2,(double)1.);
         if(usize>2){
            TFitResultPtr res=FHist->Fit("centroid","QS");
            int fstatus=res;
            if(fstatus==0){
               ufit=(float)centroid->GetParameter(1)+(float)ch1;
               // usig=abs((float)centroid->GetParameter(2));
               // ucharge=centroid->GetParameter(0)*usig*sqrt(2.*PI);
            }
         }

         if(wchmax>0&&uchmax>0){
            umax=0.;
            for (int ch=0;ch<24;ch++){
                int slot; int ch0;
                slot=sl_c1u1; ch0=ch_c1u1;
                adcval=uADCmax[ch][0];
                //adcval=uADCsum[ch];
                if(adcval>umax){
                   umax=adcval;
                   uchmax=ch;
                   usmax=uSAMPmax[ch][0];
                   uped=ADCPedestal[0][slot][ch+ch0];
                   //cok uwid=uwd[ch];
                }
            }

            wmax=0.;
            for (int ch=0;ch<72;ch++){
                int slot; int ch0;
                slot=sl_c1w1; ch0=ch_c1w1;
                adcval=wADCmax[ch][0];
                //adcval=wADCsum[ch];
                if(adcval>wmax){
                   wmax=adcval;
                   wchmax=ch;
                   wsmax=wSAMPmax[ch][0];
                   wped=ADCPedestal[0][slot][ch+ch0];
                   //cok wwid=wwd[ch];
                }
            }

         ucharge=0.;
         umcharge=0.;
         ucent=0.;
         ufit=0.;
         umcent=0.;
         umfit=0.;

         int ch1=uchmax-3;
         if(ch1<0)ch1=0;
         int ch2=uchmax+3;
         if(ch2>23)ch2=23;

         usig=0.;
         usize=0;
         FHist->Reset();
         for (int ich=ch1;ich<ch2+1;ich++){
             adcval=uADCmax[ich][0];
             if(adcval>thresh){
                umcharge+=adcval;
                umcent+=(float)adcval*ich;
                FHist->Fill((double)ich-ch1,(double)adcval);
                usize++;
             }
         }
         umcent/=umcharge;
         centroid->SetParameter(0,(double)uADCmax[uchmax][0]);
         centroid->SetParameter(1,(double)2.);
         centroid->SetParameter(2,(double)1.);
         if(usize>2){
            TFitResultPtr res=FHist->Fit("centroid","QS");
            int fstatus=res;
            if(fstatus==0){
               umfit=(float)centroid->GetParameter(1)+(float)ch1;
               usig=abs((float)centroid->GetParameter(2));
               // umcharge=centroid->GetParameter(0)*usig*sqrt(2.*PI);
            }
         }

         usize=0;
         FHist->Reset();
         for (int ich=ch1;ich<ch2+1;ich++){
             adcval=uADCsum[ich];
             if(adcval>thresh/18.){
                ucharge+=adcval;
                ucent+=(float)adcval*ich;
                FHist->Fill((double)ich-ch1,(double)adcval);
                usize++;
             }
         }

         ucent/=ucharge;
         centroid->SetParameter(0,(double)uADCsum[uchmax]);
         centroid->SetParameter(1,(double)2.);
         centroid->SetParameter(2,(double)1.);
         if(usize>2){
            TFitResultPtr res=FHist->Fit("centroid","QS");
            int fstatus=res;
            if(fstatus==0){
               ufit=(float)centroid->GetParameter(1)+(float)ch1;
               // usig=abs((float)centroid->GetParameter(2));
               // ucharge=centroid->GetParameter(0)*usig*sqrt(2.*PI);
            }
         }
         
         wcharge=0.;
         wmcharge=0.;
         wcent=0.;
         wfit=0.;
         wmcent=0.;
         wmfit=0.;
         wsig=0.;
         wsize=0;

         ch1=wchmax-3;
         if(ch1<0)ch1=0;
         ch2=wchmax+3;
         if(ch2>23)ch2=23;

         wsig=0.;
         wsize=0;
         FHist->Reset();
         for (int ich=ch1;ich<ch2+1;ich++){
             adcval=wADCmax[ich][0];
             if(adcval>thresh){
                wmcharge+=adcval;
                wmcent+=(float)adcval*ich;
                FHist->Fill((double)ich-ch1,(double)adcval);
                wsize++;
             }
         }

         wmcent/=wmcharge;
         centroid->SetParameter(0,(double)wADCmax[wchmax][0]);
         centroid->SetParameter(1,(double)2.);
         centroid->SetParameter(2,(double)1.);
         if(wsize>2){
            TFitResultPtr res=FHist->Fit("centroid","QS");
            int fstatus=res;
            if(fstatus==0){
               wmfit=(float)centroid->GetParameter(1)+(float)ch1;
               //cok wsig=abs((float)centroid->GetParameter(2));
               // wmcharge=centroid->GetParameter(0)*wsig*sqrt(2.*PI);
            }
         }

         wsize=0;
         FHist->Reset();
         for (int ich=ch1;ich<ch2+1;ich++){
             adcval=wADCsumall[ich];
             if(adcval>sthresh){
                wcharge+=adcval;
                wcent+=(float)adcval*ich;
                FHist->Fill((double)ich-ch1,(double)adcval);
                wsize++;
             }
         }

         wcent/=wcharge;
         centroid->SetParameter(0,(double)wADCsumall[wchmax]);
         centroid->SetParameter(1,(double)2.);
         centroid->SetParameter(2,(double)1.);
         if(wsize>2){
            TFitResultPtr res=FHist->Fit("centroid","QS");
            int fstatus=res;
            if(fstatus==0){
            wfit=centroid->GetParameter(1)+ch1;
            // wsig=abs(centroid->GetParameter(2));
            // wcharge=centroid->GetParameter(0)*wsig*sqrt(2.*PI);
            }
         }

         //OK for all
         }
///////////////////////

         wsig=0.;
         for (int ch=0;ch<24;ch++){
             if(abs(ch-wchmax)<2){
                wsig+=wNhit[ch];
             }
         }

         unhit=0;
         wnhit=0;
         wsize=0.;
         for (int ihit=0;ihit<200;ihit++){
             wthit[ihit]=-100.;
             wahit[ihit]=-100.;
             wmhit[ihit]=-100.;
             wchit[ihit]=-100.;
             uthit[ihit]=-100.;
             uahit[ihit]=-100.;
         }

         float tmax12=tmax1;
         if(tmax2>tmax1)tmax12=tmax2;
         tmax12=550.;
         if(wchmax>0){
            if(wmcharge>100.){
               unhit=uNhit[uchmax];
               for (int ihit=0;ihit<uNhit[uchmax];ihit++){
                   uthit[ihit]=uSAMPmax[uchmax][ihit];
                   uahit[ihit]=uADCmax[uchmax][ihit];
                   uAvsT->Fill((double)uSAMPmax[uchmax][ihit],(double)uADCmax[uchmax][ihit]); 
               }

               /// short mode
               for (int ch=0;ch<24;ch++){
                   int slot; int ch0;
                   slot=sl_c1w1; ch0=ch_c1w1;
                   if(abs(ch-wchmax)<2){
                      if(ch==wchmax)wsize=fADCnhit[0][slot][ch+ch0];
                      for (int ihit=0;ihit<fADCnhit[0][slot][ch+ch0];ihit++){
                          float adc_peak=fADCpeak[0][slot][ch+ch0][ihit]-fADCPedestal[0][slot][ch+ch0];
                          if(adc_peak>thresh){
                             wthit[wnhit]=fADCtime[0][slot][ch+ch0][ihit];
                             wmhit[wnhit++]=adc_peak;
                          }
                      }
                   }
               }
                  
               for (int iclust=0;iclust<wnclust;iclust++){
                   wAvsTnorm->Fill((double)wclustsmp[iclust],(double)wclustchn[iclust],(double)wclustamp[iclust]); 
                   //wAvsT->Fill((double)wclustsmp[iclust],(double)wclustchn[iclust],(double)wclustamp[iclust]); 
                   //wCvsT->Fill((double)wclustsmp[iclust],(double)wclustchn[iclust],(double)wclusttwd[iclust]); 
                   wCvsL->Fill((double)wclustsmp[iclust],(double)wclustchn[iclust],(double)wclustlwd[iclust]); 
               }
               for (int ch=0;ch<72;ch++){
                   for (int ihit=0;ihit<wNhit[ch]&&ihit<200;ihit++){
                       if(abs(ch-wchmax)<2){
                          //cok raw wthit[wnhit]=wSAMPmax[ch][ihit];
                          //cok raw wahit[wnhit]=wADCsum[ch][ihit];
                          //cok raw wmhit[wnhit++]=wADCmax[ch][ihit];
                          if(wnhit<999){
                             wchit[wnhit]=ch;
                             wthit[wnhit]=wSAMPmax[ch][ihit];
                             wahit[wnhit]=wADCsum[ch][ihit];
                             wmhit[wnhit++]=wADCmax[ch][ihit];
                          }
                          wAvsD->Fill((double)(wSAMPmax[ch][ihit]-48.)*dmax1/tmax1,(double)ch,(double)wADCsum[ch][ihit]); 
                          wAvsT->Fill((double)wSAMPmax[ch][ihit],(double)ch,(double)wADCsum[ch][ihit]); 
                          wCvsT->Fill((double)wSAMPmax[ch][ihit],(double)ch); 
                       } else {
                          wAvsB->Fill((double)wSAMPmax[ch][ihit],(double)ch,(double)wADCsum[ch][ihit]); 
                          wCvsB->Fill((double)wSAMPmax[ch][ihit],(double)ch); 
                          int fillok=1;
                          for (int chu=0;chu<24&&fillok;chu++){
                              for (int ihitu=0;ihitu<uNhit[chu]&&fillok;ihitu++){
                                  if(abs(uSAMPmax[chu][ihitu]-wSAMPmax[ch][ihit])<3){
                                     float y=-0.5*(chu-24.)/cos(sang)+1.*ch*tan(sang);
                                     if(wSAMPmax[ch][ihit]>tmax12+50.){
                                        wYvsB->Fill((double)ch,(double)y,(double)wADCmax[chu][ihitu]); 
                                     }
                                     //cock wTYvsB->Fill((double)wSAMPmax[ch][ihit],(double)y,(double)wADCsum[ch][ihit]); 
                                     wTYvsB->Fill((double)wSAMPmax[ch][ihit],(double)y,(double)uADCmax[chu][ihitu]); 
                                  }
                              }
                          }
                         }
                   }
               }
               print_flg=0;
            }
         }

         w2sig=0.;
         for (int ch=0;ch<120;ch++){
             if(abs(ch-wchmax)<10){
                w2sig+=w2Nhit[ch];
             }
         }

//here
         dnhit=0;
         w2nhit=0;
         w2size=0.;
         for (int ihit=0;ihit<200;ihit++){
             w2thit[ihit]=-100.;
             w2ahit[ihit]=-100.;
             w2mhit[ihit]=-100.;
             w2chit[ihit]=-100.;
             dthit[ihit]=-100.;
             dahit[ihit]=-100.;
         }

         tmax12=tmax1;
         if(tmax2>tmax1)tmax12=tmax2;
         tmax12=550.;
         //if(w2chmax>0&&abs(wcent-w2chmax/25-4.15)<20)
         if(w2chmax>0){
            if(w2mcharge>100.){
               // short mode
               for (int ch=0;ch<24;ch++){
                   int slot; int ch0;
                   slot=sl_c1w1; ch0=ch_c1w1;
                   if(abs(ch-wchmax)<2){
                      if(ch==wchmax)wsize=fADCnhit[0][slot][ch+ch0];
                      for (int ihit=0;ihit<fADCnhit[0][slot][ch+ch0];ihit++){
                          float adc_peak=fADCpeak[0][slot][ch+ch0][ihit]-fADCPedestal[0][slot][ch+ch0];
                          if(adc_peak>thresh){
                             wthit[wnhit]=fADCtime[0][slot][ch+ch0][ihit];
                             wmhit[wnhit++]=adc_peak;
                          }
                      }
                   }
               }
                  
               for (int iclust=0;iclust<w2nclust;iclust++){
                   w2AvsTnorm->Fill((double)w2clustsmp[iclust],(double)w2clustchn[iclust],(double)w2clustamp[iclust]); 
                   //w2AvsT->Fill((double)w2clustsmp[iclust],(double)w2clustchn[iclust],(double)w2clustamp[iclust]); 
                   //w2CvsT->Fill((double)w2clustsmp[iclust],(double)w2clustchn[iclust],(double)w2clusttwd[iclust]); 
                   w2CvsL->Fill((double)w2clustsmp[iclust],(double)w2clustchn[iclust],(double)w2clustlwd[iclust]); 
               }
               for (int ch=0;ch<120;ch++){
                   for (int ihit=0;ihit<w2Nhit[ch]&&ihit<200;ihit++){
                       //cold if(abs(ch-w2chmax)<10&&abs(w2charge/dcharge-0.8)<0.4&&dcharge>100000)
                       if(abs(ch-w2chmax)<10){
                          //cok raw w2thit[w2nhit]=w2SAMPmax[ch][ihit];
                          //cok raw w2ahit[w2nhit]=w2ADCsum[ch][ihit];
                          //cok raw w2mhit[w2nhit++]=w2ADCmax[ch][ihit];
                          if(w2nhit<999){
                             w2chit[w2nhit]=ch;
                             w2thit[w2nhit]=w2SAMPmax[ch][ihit];
                             w2ahit[w2nhit]=w2ADCsum[ch][ihit];
                             w2mhit[w2nhit++]=w2ADCmax[ch][ihit];
                          }
                          w2AvsD->Fill((double)(w2SAMPmax[ch][ihit]-48.)*dmax1/tmax1,(double)ch,(double)w2ADCsum[ch][ihit]); 
                          w2AvsT->Fill((double)w2SAMPmax[ch][ihit],(double)ch,(double)w2ADCsum[ch][ihit]); 
                          w2CvsT->Fill((double)w2SAMPmax[ch][ihit],(double)ch);
                     } else {
                          w2AvsB->Fill((double)w2SAMPmax[ch][ihit],(double)ch,(double)w2ADCsum[ch][ihit]); 
                          w2CvsB->Fill((double)w2SAMPmax[ch][ihit],(double)ch); 
                          int fillok=1;
                          for (int chd=0;chd<192&&fillok;chd++){
                              for (int ihitd=0;ihitd<dNhit[chd]&&fillok;ihitd++){
                                  if(abs(dSAMPmax[chd][ihitd]-w2SAMPmax[ch][ihit])<3){
                                     float y=chd;
                                     if(w2SAMPmax[ch][ihit]>tmax12+50.){
                                        w2YvsB->Fill((double)ch,(double)y,(double)w2ADCmax[chd][ihitd]); 
                                     }
                                     //cock wTYvsB->Fill((double)wSAMPmax[ch][ihit],(double)y,(double)wADCsum[ch][ihit]); 
                                     w2TYvsB->Fill((double)w2SAMPmax[ch][ihit],(double)y,(double)dADCmax[chd][ihitd]); 
                                  }
                              }
                          }
                       }
                   }
               }
               print_flg=0;    
            }
         }

         dnhit=0;
         dsize=0.;
         for (int ihit=0;ihit<200;ihit++){
             dthit[ihit]=-100.;
             dahit[ihit]=-100.;
             dmhit[ihit]=-100.;
             dchit[ihit]=-100.;
         }

         tmax12=tmax1;
         if(tmax2>tmax1)tmax12=tmax2;
         tmax12=550.;
         if(dchmax>0){
            if(dmcharge>100.){
               dnhit=dNhit[uchmax];
               for (int ihit=0;ihit<dNhit[dchmax];ihit++){
                   dthit[ihit]=dSAMPmax[dchmax][ihit];
                   dahit[ihit]=dADCmax[dchmax][ihit];
                   dAvsT->Fill((double)dSAMPmax[dchmax][ihit],(double)dADCmax[dchmax][ihit]); 
               }
                  
               for (int iclust=0;iclust<dnclust;iclust++){
                   dAvsTnorm->Fill((double)dclustsmp[iclust],(double)dclustchn[iclust],(double)dclustamp[iclust]); 
                   //w2AvsT->Fill((double)w2clustsmp[iclust],(double)w2clustchn[iclust],(double)w2clustamp[iclust]); 
                   //w2CvsT->Fill((double)w2clustsmp[iclust],(double)w2clustchn[iclust],(double)w2clusttwd[iclust]); 
                   dCvsL->Fill((double)dclustsmp[iclust],(double)dclustchn[iclust],(double)dclustlwd[iclust]); 
               }
               for (int ch=0;ch<192;ch++){
                   for (int ihit=0;ihit<dNhit[ch]&&ihit<200;ihit++){
                       if(abs(ch-dchmax)<10){
                          if(dnhit<999){
                             dchit[dnhit]=ch;
                             dthit[dnhit]=dSAMPmax[ch][ihit];
                             dahit[dnhit]=dADCsum[ch][ihit];
                             dmhit[dnhit++]=dADCmax[ch][ihit];
                          }
                          dAvsD->Fill((double)(dSAMPmax[ch][ihit]-48.)*dmax1/tmax1,(double)ch,(double)dADCsum[ch][ihit]); 
                          dAvsT->Fill((double)dSAMPmax[ch][ihit],(double)ch,(double)dADCsum[ch][ihit]); 
                          dCvsT->Fill((double)dSAMPmax[ch][ihit],(double)ch); 
                       } else {
                           dAvsB->Fill((double)dSAMPmax[ch][ihit],(double)ch,(double)dADCsum[ch][ihit]); 
                           dCvsB->Fill((double)dSAMPmax[ch][ihit],(double)ch); 
                     
                         }
                   }
               }
               print_flg=0;
            }
         }

         if(wnclust>0)wwid=wclusttwd[0];
         if(wnclust>0)wsig=wclustlwd[0];
         wsig=wnclust;
         if(w2nclust>0)w2wid=w2clusttwd[0];
         if(w2nclust>0)w2sig=w2clustlwd[0];
         w2sig=w2nclust;

///
         int isamp0=35;
         int nsamp=72-isamp0;
         bool found=false;

         if(wchmax<12&&wnhit>0){
            to_sergey_rad<<endl<<" "<<nsamp<<endl;
            for (int isamp=isamp0;isamp<72;isamp++){ 
                found=false;
                for (int ihit=0;ihit<wnhit;ihit++){
                    if(isamp==wthit[ihit]){
                       to_sergey_rad<<wahit[ihit]<<" ";
                       found=true;
                       break;
                    } 
                }
                if(!found)  to_sergey_rad<<0.<<" ";
            }
            to_sergey_rad<<endl;
         } else if (wchmax>12&&wnhit>0){
            to_sergey_norad<<endl<<" "<<nsamp<<endl;
            for (int isamp=isamp0;isamp<72;isamp++){
                found=false;
                for (int ihit=0;ihit<wnhit;ihit++){
                    if(isamp==wthit[ihit]){
                       to_sergey_norad<<wahit[ihit]<<" ";
                       found=true;
                       break;
                    } 
                }
                if(!found)  to_sergey_norad<<0.<<" ";
            }
            to_sergey_norad<<endl;
           }

///
// to sergey raw energy deposition
        
           //cout<<"wchmax, wAmax[wchmax],wSmax[wchmax]"<<wchmax<<" "<<wAmax[wchmax]<<" "<<wSmax[wchmax]<<endl;
           int slot; int ch0;
           slot=sl_c1w1; ch0=ch_c1w1;
           //if(wnhit>0&&wAmax[wchmax]>300.&&abs(ADCPedestal[0][slot][ch0+wchmax]-100.)<10.)
           if(wnhit>0&&wAmax[wchmax]>200.){
              int isamp0=40;
              int isamp1=340;
              int nsamp=isamp1-isamp0;
              float dedx[400];
              for (int i=0;i<samples;i++){
                  dedx[i]=0.;
              }
              for (int ch=0;ch<24;ch++){
                  //if(abs(ch-wchmax)<2&&wAmax[ch]>thresh&&abs(ADCPedestal[0][slot][ch0+ch]-100.)<10.)
                  if(abs(ch-wchmax)<2&&wAmax[ch]>thresh){
                     for (int i=0;i<samples;i++){
                         dedx[i]+=ADCSamples[0][slot][ch+ch0][i]-ADCPedestal[0][slot][ch+ch0];
                     }
                  }
              }
              if(wchmax>13&&wchmax<23) {
                 to_sergey_rad<<nsamp<<" "<<evtCount<<" "<<wchmax<<endl;
                 for (int i=isamp0;i<isamp1;i++){
                     to_sergey_rad<<dedx[i]<<" "; 
                     //cout<<dedx[i]<<" ";
                 }
                 to_sergey_rad<<endl;
                 //cout<<endl;
              } else if (wchmax>1&&wchmax<11) {
                 to_sergey_norad<<nsamp<<" "<<evtCount<<" "<<wchmax<<endl;
                 for (int i=isamp0;i<isamp1;i++){
                     to_sergey_norad<<dedx[i]<<" "; 
                 }
                 to_sergey_norad<<endl;
                }
           }
             
// end  to sergey         
///
// to sergey GEM raw energy deposition
        
           //cout<<"wchmax, wAmax[wchmax],wSmax[wchmax]"<<wchmax<<" "<<wAmax[wchmax]<<" "<<wSmax[wchmax]<<endl;
           //if(wnhit>0&&wAmax[wchmax]>300.&&abs(ADCPedestal[0][slot][ch0+wchmax]-100.)<10.)
           if(wnhit>0&&wAmax[wchmax]>300.&&w2nhit>0&&w2Amax[w2chmax]>300&&abs((w2chmax)/25.-(wchmax)+5.)<1.){
              int isamp0=50;
              int isamp1=170;
              int nsamp=isamp1-isamp0;
              float dedx[200];
              for (int i=0;i<samples;i++){
                  dedx[i]=0.;
              }
              for (int ch=0;ch<240;ch++){
                  int slot=GetXSlot(ch);
                  int gemch=GetXChan(ch);
                  if(abs(ch-w2chmax)<5&&w2Amax[ch]>thresh&&abs(ADCPedestal[0][slot][gemch]-100.)<10.){
                     for (int i=0;i<samples;i++){
                         dedx[i]+=ADCSamples[0][slot][gemch][i]-ADCPedestal[0][slot][gemch];
                     }
                  }
              }
              if(w2chmax<221&&w2chmax>188) {
                 to_sergey_rad2<<nsamp<<" "<<evtCount<<" "<<w2chmax<<endl;
                 for (int i=isamp0;i<isamp1;i++){
                     to_sergey_rad2<<dedx[i]<<" "; 
                     //cout<<dedx[i]<<" ";
                 }
                 to_sergey_rad2<<endl;
                 //cout<<endl;
              } else if (w2chmax<119&&w2chmax>86) {
                 to_sergey_norad2<<nsamp<<" "<<evtCount<<" "<<w2chmax<<endl;
                 for (int i=isamp0;i<isamp1;i++){
                     to_sergey_norad2<<dedx[i]<<" "; 
                 }
                 to_sergey_norad2<<endl;
                }
            }
             
// end  to sergey GEM raw energy deposition        
/// 
// to sergey GEM clusters
        
            if(w2nclust>0){
               to_sergey_rad<<w2nclust<<endl;
               for (int i=0;i<w2nclust;i++){
                   to_sergey_rad<<w2clustchn[i]<<" "; 
                   to_sergey_rad<<w2clustsmp[i]<<" "; 
                   to_sergey_rad<<w2clustamp[i]<<" "; 
                   to_sergey_rad<<w2clusttwd[i]<<" "; 
                   to_sergey_rad<<w2clustlwd[i]<<" "; 
               }
               to_sergey_rad<<endl;
            }
             
// end to sergey GEM clusters
///

       wcent=wchmax+(wchmax-w2chmax/25-4.15>0)*(0.2-sqrt(0.04+98*(wsmax-53)))/98
                     +(wchmax-w2chmax/25-4.15<=0)*(0.2+sqrt(0.04+98*(wsmax-53)))/98;
       //cokcok fdcFeTree->Fill();
       fdcFeTree->Fill();
      
       //cout<<" displaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaayyyyyyyyyyyyy="<<DISPLAY<<endl;
       //cout<<"ucharge/dcharge="<<ucharge/dcharge<<endl;
       //cout<<"ucharge="<<ucharge<<endl;
       //cout<<"ucent-uch="<<ucent-uchmax<<endl;
       //cout<<"usize,dsize="<<usize<<" "<<dsize<<endl;

//return 0;
///
       if(DISPLAY){
          pad_plot->Reset();
          ct_plot->Reset();
          wct_plot->Reset();
          cty_plot->Reset();
          wcty_plot->Reset();
          DISPLAY=1;
          //if(wmcharge>0)ratio=umcharge/wmcharge;
          int OKp=0;
          int OKu=0;
          int OKd=0;
          int OKw2=0;
          int OKw=0;
          wchmax=0;
          if(wchmax!=11111){
             TMultiGraph *mg = new TMultiGraph("mg","f125 samples");
             TGraph *graf1[48];
             TGraph *graf2[240];
             float Xaxis[400];
             float Yuaxis[24][400];
             float Ydaxis[48][400];
             float Yw2axis1[240][400];
             float Yw2axis[240][200];
             float Yw2axis2[240][400];
             float Ypaxis[100][200];
             float Ywaxis[24][400];
             int smp1=0;
             int smp2=samples;
             smp1=0;
             smp2=200;
             int ns=smp2-smp1;
             for (Int_t i=smp1;i<smp2;i++){
                 Xaxis[i-smp1] = i;
             }

          int okdisp=0;
          int ucol=6;
          int wcol=1;
          int uchlast=-1;
          int wchlast=-1;
          int uslast=-1;
          int wslast=-1;
          int slot;
          int ch0;

          for (Int_t i=smp1;i<smp2;i++){
// w1
              slot=sl_c1w1; ch0=ch_c1w1;
              for (Int_t ch=0;ch<24;ch++){ 
                  if(wAmax[ch]>thresh){
                     Ywaxis[ch][i-smp1] = ADCSamples[0][slot][ch+ch0][i]-ADCPedestal[0][slot][ch+ch0];
                     if(Ywaxis[ch][i-smp1]>thresh/3.)wct_plot->Fill((float)(ch+wchoffset)*10.,(float)(i-smp1)/1.,Ywaxis[ch][i-smp1]);
                     OKw=1;
                  }
              } //end ch loop
// u
              slot=sl_c1u1; ch0=ch_c1u1;
              for (Int_t ch=0;ch<24;ch++){ 
                  if(uAmax[ch]>thresh/1.){
                     Yuaxis[ch][i-smp1] = ADCSamples[0][slot][ch+ch0][i]-ADCPedestal[0][slot][ch+ch0];
                     if(Yuaxis[ch][i-smp1]>thresh/1.)wcty_plot->Fill((float)(ch+uchoffset)*5.,(float)(i-smp1)/1.,Yuaxis[ch][i-smp1]);
                     OKu=1;
                  }
              } //end ch loop
// d
             for (Int_t ch=0;ch<240;ch++){ 
                 if(dAmax[ch]>thresh){
                    int slot=GetYSlot(ch);
                    int k=GetYChan(ch);
                    Ydaxis[ch][i-smp1] = ADCSamples[0][slot][k][i]-ADCPedestal[0][slot][k];
                    if(Ydaxis[ch][i-smp1]>thresh/1.)cty_plot->Fill((float)(ch+dchoffset)*0.4,(float)(i-smp1)/1.,Ydaxis[ch][i-smp1]);
                    OKd=1;
                 }
             } //end ch loop
// w2
             for (Int_t ch=0;ch<240;ch++){ 
                 if(w2Amax[ch]>thresh){
                    if(w2Amax[ch]>200.)crate->Fill((float)ch,w2Amax[ch]);
                    int slot=GetXSlot(ch);
                    int k=GetXChan(ch);
                    Yw2axis[ch][i-smp1] = ADCSamples[0][slot][k][i]-ADCPedestal[0][slot][k];
                    if(Yw2axis[ch][i-smp1]>thresh)ct_plot->Fill((float)(ch+w2choffset)*0.4,(float)(i-smp1)/1.,Yw2axis[ch][i-smp1]);
                    OKw2=1;
                 } 
             } //end ch loop
// pad
             for (Int_t ch=0;ch<100;ch++){ 
                 if(pAmax[ch]>thresh/2.){
                    int slot=GetPSlot(ch);
                    int k=GetPChan(ch);
                    float padc = ADCSamples[0][slot][k][i]-ADCPedestal[0][slot][k];
                    Ypaxis[ch][i-smp1] = padc;
                    int pxch=ch/10;
                    int pych=ch%10;
                    //cokcok DIRC if(padc>thresh/2.)pad_plot->Fill((float)((pxch+5)%10),(float)(pych),padc);
                    if(padc>thresh/2.&&abs(i-42.5)<4)pad_plot->Fill((float)(pych),(float)(pxch),padc);
                    //PS if(padc>thresh/2.&&abs(pSmax[ch]-45.)<15.)pad_plot->Fill((float)(pych),(float)(pxch),padc);
                    //if(padc>thresh/2.)pad_plot->Fill((float)(pychmax),(float)(pxchmax),padc);
                    //if(padc>thresh/2.)pad_plot->Fill((float)(pxch),(float)(pych),padc);
                    OKp=1;
                 } 
             } //end ch loop
          } //end sample loop

          if(OKp){
             bool above_thresh=false;
             int k=0;
             for (int k=0;k<100;k++){
                 if(pAmax[k]>thresh/2.){      
                    cout<<" chan ========================== "<<k<<" "<<pAmax[k]<<" col= "<<wcol<<endl;
                    graf2[k] = new TGraph(ns,Xaxis,Ypaxis[k]);
                    graf2[k]->SetMarkerStyle(1);
                    graf2[k]->SetLineStyle(1);
                    graf2[k]->SetMarkerColor(wcol);
                    graf2[k]->SetLineColor(wcol);
                    mg->Add(graf2[k]);
                    wcol++;
                    above_thresh=true;
                 } 
             } 
          }
          cout<<endl;

          if(OKw){
             int k=0;
             for (int k=0;k<24;k++){
                 if(wAmax[k]>thresh){
                    //wcol=2;
                    //if(k<12)wcol=1; 
                    graf2[k] = new TGraph(ns,Xaxis,Ywaxis[k]);
                    graf2[k]->SetMarkerStyle(1);
                    graf2[k]->SetLineStyle(1);
                    graf2[k]->SetMarkerColor(wcol);
                    graf2[k]->SetLineColor(wcol);
                    //cok mg->Add(graf2[k]);
                    //cok wcol++;
                 } 
             } 
          } 

          if(OKu){
             int k=0;
             for (int k=0;k<48;k++){
                 if(uAmax[k]>thresh/2.){
                    //cout<<" cathodes ch,dAmax,dSmax, ucol="<<k<<" "<<dAmax[k]<<" "<<dSmax[k]<<" "<<ucol<<endl; 
                    ucol=4;
                    graf1[k] = new TGraph(ns,Xaxis,Yuaxis[k]);
                    graf1[k]->SetMarkerStyle(1);
                    graf1[k]->SetLineStyle(1);
                    graf1[k]->SetMarkerColor(ucol);
                    graf1[k]->SetLineColor(ucol);
                    //cok mg->Add(graf1[k]);
                    //cok  ucol++;
                    OKu=1;
                 } 
             } 
          } 
            
         //    GEM-TRD signals:
         /*
              if(OKw2){
              bool above_thresh=false;
              int k=0;
              for (int k=0;k<240;k++){
              //for (int k=0;k<240;k+=12){
              //if(w2Amax[k]>thresh){
              if(w2Amax[k]>thresh){
              
              cout<<" chan ========================== "<<k<<" col= "<<wcol<<endl;
              graf2[k] = new TGraph(ns,Xaxis,Yw2axis[k]);
              graf2[k]->SetMarkerStyle(1);
              graf2[k]->SetLineStyle(1);
              graf2[k]->SetMarkerColor(wcol);
              graf2[k]->SetLineColor(wcol);
              mg->Add(graf2[k]);
              wcol++;
              above_thresh=true;
              } 
              } 
      //cok mg->Draw("alp");
      //cokcok     if(above_thresh){
      //     mg->Draw("alp");
      //     myc->Update();
      //Int_t nexti=0;
      //cin>>nexti;
      //cokcok      }

              } 
           */

           //   
	    
          /*cok for GEM and WC if(OKw&&OKw2)
          //if(OKw&&OKw2)
          //if(OKw2)
          //for PS arm if(OKw&&OKw2&&abs(w2chmax*0.4+wchmax*10.-136.)<5.){
          //if(OKw&&OKw2&&abs(w2chmax*0.4+wchmax*10.-136.)<5.){
          //if(abs(w2chmax*0.04+wchmax-13.7)<3.&&wchmax>6&&wchmax<14&&abs(wchmax+pxchmax-16)<1.&&dchmax>=0&&pychmax>0&&pychmax<9&&pxchmax>0&&pxchmax<9)
          //if(OKw&&OKw2&&pchmax>=0&&uchmax>=0&&OKu>0&&abs(usmax-45)<15.&&abs(psmax-45)<15.&&dchmax<192)
          */
       if(OKw2>0&&OKd>0){
          cout<<"pych,w2ch,dch="<<pychmax<<" "<<w2chmax<<" "<<dchmax<<endl;
          cout<<"pxch,w2ch,dch="<<pxchmax<<" "<<w2chmax<<" "<<dchmax<<endl;
          //cout<<"pxch,uch,w2ch="<<pxchmax<<" "<<uchmax<<" "<<w2chmax<<endl;
          cout<<" delta dch-w2ch = "<<dchmax*0.04-w2chmax*0.04<<endl;
          cout<<" delta pxch-w2ch = "<<pxchmax-w2chmax*0.04<<endl;
          cout<<" delta pych-w2ch = "<<pychmax-w2chmax*0.04<<endl;

/*
          cout<<" D I R C "<<endl;
          cout<<endl;
          cout<<" v e r t i c a l "<<endl;
          cout<<" delta pxch-w2ch = "<<pxchmax+w2chmax*0.04<<endl;
          //cout<<" pxch = "<<(int)(-w2chmax*0.04+8.)<<endl;
          cout<<" pxch = "<<(int)(-w2chmax*0.04+8.)<<endl;
          cout<<" delta ucent-w2ch = "<<ucent*0.5+w2chmax*0.04-9.<<endl;
          cout<<endl;
          cout<<"  h o r i z o n t a l: "<<endl;
          cout<<" delta dch-wch = "<<dchmax*0.04-wchmax+7.8<<endl;
          cout<<" delta pych-dch = "<<floor((pchmax+8)%10)+dchmax*0.04-8.5<<endl;
          cout<<" pych= "<<(int)(-dchmax*0.04+8.5)<<endl;
          cout<<endl;
          cout<<endl;
          cout<<" PS "<<endl;
          cout<<endl;
          cout<<" v e r t i c a l "<<endl;
          cout<<" delta ucent-pych = "<<ucent*0.5-pychmax+2.<<endl;
          cout<<" pych= "<<ucent*0.5+2.<<endl;
          cout<<" delta ucent-dch = "<<ucent*0.5-dchmax*0.04-0.4<<endl;
          cout<<endl;
          cout<<"  h o r i z o n t a l: "<<endl;
          cout<<" delta dch-wch = "<<w2chmax*0.04+wchmax-13.7<<endl;
          cout<<" delta wch-pxch = "<<wchmax+pxchmax-16.<<endl;
          cout<<" pxch= "<<-wchmax+16.<<endl;
          cout<<endl;
          cout<<endl;
*/
          //494 if(OKw&&OKw2&&abs(wcent-w2chmax/25-4.15)<20.)

      TCanvas *myc;
      myc = new TCanvas("myc", "Event", 1000, 800);
      myc->Draw();
      myc->Clear("D");
      myc->Divide(3,2);
      //myc->Divide(1,2);
      //??? cout << " event = " << ev << endl;
      //myc->SetFillColor(11);
      //TPad *p1 = (TPad*)(myc->cd(1));
      //p1->SetLogz();
      //TPad *p2 = (TPad*)(myc->cd(2));
      //p2->SetLogz();
      
      myc->cd(1);
      gPad->SetGrid();
      gPad->SetLeftMargin(0.08);
      gPad->SetRightMargin(0.);
      gPad->SetTopMargin(0.);
      gPad->SetBottomMargin(0.08);
      gPad->Modified();
      mg->SetMaximum(1000.);
      mg->SetMinimum(-50.);
      //cok mg->Draw("alp");
      mg->Draw("alp");

      myc->cd(4);
      gPad->SetGrid();
      gPad->SetLeftMargin(0.08);
      //gPad->SetRightMargin(0.);
      gPad->SetTopMargin(0.);
      gPad->SetBottomMargin(0.08);
      gPad->Modified();
      pad_plot->Draw("colz");

      myc->cd(2);
      gPad->SetGrid();
      gPad->SetLeftMargin(0.08);
      gPad->SetRightMargin(0.);
      gPad->SetTopMargin(0.);
      gPad->SetBottomMargin(0.08);
      gPad->Modified();
      wct_plot->Draw("col");

      myc->cd(5);
      gPad->SetGrid();
      gPad->SetLeftMargin(0.08);
      gPad->SetRightMargin(0.);
      gPad->SetTopMargin(0.);
      gPad->SetBottomMargin(0.08);
      gPad->Modified();
      wcty_plot->Draw("col");
      wcty_plot->Draw("col");

      myc->cd(3);
      gPad->SetGrid();
      gPad->SetLeftMargin(0.08);
      gPad->SetRightMargin(0.);
      gPad->SetTopMargin(0.);
      gPad->SetBottomMargin(0.08);
      gPad->Modified();
      ct_plot->Draw("col");
      //cty_plot->Draw("col");

      myc->cd(6);
      gPad->SetGrid();
      gPad->SetLeftMargin(0.08);
      gPad->SetRightMargin(0.);
      gPad->SetTopMargin(0.);
      gPad->SetBottomMargin(0.08);
      gPad->Modified();
      cty_plot->Draw("col");
      //ct_plot->Draw("col");

      myc->Update();
      //theApp.Run();
      Int_t nexti=0;
      cin>>nexti;
      if(nexti==9) myc->Print("myc.pdf");
      if(nexti==9) myc->Print("myc.root");
   }
       	
        } // end display condition
	} // end display    
//return 0;
        } //end samples>0 
        } //end NEVENT
    
        } catch (evioException e) {
            cerr << "Event: "<<evtCount<<"  exception at eventTree()"<<endl;
            cerr << endl << e.toString() << endl << endl << "Try to continue" <<endl;
          }

          ReadBack = EvOchan.read();

    } //end of if event=< loop 
    } //end of event loop 
      
    to_sergey_rad.close();
    to_sergey_norad.close();

    ROOTfile->cd();
    uAvsT->Write();
    dAvsT->Write();
    wAvsT->Write();
    crate->Write();
    ROOTfile->Write();
  } //1==2
  } //OK
  } // end file loop
    ROOTfile->Close();
    delete ROOTfile;
    cout<<"Closing root file "<<root_fname<<endl;
  
  TGraphErrors *graph;
  char t1[128];
  char t2[128];
  sprintf(t1,"graph%d_%d_%d",ROC,SLOT,CHANNEL);
  sprintf(t2,"Pedestals ROC %d SLOT %d CHANNEL %d",ROC,SLOT,CHANNEL);
  graph = new TGraphErrors(NCon,x,y,NULL,dy);
  graph->SetTitle(t2);
  graph->SetMarkerColor(4);
  graph->SetMarkerStyle(21);
  graph->GetYaxis()->SetTitle("Pedestal [ADC counts]"); 
  
  if(DISPLAY){
     TCanvas *myc1;
     char str1[128];
     sprintf(str1,"Pedestals ROC %d ADC125 Samples in SLOT %d CHANNEL %d",ROC,SLOT,CHANNEL);
     myc1 = new TCanvas("myc1", str1, 800, 400);
     myc1->SetFillColor(42);
     gPad->SetGrid();
     //cok graph->Draw("AP");
     graph->Draw("AP");
     myc1->Update();

     cout<<"Continue: ";
     char inp[128];
     cin>>inp; 
  }

/*
  TFile *fout;
  sprintf(fnam,"%s/PedestalsROC%dSLOT%dCHANNEL%d.root",DataDir,ROC,SLOT,CHANNEL);
  fout = new TFile(fnam,"RECREATE");
  graph->Write();

  fout->Close();
*/

  return 0;
  
} //end main


void analyzeEvent(evioDOMTree &eventTree) {

  //cout << endl << endl << endl << "analyzing event " << evtCount << endl << endl;
  // get list of all banks in event
  evioDOMNodeListP bankList2 = eventTree.getNodeList(typeIs<uint64_t>());
  evioDOMNodeList::iterator iter = bankList2->begin();

  const uint64_t *run_number_and_type = NULL;

  for (; iter!=bankList2->end(); iter++){
      evioDOMNodeP bankPtr = *iter;
      evioDOMNodeP physics_event_built_trigger_bank = bankPtr->getParent();
      if(physics_event_built_trigger_bank == NULL) continue;
      uint32_t tag = physics_event_built_trigger_bank->tag;
      const vector<uint64_t> *vec;
      switch(tag){
        case 0xFF22:
        case 0xFF23:
        case 0xFF26:
        case 0xFF27:
        vec = bankPtr->getVector<uint64_t>();
        if(!vec) continue;
        if(vec->size()<1) continue;
        run_number_and_type = &((*vec)[vec->size()-1]);
        break;
      }
      if(run_number_and_type != NULL){
         TheRunNumber =  ((*vec)[vec->size()-1])>>32;
         EVENTNUMBER = ((*vec)[0]);
         int MEVENTS = vec->size()-2;
         NEventsInBlock = MEVENTS ;
         TheEventNum = EVENTNUMBER;
         EVTCounts += MEVENTS;
      }
  }

  evioDOMNodeListP bankList = eventTree.getNodeList(typeIs<uint32_t>());
  evioDOMNodeList::iterator iterX = bankList->begin();

  for (; iterX!=bankList->end(); iterX++){
      evioDOMNodeP bankPtr = *iterX;
      evioDOMNodeP physics_event_built_trigger_bank = bankPtr->getParent();
      if(physics_event_built_trigger_bank == NULL) continue;
      if(bankPtr->tag==1){
         const vector<uint32_t> *vec;
         vec = bankPtr->getVector<uint32_t>();
         int NEV = vec->size()/4;
         if(NEventsInBlock != NEV){
            cout<<"Error Event Number Miss Match In Block "<<endl;
            cout<<"Expected: "<<NEventsInBlock<<"    found: "<<NEV<<endl;
         }
         for (int k=0; k<NEV; k++){
             uint64_t hi_t = (*vec)[k*4];
             uint64_t lo_t = (*vec)[k*4+1];
             TRIGGERTIMES = hi_t + (lo_t<<32);
             TRIGGER_MASK_GT = (*vec)[k*4+2];
             TRIGGER_MASK_FP = (*vec)[k*4+3];
             //cout<<" count triggers "<<endl;
             CountTriggers();
         }
      }
  }
  
  
  // analyze each bank
  for_each(bankList->begin(),bankList->end(),analyzeBank);
}

void CountTriggers(){
   int bit1[100];
   int NBits1 = 0;
   int bit1_S[100];
   int NBits1_S = 0;
   if(TRIGGER_MASK_GT>0){
      unsigned int val = TRIGGER_MASK_GT;
      for (int nn=0; nn<32; nn++){
          bit1[nn] = -1;
          if(val & (1<<nn)){
             //cout<<" trigger bit = "<<nn<<endl;
             bit1[NBits1] = nn;
             NBits1++;
          }
          bit1_S[nn] = -1;
          if(val == (uint32_t) (1<<nn) ){
             bit1_S[NBits1_S] = nn;
             NBits1_S++;
          }
      }
   }
   int bit2[100];
   int NBits2 = 0;
   if(TRIGGER_MASK_FP>0){
      unsigned int val = TRIGGER_MASK_FP;
      for (int nn=0; nn<32; nn++){
          bit2[nn] = -1;
          if(val & (1<<nn)){
             bit2[NBits2] = nn;
             NBits2++;
          }
      }
   }

  if (NBits1){ // GTP triggers
  }
  if (NBits1_S){ // SINGLE GTP Trigger
  }
  if (NBits2){ // FP TRIGGER
  }

}

void analyzeBank(evioDOMNodeP bankPtr) { 
  
  // dump bank info for all banks
  //   cout << hex << left << "bank content type:  0x" << setw(6) << bankPtr->getContentType() 
  //        << "   tag:  0x" << setw(6) << bankPtr->tag 
  //        << "   num:  0x" << setw(6) << (int)bankPtr->num << dec << endl;

  evioDOMNodeP data_bank = bankPtr->getParent();
  if( data_bank==NULL ) {
     if(VERBOSE>9) cout << "     bank has no parent. skipping ... " << endl;
     return;
  }
  evioDOMNodeP physics_event_bank = data_bank->getParent();
  if( physics_event_bank==NULL ){
     if(VERBOSE>9) cout << "     bank has no grandparent. skipping ... " << endl;
     return;
  }
  if( physics_event_bank->getParent() != NULL ){
     if(VERBOSE>9) cout << "     bank DOES have great-grandparent. skipping ... " << endl;
     return; // physics event bank should have no parent!
  }
  if(VERBOSE>9){
     cout << "     Physics Event Bank: tag="  << physics_event_bank->tag << " num=" << (int)physics_event_bank->num << dec << endl;
     cout << "     Data Bank:          tag="  << data_bank->tag << " num=" << (int)data_bank->num << dec << endl;
  }
  if((data_bank->tag & 0xFF00) == 0xFF00){
     if(VERBOSE>9) cout << "     Data Bank tag is in reserved CODA range. This bank is not ROC data. Skipping ..." << endl;
     return;
  }
  
  if(VERBOSE>9) cout << "     bank lineage check OK. Continuing with parsing ... " << endl;

  // Get data from bank in the form of a vector of uint32_t
  const vector<uint32_t> *vec = bankPtr->getVector<uint32_t>();
  //const uint32_t *iptr = &(*vec)[0];
  //const uint32_t *iend = &(*vec)[vec->size()];
  //cok if(VERBOSE>6) cout << "     uint32_t bank has " << vec->size() << " words" << endl;
  //check cout << "     uint32_t bank has " << vec->size() << " words"<<" bank_tag="<<data_bank->tag << endl;
  
  int HitCnt = 0;

  if(data_bank->tag == 65313) {
     cout<<"65313 New event"<<endl;
  } else if ( data_bank->tag == 76  && bankPtr->tag==16 ) { // rocTRD1
  //} else if ( data_bank->tag == 53) { // rocfdc

     int Sz;
     Sz = vec->size();

     if(Sz>0){
        int OLDSLOT;
        int SLOTNUM, CHANNEL,WSize,DATAReady;
        int slotidx,idx;
        char Crate[128];
        int C;
        float ped,adc1,adc2;
        int pedcnt;
        int MaxSlot;
        char Detector[128];
        int DetectorID = 0;
        int ROCID=0;
        float TIME;
        int NPEAK;
        float CHARGE;
        float PEDESTAL;
        float PEAK;
        ROCSlots[ROCID] = 0;

        DATAReady = 0;
        int NPK_count = 0;
        int NPK = 0;
        OLDSLOT = 0;
        SLOTNUM = 0;
        slotidx = 0;
        WSize=0;

        for (int slot=0;slot<15;slot++){
            for (int ch=0;ch<72;ch++){
                fADCnhit[ROCID][slot][ch]=0;
                for (int hit=0;hit<200;hit++){
                    fADCpeak[ROCID][slot][ch][hit]=-1000.;
                    fADCcharge[ROCID][slot][ch][hit]=-1000.;
                    fADCtime[ROCID][slot][ch][hit]=-1000.;
                }
                for (int sm=0;sm<2000;sm++){
	            ADCSamples[ROCID][slot][ch][sm] = -1000.;
                }
	        ADCPedestal[ROCID][slot][ch] = 0.;
	        fADCPedestal[ROCID][slot][ch] = 0.;
            }
        }

      for (int k=0; k<Sz; k++){	
	  unsigned int data = (*vec)[k];
          //cout<<" data="<<hex<<data<<dec<<endl;
          //cout<<" mode="<<((data & 0xf8000000) >>27)<<endl;
          if(data_bank->tag==58){
             // cout<<" mode="<<((data & 0xf8000000) >>27)<<endl;
             //check  printf("mode=0x%x \n",data );
          }
	  if(((data & 0xf8000000) >>27) == 0x10) { // Block Header
	     SLOTNUM = ((data& 0x07C00000)>>22);
             //cout<<"slot="<<SLOTNUM<<endl;
	     int evntnost = (data& 0xf);
             //cout<<" slot, number of events in block="<<evntnost<<endl;
	  if(SLOTNUM!=OLDSLOT){
	     //cout<<SLOTNUM<<"   "<<OLDSLOT<<endl;
	     OLDSLOT = SLOTNUM;  
	     ROCSlots[ROCID]++;
	  }

	  slotidx = SLOTNUM-3;
	  if(SLOTNUM>10){
	     slotidx -= 2; 
	  }
	  MaxSlot = slotidx;
	  } else if (((data & 0xf8000000)>>27) == 0x12) {
              //cout<<" ST event no="<<SLOTNUM<<" "<<(data & 0x3FFFFF)<<endl; 
	      evntno_trd = (data & 0x3FFFFF);
            } else if (((data & 0xf8000000)>>27) == 0x13) {
                long int ta = (long int)(data>>16)&0xff;
                long int tb = (long int)(data>>8)&0xff;
                long int tc = (long int)data&0xff;
	        unsigned int next_data = (*vec)[k+1];
                long int td = (long int)(next_data>>16)&0xff;
                long int te = (long int)(next_data>>8)&0xff;
                long int tf = (long int)next_data&0xff;
                //cout<<" a,b,c,d,e,f="<<hex<<ta<<" "<<tb<<" "<<tc<<" "<<td<<" "<<te<<" "<<tf<<dec<<endl; 
                long int trigtrd=(tf<<0) + (te<<8) + (td<<16) + (tc<<24) + (tb<<32) + (ta<<40);
                //cout<<" trigtrd="<<trigtrd<<endl;
                trig_trd[SLOTNUM-10]=(tf<<0) + (te<<8) + (td<<16) + (tc<<24) + (tb<<32) + (ta<<40);
                trig_trd[SLOTNUM-10]=8*trig_trd[SLOTNUM-10];
                //trig_trd[SLOTNUM-10]=8*trig_trd[SLOTNUM-10];
                //cout<<" -----TRD ----- slot,trig_trd,delta="<<SLOTNUM<<" "<<trig_trd[SLOTNUM-10][9]<<" "<<trig_st[3][9]-trig_trd[SLOTNUM-10][9]<<endl; 
	    } else if (((data & 0xf8000000)>>27 == 0x14)) {
	        CHANNEL = ((data & 0x7F00000)>>20) ; // flash is couning channels from 1 to 72 need 0 to 71
	        WSize =  (data & 0xFFF);
                //cout<<" SLOTNUM,CHANNEL,WSize="<<SLOTNUM<<" "<<CHANNEL<<" "<<WSize<<"}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}"<<endl;
                //cout<<" channel,samples="<<CHANNEL<<" "<<WSize<<endl;
	        DATAReady = WSize/2;
                idx = 0;
	        ped = 0.;
	        pedcnt = 0;
	        ADCPedestal[ROCID][slotidx][CHANNEL] = 0.;
	        fADCPedestal[ROCID][slotidx][CHANNEL] = 0.;
                //check if(data_bank->tag==58)cout<<endl<<endl<<" slot,ch,wsize="<<SLOTNUM<<" "<<CHANNEL<<" "<<WSize<<endl;;
	    } else if (DATAReady>0) { // Window Raw Data values
	        DATAReady--;
 
                if(data_bank->tag==58){
                   //check cout<<" "<<adc1<<" "<<adc2<<" ";
                }
  	        if (pedcnt<16){
	        //if (pedcnt<4)
	        adc1 =  (float)((data & 0x1FFF0000) >> 16);
	        adc2 =  (float)(data & 0x1FFF);
                //cout<<" adc1,adc2="<<adc1<<" "<<adc2<<endl;
	        ped += adc1;
	        ped += adc2;
                //cout<<" slotidx="<<slotidx<<endl;
	        ADCSamples[ROCID][slotidx][CHANNEL][idx++] = adc1;
	        ADCSamples[ROCID][slotidx][CHANNEL][idx++] = adc2;
                //if(NPK>10)cout<<" rocid,slot,ch,idx,ADCSamples="<<ROCID<<" "<<slotidx<<" "<<CHANNEL<<" "<<idx<<" "<< ADCSamples[ROCID][slotidx][CHANNEL][idx-1]<<" "<<ADCSamples[ROCID][slotidx][CHANNEL][idx-2]<<endl;
	        pedcnt+=2;
	        if(pedcnt>15){
	           ped /= 16.;
	           ADCPedestal[ROCID][slotidx][CHANNEL] = ped;
	        }
	        } else {
	           adc1 =  (float)((data & 0x1FFF0000) >> 16);
	           adc2 =  (float)(data & 0x1FFF);
                  //cout<<" adc1,adc2="<<adc1<<" "<<adc2<<endl;
                  if(adc1>4095)adc1=4095;
                  if(adc2>4095)adc2=4095;
	          ADCSamples[ROCID][slotidx][CHANNEL][idx++] = adc1;
                  //if(adc1>200.)crate->Fill((float)slotidx*100.+CHANNEL,adc1);
	          ADCSamples[ROCID][slotidx][CHANNEL][idx++] = adc2;
                  //if(adc2>200.)crate->Fill((float)slotidx*100.+CHANNEL,adc1);
                 if(NPK>10){
                    print_flg=1;
                    //cout<<" rocid,slot,ch,idx,ADCSamples="<<ROCID<<" "<<slotidx<<" "<<CHANNEL<<" "<<idx-2<<" "<< ADCSamples[ROCID][slotidx][CHANNEL][idx-2]<<" "<<ADCSamples[ROCID][slotidx][CHANNEL][idx-1]<<" "<<ADCPedestal[ROCID][slotidx][CHANNEL]<<endl;
                 }
	         if( (adc1-ADCPedestal[ROCID][slotidx][CHANNEL])>200. || 
		(adc2-ADCPedestal[ROCID][slotidx][CHANNEL])>200.) {
                    slot_special=slotidx;
                    ch_special=CHANNEL;
                    samp_special=idx-1;
                    //cout<<" ------------------------------ rocid,slot,ch,idx,ADCSamples="<<ROCID<<" "<<slotidx<<" "<<CHANNEL<<" "<<idx<<" "<< ADCSamples[9][ROCID][slotidx][CHANNEL][idx-1]<<endl;
                    //cout<<" ------------------------------ rocid,slot,ch,idx,ADCPedestal="<<ROCID<<" "<<slotidx<<" "<<CHANNEL<<" "<<idx<<" "<< ADCPedestal[9][ROCID][slotidx][CHANNEL]<<endl;
	         }
	   }
	   } else if (((data & 0xf8000000)>>27) == 0x19) { //pulse integral
	  CHANNEL = ((data & 0x7F00000)>>20) ; 
	  NPK = ((data & 0xF8000)>>15) ; 
          NPK_count=NPK;
       } else if (NPK_count>0) { //Peak loop
          NPK_count--;
          float peak = (float) ((data & 0x7FF80000)>>19) ;
          float time = (float) ((data & 0x7F800)>>11) ;
          float ped = (float) (data & 0x7FF) ;
          fADCpeak[ROCID][slotidx][CHANNEL][fADCnhit[ROCID][slotidx][CHANNEL]]=peak-ped;
          fADCtime[ROCID][slotidx][CHANNEL][fADCnhit[ROCID][slotidx][CHANNEL]]=time;
          //if(NPK>10) cout<<" -------------------------------- "<<fADCnhit[ROCID][slotidx][CHANNEL]<<" "<<CHANNEL<<" "<<time<<" "<<peak<<" "<<ped<<endl;
          fADCnhit[ROCID][slotidx][CHANNEL]++;
	} else if (((data & 0xf8000000)>>27) == 0x18) { // pulse time
	  CHANNEL = ((data & 0x7F00000)>>20) ; 
	  NPEAK = ((data & 0xC0000)>>18) ; 
          TIME = (float) (data & 0xFFFF) ;
          //if((ROCID==3&&slotidx==7&&CHANNEL==9))
          //check if((NPEAK>1))
           //    cout<<" roc,slot,ch="<<ROCID<<" "<<slotidx<<" "<<CHANNEL<<endl;
           //    cout<<" time, peak number="<<TIME/64<<" "<<NPEAK<<endl;
           //    cout<<" samples0="<<endl;
           //    for(int sm=0;sm<160;sm++)
           //    cout<<" "<<ADCSamples[ROCID][slotidx][CHANNEL][sm];
          
          //check
          fADCtime[ROCID][slotidx][CHANNEL][NPEAK]=TIME/64.;
	} else if (((data & 0xf8000000)>>27) == 0x10) { // pulse pedestal and max amplitude
	  //run<375(Beni's numbering)  CHANNEL = ((data & 0x7F00000)>>20) ; 
	  //CHANNEL = ((data & 0x7800000)>>23) ;
	  NPEAK = ((data & 0x600000)>>21) ; 
          PEAK = (float) (data & 0xFFF) ;; 
	  PEDESTAL = (float) ((data & 0x1FF000)>>12) ; 
	  
	  //fADCPedestal[ROCID][slotidx][CHANNEL] =PEDESTAL;
          //fADCpeak[ROCID][slotidx][CHANNEL][0]=PEAK-PEDESTAL;
          //fADCnhit[ROCID][slotidx][CHANNEL]=1;
          //if((ROCID==3&&slotidx==7&&CHANNEL==3))
          //check if((NPEAK>1))
          //     cout<<" roc,slot,ch="<<ROCID<<" "<<slotidx<<" "<<CHANNEL<<endl;
          //     cout<<" ped, peak, npeak="<<PEDESTAL<<" "<<PEAK<<" "<<NPEAK<<endl;
          
          //check 
       }
    }
    WindowSize = WSize;

    } //Sz>0
 } else if ( data_bank->tag == 83) { //end rocst, begin rocps1 
   int Sz;
   Sz = vec->size();
   if(Sz>0){
      int OLDSLOT;
      int SLOTNUM, CHANNEL,WSize,DATAReady;
      int slotidx,idx;
      char Crate[128];
      int C;
      float ped,adc1,adc2;
      int pedcnt;
      int MaxSlot;
      char Detector[128];
      int DetectorID = 0;
      int ROCID=0;

      float TIME;
      int NPEAK;
      float CHARGE;
      float PEDESTAL;
      float PEAK;
      ROCSlots[ROCID] = 0;

      DATAReady = 0;
      OLDSLOT = 0;
      SLOTNUM = 0;
      slotidx = 0;
      WSize=0;

      for (int slot=0;slot<15;slot++){
          for (int ch=0;ch<16;ch++){
              for (int sm=0;sm<2000;sm++){
	          PSADCSamples[slot][ch][sm] = -1000.;
              }
	      PSADCPedestal[slot][ch] = 0.;
          }
      }

      for (int k=0; k<Sz; k++){	
	  unsigned int data = (*vec)[k];
         //cout<<" data="<<hex<<data<<dec<<endl;
         // cout<<" mode="<<((data & 0xf8000000) >>27)<<endl;
	 if(((data & 0xf8000000) >>27) == 0x10) { // Block Header
	    SLOTNUM = ((data& 0x07C00000)>>22);
            //cout<<"slot="<<SLOTNUM<<endl;
	    int evntnblock = (data& 0xf);
            //cout<<" number of events in block="<<evntblock<<endl;
	    slotidx = SLOTNUM-3;
	    if(SLOTNUM>10){
	       slotidx -= 2; 
	    }
	    MaxSlot = slotidx;
      } else if (((data & 0xf8000000)>>27) == 0x12) {
          evntno_ps=(data & 0x3FFFFF);
          //cout<<" PS event no="<<SLOTNUM<<" "<<(data & 0x3FFFFF)<<endl; 
      } else if (((data & 0xf8000000)>>27) == 0x13) {
          long int pstd = (long int)(data>>16)&0xff;
          long int pste = (long int)(data>>8)&0xff;
          long int pstf = (long int)data&0xff;
	  unsigned int next_data = (*vec)[k+1];
          long int psta = (long int)(next_data>>16)&0xff;
          long int pstb = (long int)(next_data>>8)&0xff;
          long int pstc = (long int)next_data&0xff;
          if(SLOTNUM<10){
             trig_ps[SLOTNUM]=(pstf<<0) + (pste<<8) + (pstd<<16) + (pstc<<24) + (pstb<<32) + (psta<<40);
             long int trig_delta=trig_ps[SLOTNUM]-trig_trd[3];
             //cout<<" ><><><PS><><>< slot,trig_ps,trig_delta="<<SLOTNUM<<" "<<trig_ps[SLOTNUM][9]<<" "<<trig_delta<<endl; 
          }
          //if(SLOTNUM==3){
          //TRIGtest->Fill((double)trig_delta,(double)evntno_ps);
          //TRIGtest->SetPoint(tgpoint,(double)evntno_ps,(double)trig_delta);
          //TGtrig->SetPoint(tgpoint,(double)evntno_ps,(double)trig_delta);
          //TGtrig->SetPoint(tgpoint,(double)evntno_ps,(double)trig_delta);
          //TGtrig->SetPoint(tgpoint,(double)evntno_ps,(double)evntno_ps-evntno_trd);
          //tgpoint++;
          //}
          //if(abs(trig_delta+8000)>20)cout<<" ><><><PS><><>< slot,trig_delta="<<SLOTNUM<<" "<<trig_delta<<endl; 
          //cout<<" PS slotnum="<<SLOTNUM<<endl;
          /*
          if(SLOTNUM==13){
          for (int itrd=0;itrd<10;itrd++){
          int jtrd=3;
            cout<<" "<<trig_trd[jtrd][itrd];
          }
          cout<<endl;
          for (int ips=0;ips<10;ips++){
          int jps=3;
            cout<<" "<<trig_ps[jps][ips];
          }
          cout<<endl;
          for (int ips=0;ips<10;ips++){
          int jps=3;
          cout<<endl;
          for (int itrd=0;itrd<10;itrd++){
          int jtrd=3;
            long int delta=trig_ps[jps][ips]-trig_trd[jtrd][itrd];
            cout<<" "<<delta;
          }
          }
          cout<<endl;
          }
          */
      } else if (((data & 0xf8000000)>>27) == 0x14) {
	  CHANNEL = ((data & 0x7800000)>>23) ; // flash is couning channels from 1 to 72 need 0 to 71
	  WSize =  (data & 0xFFF);
          //cout<<" channel,samples="<<CHANNEL<<" "<<WSize<<endl;
	  DATAReady = WSize/2;
	  idx = 0;
	  ped = 0.;
	  pedcnt = 0;
	  PSADCPedestal[slotidx][CHANNEL] = 0.;
             //check if(data_bank->tag==58)cout<<endl<<endl<<" slot,ch,wsize="<<SLOTNUM<<" "<<CHANNEL<<" "<<WSize<<endl;;
      } else if (DATAReady>0) { // Window Raw Data values
	  DATAReady--; 
	  if(pedcnt<16){
	     //if (pedcnt<4)
	     adc1 =  (float)((data & 0x1FFF0000) >> 16);
	     adc2 =  (float)(data & 0x1FFF);
	     ped += adc1;
	     ped += adc2;
	     PSADCSamples[slotidx][CHANNEL][idx++] = adc1;
	     PSADCSamples[slotidx][CHANNEL][idx++] = adc2;
             //cout<<" rocid,slot,ch,idx,ADCSamples="<<ROCID<<" "<<slotidx<<" "<<CHANNEL<<" "<<idx<<" "<< ADCSamples[ROCID][slotidx][CHANNEL][idx-1]<<endl;
	     pedcnt+=2;
	     if(pedcnt>15){
	        ped /= 16.;
	        PSADCPedestal[slotidx][CHANNEL] = ped;
	     }
	  } else {
	      adc1 =  (float)((data & 0x1FFF0000) >> 16);
	      adc2 =  (float)(data & 0x1FFF);
              if(adc1>4095)adc1=4095;
              if(adc2>4095)adc2=4095;
	      PSADCSamples[slotidx][CHANNEL][idx++] = adc1;
	      PSADCSamples[slotidx][CHANNEL][idx++] = adc2;
          }	    
      }//end data ready
      PSWindowSize = WSize;
    } // data loop
   } //Sz>0
 } else if ( data_bank->tag == 84) { //end rocps1, begin rocps2 
     //cout<<" -------------------- rocPS222222"<<endl;
     int Sz;
     Sz = vec->size();
     if(Sz>0){
        int SLOTNUM, CHANNEL,WSize,DATAReady;
        int idx;
        float ped,adc1,adc2;
        int pedcnt;

        DATAReady = 0;
        SLOTNUM = 0;
        WSize=0;

        for (int ch=0;ch<16;ch++){
            for (int sm=0;sm<2000;sm++){
	        PSCADCSamples[ch][sm] = -1000.;
            }
	    PSCADCPedestal[ch] = 0.;
        }

        for (int k=0; k<Sz; k++){
	    unsigned int data = (*vec)[k];
           //cout<<" data="<<hex<<data<<dec<<endl;
           // cout<<" mode="<<((data & 0xf8000000) >>27)<<endl;
	   if(((data & 0xf8000000) >>27) == 0x10) { // Block Header
	      SLOTNUM = ((data& 0x07C00000)>>22);
              //cout<<" SLOTNUM="<<SLOTNUM<<endl;
	      unsigned int data1 = (*vec)[k+1];
              //cout<<" data type="<<((data1& 0xf8000000)>>27)<<endl;
	   } else if (((data & 0xf8000000)>>27) == 0x18&&SLOTNUM==6) {
              CHANNEL = ((data & 0x7800000)>>23) ;
              //cout<<"channel="<<CHANNEL<<endl;
              PSCADCSamples[CHANNEL][10]=200.;
              PSCADCPedestal[CHANNEL]=100.;
	   } else if (((data & 0xf8000000)>>27) == 0x14&&SLOTNUM==6) {
	      CHANNEL = ((data & 0x7800000)>>23) ; // flash is couning channels from 1 to 72 need 0 to 71
	      WSize =  (data & 0xFFF);
              //cout<<" channel,samples="<<CHANNEL<<" "<<WSize<<endl;
	      DATAReady = WSize/2;
	      idx = 0;
	      ped = 0.;
	      pedcnt = 0;
	      PSCADCPedestal[CHANNEL] = 0.;
              //check if(data_bank->tag==58)cout<<endl<<endl<<" slot,ch,wsize="<<SLOTNUM<<" "<<CHANNEL<<" "<<WSize<<endl;;
	   } else if (DATAReady>0&&SLOTNUM==6) { // Window Raw Data values
	      DATAReady--; 
	      if(pedcnt<16){
	         //if (pedcnt<4)
	         adc1 =  (float)((data & 0x1FFF0000) >> 16);
	         adc2 =  (float)(data & 0x1FFF);
	         ped += adc1;
	         ped += adc2;
	         PSCADCSamples[CHANNEL][idx++] = adc1;
	         PSCADCSamples[CHANNEL][idx++] = adc2;
                 //cout<<" slot,ch,idx,ADCSamples="<<SLOTNUM<<" "<<CHANNEL<<" "<<idx<<" "<< PSCADCSamples[CHANNEL][idx-1]<<endl;
	         pedcnt+=2;
	         if(pedcnt>15){
	            ped /= 16.;
	            PSCADCPedestal[CHANNEL] = ped;
	         }
	   } else {
	      adc1 =  (float)((data & 0x1FFF0000) >> 16);
	      adc2 =  (float)(data & 0x1FFF);
              if(adc1>4095)adc1=4095;
              if(adc2>4095)adc2=4095;
  	      PSCADCSamples[CHANNEL][idx++] = adc1;
	      PSCADCSamples[CHANNEL][idx++] = adc2;
           }	    
           }//end data ready
           //cok PSCWindowSize = WSize;
           PSCWindowSize = 100;
    } // data loop
   } //Sz>0
   } //end rocps2
}

