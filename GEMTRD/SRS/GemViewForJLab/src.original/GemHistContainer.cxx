#include "stdio.h"
#include "hardcode.h"
#include "assert.h"
#include "GemHistContainer.h"
#include <iostream>
using namespace std;

//====================================================================
GemHistContainer::GemHistContainer(GemView* const p, int id) : 
  TQtWidget(p, Form("APV_%d", id), Qt::Widget, true), fParent(p),
  fModuleID(id), fIsActive(false)
{

  //   printf(" GemHistContainer::GemHistContainer() => %d \n", id) ;
  this->setObjectName(QString::fromUtf8(Form("APV_%d", id)));
  EnableSignalEvents(kMousePressEvent);
  fAPV = nullptr;
  fRootHist = nullptr;
  TCanvas *c1 = this->GetCanvas();
  c1->SetTopMargin(0.12);
}

//====================================================================
GemHistContainer::~GemHistContainer() {}

//====================================================================
void GemHistContainer::connectAPV(GemAPV *apv) {
  if (fIsActive) fAPV = apv;
}

//_______________________________________________________________
void GemHistContainer::RegisterHist(TH1I *hist) {
  if (fIsActive) fRootHist = hist;
}

//====================================================================
TH1I * GemHistContainer::getHist() {
  if (fIsActive) return fRootHist;
  else return 0;
}

//====================================================================
void GemHistContainer::drawHist() {
  gStyle->SetOptStat(0);
  if (fIsActive){
    TCanvas *c1 = this->GetCanvas();
    c1->cd();
    c1->SetFillColor(10);
    fRootHist->Draw();
    c1->Modified();
    c1->Update();
    this->Refresh();
  }
}

//====================================================================
void GemHistContainer::enterEvent(QEvent *event) {
  if (fIsActive){
    if (fAPV != nullptr)  fAPV->turnOnModuleColor(true);
    TCanvas *c1 = this->GetCanvas();
    //    c1->SetFillColor(5);
    c1->Modified();
    c1->Update();

  }
  TQtWidget::enterEvent(event);
}

//====================================================================
void GemHistContainer::leaveEvent(QEvent *event) {
  if (fIsActive) {
    if (fAPV != nullptr) fAPV->turnOnModuleColor(false);
    TCanvas *c1 = this->GetCanvas();
    //    c1->SetFillColor(10);
    c1->Modified();
    c1->Update();
  }
  TQtWidget::leaveEvent(event);
}

//====================================================================
void GemHistContainer::mousePressEvent(QMouseEvent* e) {
  if (fIsActive) {
    if (e->button() == Qt::LeftButton) printf(" GemHistContainer::mousePressEvent(0 => DrawCornerHisto(fRootHist) has been disbled for now \n") ;
  }
  TQtWidget::mousePressEvent(e);
}

//====================================================================_
void GemHistContainer::SetBasicInfo(TString plane, int fecid, int apv_index, int apv_id) {
  fFECID = fecid;
  fPlane = plane;
  fIndex = apv_index;
  fAPVID = apv_id;
}

//====================================================================_
void GemHistContainer::RefreshGraphics() {
  TCanvas *c1 = this->GetCanvas();
  c1->Modified();
  c1->Update();
  this->Refresh();
}






















