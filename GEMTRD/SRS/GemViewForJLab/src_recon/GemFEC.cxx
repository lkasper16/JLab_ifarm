#include "GemFEC.h"
#include "TCanvas.h"
#include <vector>
#include "TAxis.h"
//====================================================================
GemFEC::GemFEC() {}
//====================================================================
GemFEC::GemFEC(GemView* const p, int FECid, int nChannel, list<int> * APV_id_list):
  TQtWidget(p, "", Qt::Widget, true), fFECid(FECid), fNChannel(nChannel), fNBins(2500) {

  //  printf("  GemFEC::GemFEC(), FECId=%d, numberChannels=%d \n", FECid, fNChannel) ;

  fMapping = GemMapping::GetInstance();
  EnableSignalEvents(kMousePressEvent);
  this->setObjectName(QString::fromUtf8(Form("FEC %i", fFECid)));
  
  fChannelHist = new TH1I * [fNChannel];
  for (int i=0; i<fNChannel; i++) {
    //    printf("  GemFEC::GemFEC(), FECId=%d, Channel=%d \n", FECid, i) ;
    fChannelIsActive.push_back(0);
    fChannelHist[i] = new TH1I (Form("APV_%d_FEC_%d", i, fFECid), Form("APV_%d_FEC_%d_INACTIVE", i, fFECid), fNBins, 0, fNBins);
    fChannelHist[i] -> SetLabelSize(0.05, "X");
    fChannelHist[i] -> SetLabelSize(0.05, "Y");
    gStyle -> SetTitleFontSize(0.08);
    fChannelHist[i]->SetStats(0);
  }
  //set the active channel according to the list
  for (std::list<int>::iterator it = (*APV_id_list).begin(); it != (*APV_id_list).end(); it++){
    TString thisPlane = (fMapping->GetPlaneFromAPVID(*it)).Data();
    Int_t thisIndex = fMapping->GetAPVIndexOnPlane(*it);
    fAPVID.push_back(*it);
    int apvCh = *it % fNChannel;
    assert(apvCh<fNChannel);
    fChannelIsActive.at(apvCh) = 1;
    fChannelHist[apvCh]->SetTitle(Form("APV_%d_FEC_%d_%s_%d", apvCh, fFECid, thisPlane.Data(), thisIndex));	
  }
}
//====================================================================
GemFEC::~GemFEC()
{
  for (int i=0; i<fNChannel; i++){
    delete fChannelHist[i];	
  } 
}

//====================================================================
void GemFEC::ClearHist() {
  for (int i=0; i<fNChannel; i++) fChannelHist[i]->Reset("M");
  TCanvas *c1 = this->GetCanvas();
  c1->GetListOfPrimitives()->Clear();
  c1->Modified();
  c1->Update();
}

//====================================================================
void GemFEC::DrawHist() {
  TCanvas *c1 = this->GetCanvas();
  //  c1->SetFillColor(10);
  c1->Divide(4,4);
  for (int i=0; i<fNChannel; i++) {
    c1->cd(i+1);
    TPad* pad = (TPad*)c1->GetPad(i+1);
    pad->SetLeftMargin(0.13);
    fChannelHist[i]->Draw();
  }
  c1->Modified();
  c1->Update();
  this->Refresh();
}

//====================================================================
int GemFEC::GetAPVIDFromThisFEC(int i) {
  if (i > (int)fAPVID.size() - 1){
    cout<<"FEC "<<fFECid<<" does not has that many active channels"<<endl;
    return -1;
  }else{
    return fAPVID.at(i);
  }
}

//====================================================================
void GemFEC::RefreshGraphics() {
  TCanvas *c1 = this->GetCanvas();
  c1->Modified();
  c1->Update();
  this->Refresh();
}

















