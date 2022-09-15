#include "GemZeroSupView.h"

GemZeroSupView::GemZeroSupView(int onlineMode) {
  fIsOnlineMode = onlineMode;
  fMapping = GemMapping::GetInstance();
  printf("GemZeroSupView::GemZeroSupView() ==> All initialisation done\n") ;
}

//===========================================
GemZeroSupView::~GemZeroSupView() {
}

//==============================================
void GemZeroSupView::InitZeroSupHist() {
  Int_t nbOfPlanes = fMapping->GetNbOfPlane() ;
  printf(" GemZeroSupView::InitGUI() ==> i=%d \n",nbOfPlanes) ;

  fZeroSupDisplay              = new TQtWidget *[fMapping->GetNbOfPlane()];
  fRawDataDisplay              = new TQtWidget *[fMapping->GetNbOfPlane()];
  fADCDisplay                  = new TQtWidget *[fMapping->GetNbOfPlane()];
  fHitCountDisplay             = new TQtWidget *[fMapping->GetNbOfPlane()];
  fHitUniformityDisplay        = new TQtWidget *[fMapping->GetNbOfPlane()];    
  fClusterCountDisplay         = new TQtWidget *[fMapping->GetNbOfPlane()];
  fClusterUniformityDisplay    = new TQtWidget *[fMapping->GetNbOfPlane()];    
  fClusterMultiplicityDisplay  = new TQtWidget *[fMapping->GetNbOfPlane()];
  fClusterSizeDisplay          = new TQtWidget *[fMapping->GetNbOfPlane()];    

  for (int i=0; i<nbOfPlanes; i++){
    fZeroSupDisplay[i]             = new TQtWidget();
    fRawDataDisplay[i]             = new TQtWidget();
    fADCDisplay[i]                 = new TQtWidget();
    fHitCountDisplay[i]            = new TQtWidget();
    fHitUniformityDisplay[i]       = new TQtWidget();    
    fClusterCountDisplay[i]        = new TQtWidget();
    fClusterUniformityDisplay[i]   = new TQtWidget();    
    fClusterSizeDisplay[i]         = new TQtWidget();    
    fClusterMultiplicityDisplay[i] = new TQtWidget();    
    DisableMouseEvent(fZeroSupDisplay[i]);
  }  
}

//===============================================
void GemZeroSupView::InitGUI() {
  printf(" GemZeroSupView::InitGUI() ==> Start \n") ;
  resize(QDesktopWidget().availableGeometry(this).size() * 0.85);
  fWidth = width();
  fHeight = height();

  fLayout       = new QHBoxLayout;  
  fLeftSplitter = new QVBoxLayout();  
  fTabWidget = new QTabWidget;
  fTabWidget->setMinimumWidth(fWidth);
  
  fZeroSupWidget = new QWidget();
  fLayoutForZeroSup = new QGridLayout();
  fZeroSupWidget->setLayout(fLayoutForZeroSup);
  fTabWidget->addTab(fZeroSupWidget, "Pre zero supppresion Vs Post ero suppression");

  fRawDataWidget = new QWidget();
  fLayoutForRawData = new QGridLayout();
  fRawDataWidget->setLayout(fLayoutForRawData);
  fTabWidget->addTab(fRawDataWidget, "Pre zero suppresion");

  if (!fIsOnlineMode) {
    fHitCountWidget = new QWidget();
    fLayoutForHitCount = new QGridLayout();
    fHitCountWidget->setLayout(fLayoutForHitCount);
    fTabWidget->addTab(fHitCountWidget, "Strip Hit Count Distribution");

    fHitUniformityWidget = new QWidget();
    fLayoutForHitUniformity = new QGridLayout();
    fHitUniformityWidget->setLayout(fLayoutForHitUniformity);
    fTabWidget->addTab(fHitUniformityWidget, "Strip Hit Uniformity Distribution");
 
    fClusterCountWidget = new QWidget();
    fLayoutForClusterCount = new QGridLayout();
    fClusterCountWidget->setLayout(fLayoutForClusterCount);
    fTabWidget->addTab(fClusterCountWidget, "Cluster Hit Count Distribution");

    fClusterUniformityWidget = new QWidget();
    fLayoutForClusterUniformity = new QGridLayout();
    fClusterUniformityWidget->setLayout(fLayoutForClusterUniformity);
    fTabWidget->addTab(fClusterUniformityWidget, "Cluster Hit Uniformity Distribution");

    fClusterSizeWidget = new QWidget();
    fLayoutForClusterSize = new QGridLayout();
    fClusterSizeWidget->setLayout(fLayoutForClusterSize);
    fTabWidget->addTab(fClusterSizeWidget, "Cluster size distribution");

    fClusterMultiplicityWidget = new QWidget();
    fLayoutForClusterMultiplicity = new QGridLayout();
    fClusterMultiplicityWidget->setLayout(fLayoutForClusterMultiplicity);
    fTabWidget->addTab(fClusterUniformityWidget, "Cluster multiplicity distribution");

    fADCWidget = new QWidget();
    fLayoutForADC = new QGridLayout();
    fADCWidget->setLayout(fLayoutForADC);
    fTabWidget->addTab(fADCWidget, "Cluster ADC Distribution");
  }

  map <TString, Int_t> listOfPlanes = fMapping->GetPlaneIDFromPlaneMap() ;
  map <TString, Int_t>::const_iterator plane_itr ;

  for (plane_itr = listOfPlanes.begin(); plane_itr != listOfPlanes.end(); ++ plane_itr) {
    Int_t planeId = (*plane_itr).second ;
    Int_t detId = fMapping->GetDetectorID(fMapping->GetDetectorFromPlane( (*plane_itr).first ) ) ;
    Int_t i = 2 * detId + planeId ;
    fLayoutForZeroSup->addWidget(fZeroSupDisplay[i], detId, planeId) ;
    fLayoutForRawData->addWidget(fRawDataDisplay[i], detId, planeId) ;
    if (!fIsOnlineMode) {
      fLayoutForHitCount->addWidget(fHitCountDisplay[i], detId, planeId);
      fLayoutForHitUniformity->addWidget(fHitUniformityDisplay[i], detId, planeId);
      fLayoutForClusterCount->addWidget(fClusterCountDisplay[i], detId, planeId);
      fLayoutForClusterUniformity->addWidget(fClusterUniformityDisplay[i], detId, planeId);
      fLayoutForClusterSize->addWidget(fClusterSizeDisplay[i], detId, planeId);
      fLayoutForClusterMultiplicity->addWidget(fClusterMultiplicityDisplay[i], detId, planeId);
      fLayoutForADC->addWidget(fADCDisplay[i], detId, planeId);
    }
  }

  fLeftSplitter->addWidget(fTabWidget);  
  fLayout->addLayout(fLeftSplitter);
  
  fMainWidget = new QWidget;
  fMainWidget->setLayout(fLayout);
  setCentralWidget(fMainWidget);
  setWindowTitle(tr("GEM Hits Monitor"));
  printf(" GemZeroSupView::InitGUI() ==> done \n") ;

}

//=====================================================
void GemZeroSupView::InitToolBox() {
  printf(" GemZeroSupView::InitToolBox() ==> Start \n") ;
  fToolBox = new QHBoxLayout;
  fEventSpin = new QSpinBox;
  
  fEventSpin->setRange(0, 100000);
  QObject::connect(fEventSpin, SIGNAL(valueChanged(int)), this, SLOT(SpinCurrentEvent(int)));
  fAnalyzer = new QPushButton("Analyze file");
  fToolBox->addWidget(fEventSpin);
  QObject::connect(fAnalyzer, SIGNAL(clicked()), this, SLOT(AnalyzeFile()));
  fToolBox->addWidget(fAnalyzer);;

  /**
  if (fConfig.GetOnlineMode() == 0) {
    fEventSpin->setRange(0, 100000);
    QObject::connect(fEventSpin, SIGNAL(valueChanged(int)), this, SLOT(SpinCurrentEvent(int)));
  }else{
    fEventSpin->setRange(fConfig.GetCycleWait(), 30);
    QObject::connect(fEventSpin, SIGNAL(valueChanged(int)), this, SLOT(ChangeTimerInterval(int)));
  }
  fToolBox->addWidget(fEventSpin);
   
  if (fConfig.GetOnlineMode()) { 
    fAutoMode = new QPushButton("Auto Update Off");
    QObject::connect(fAutoMode, SIGNAL(clicked()), this, SLOT(ChangeAutoMode()));
    fToolBox->addWidget(fAutoMode);
  }else{
    fAnalyzer = new QPushButton("Analyze file");
    QObject::connect(fAnalyzer, SIGNAL(clicked()), this, SLOT(AnalyzeFile()));
    fToolBox->addWidget(fAnalyzer);
  }
  */

  fEnableRootFile = new QPushButton("Write to Root File");
  QObject::connect(fEnableRootFile, SIGNAL(clicked()), this, SLOT(WriteRootFile()));
  fToolBox->addWidget(fEnableRootFile);

  fLeftSplitter->addLayout(fToolBox);  
  printf(" GemZeroSupView::InitToolBox() ==> Done \n") ;

}

//==================================================
/**
void GemZeroSupView::InitRootGraphics() {
  printf(" GemZeroSupView::InitRootGraphics() ==> Start \n") ;
#if ROOT_VERSION_CODE >= ROOT_VERSION(5,16,0)
  // Make sure the ROOT graphical layer is initialised.
  static struct needgraph {   needgraph () {  TApplication::NeedGraphicsLibs() ;  gApplication->InitializeGraphics();} }  needgraph;
#endif
  printf(" GemZeroSupView::InitRootGraphics() ==> Done \n") ;
}
*/

//======================================================================
void GemZeroSupView::Clear(int plane, int type) {
  if (type == 0) {
    TCanvas *thisCanvas = fZeroSupDisplay[plane]->GetCanvas();
    thisCanvas->GetListOfPrimitives()->Clear();
    thisCanvas->Modified();
    thisCanvas->Update();
    fZeroSupDisplay[plane]->Refresh(); 
    
    TCanvas *theCanvas = fRawDataDisplay[plane]->GetCanvas();
    theCanvas->GetListOfPrimitives()->Clear();
    theCanvas->Modified();
    theCanvas->Update();
    fRawDataDisplay[plane]->Refresh();     
  }

  else if (type == 1) {
    TCanvas *thisCanvas = fADCDisplay[plane]->GetCanvas();
    thisCanvas->GetListOfPrimitives()->Clear();
    thisCanvas->Modified();
    thisCanvas->Update();
    fADCDisplay[plane]->Refresh();
  } 
  else if (type == 2) {
    TCanvas *thisCanvas = fHitCountDisplay[plane]->GetCanvas();
    thisCanvas->GetListOfPrimitives()->Clear();
    thisCanvas->Modified();
    thisCanvas->Update();
    fHitCountDisplay[plane]->Refresh();
  }
  else if (type == 3) {
    TCanvas *thisCanvas = fHitUniformityDisplay[plane]->GetCanvas();
    thisCanvas->GetListOfPrimitives()->Clear();
    thisCanvas->Modified();
    thisCanvas->Update();
    fHitUniformityDisplay[plane]->Refresh();
  }
  else if (type == 4) {
    TCanvas *thisCanvas = fClusterCountDisplay[plane]->GetCanvas();
    thisCanvas->GetListOfPrimitives()->Clear();
    thisCanvas->Modified();
    thisCanvas->Update();
    fClusterCountDisplay[plane]->Refresh();
  }
  else if (type == 5) {
    TCanvas *thisCanvas = fClusterUniformityDisplay[plane]->GetCanvas();
    thisCanvas->GetListOfPrimitives()->Clear();
    thisCanvas->Modified();
    thisCanvas->Update();
    fClusterUniformityDisplay[plane]->Refresh();
  }

  else if (type == 6) {
    TCanvas *thisCanvas = fClusterSizeDisplay[plane]->GetCanvas();
    thisCanvas->GetListOfPrimitives()->Clear();
    thisCanvas->Modified();
    thisCanvas->Update();
    fClusterSizeDisplay[plane]->Refresh();
  }

  else if (type == 7) {
    TCanvas *thisCanvas = fClusterMultiplicityDisplay[plane]->GetCanvas();
    thisCanvas->GetListOfPrimitives()->Clear();
    thisCanvas->Modified();
    thisCanvas->Update();
    fClusterMultiplicityDisplay[plane]->Refresh();
  }

}

//======================================================================
void GemZeroSupView::DrawZeroSupHist(int plane, TH1F* preZeroSupHist, TH1F *zeroSupHist) {
  Clear(plane, 0);
  TCanvas *thisCanvas = fZeroSupDisplay[plane]->GetCanvas();
  thisCanvas->Divide(1,2);
  thisCanvas->cd(1);
  preZeroSupHist->Draw();
  thisCanvas->cd(2);
  thisCanvas->Modified();
  thisCanvas->Update();
  fZeroSupDisplay[plane]->Refresh();
  
  TCanvas *theCanvas = fRawDataDisplay[plane]->GetCanvas();
  theCanvas->cd();
  theCanvas->SetFillColor(10);
  preZeroSupHist->Draw();
  theCanvas->Modified();
  theCanvas->Update();
  fRawDataDisplay[plane]->Refresh();
}

//=======================================================================
void GemZeroSupView::DrawADCHist(int plane, TH1F* theHist) {
  if (!fIsOnlineMode){
  Clear(plane, 1);
  TCanvas *thisCanvas = fADCDisplay[plane]->GetCanvas();
  //thisCanvas->SetFillColor(10);
  thisCanvas->cd();
  theHist->Draw();
  thisCanvas->Modified();
  thisCanvas->Update();
  fADCDisplay[plane]->Refresh();
  }
}

//==================================================================
void GemZeroSupView::DrawHitCountHist(int plane, TH1F* theHist) {
  if (!fIsOnlineMode){
  Clear(plane, 2);
  TCanvas *thisCanvas = fHitCountDisplay[plane]->GetCanvas();
  //thisCanvas->SetFillColor(10);
  thisCanvas->cd();
  theHist->Draw();
  thisCanvas->Modified();
  thisCanvas->Update();
  fHitCountDisplay[plane]->Refresh();
  }
}

//==================================================================
void GemZeroSupView::DrawHitUniformityHist(int plane, TH1F* theHist) {
  if (!fIsOnlineMode) {
    Clear(plane, 3);
    TCanvas *thisCanvas = fHitUniformityDisplay[plane]->GetCanvas();
    //thisCanvas->SetFillColor(10);
    thisCanvas->cd();
    theHist->Draw();
    thisCanvas->Modified();
    thisCanvas->Update();
    fHitUniformityDisplay[plane]->Refresh();
  }
}

//==================================================================
void GemZeroSupView::DrawClusterCountHist(int plane, TH1F* theHist) {
  if (!fIsOnlineMode){
  Clear(plane, 4);
  TCanvas *thisCanvas = fClusterCountDisplay[plane]->GetCanvas();
  //thisCanvas->SetFillColor(10);
  thisCanvas->cd();
  theHist->Draw();
  thisCanvas->Modified();
  thisCanvas->Update();
  fClusterCountDisplay[plane]->Refresh();
  }
}

//==================================================================
void GemZeroSupView::DrawClusterUniformityHist(int plane, TH1F* theHist) {
  if (!fIsOnlineMode) {
    Clear(plane, 5);
    TCanvas *thisCanvas = fClusterUniformityDisplay[plane]->GetCanvas();
    //thisCanvas->SetFillColor(10);
    thisCanvas->cd();
    theHist->Draw();
    thisCanvas->Modified();
    thisCanvas->Update();
    fClusterUniformityDisplay[plane]->Refresh();
  }
}
//==================================================================
void GemZeroSupView::DrawClusterSizeHist(int plane, TH1F* theHist) {
  if (!fIsOnlineMode) {
    Clear(plane, 6);
    TCanvas *thisCanvas = fClusterSizeDisplay[plane]->GetCanvas();
    //thisCanvas->SetFillColor(10);
    thisCanvas->cd();
    theHist->Draw();
    thisCanvas->Modified();
    thisCanvas->Update();
    fClusterSizeDisplay[plane]->Refresh();
  }
}

//==================================================================
void GemZeroSupView::DrawClusterMultiplicityHist(int plane, TH1F* theHist) {
  if (!fIsOnlineMode) {
    Clear(plane, 7);
    TCanvas *thisCanvas = fClusterMultiplicityDisplay[plane]->GetCanvas();
    //thisCanvas->SetFillColor(10);
    thisCanvas->cd();
    theHist->Draw();
    thisCanvas->Modified();
    thisCanvas->Update();
    fClusterMultiplicityDisplay[plane]->Refresh();
  }
}

//====================================================================
void GemZeroSupView::DisableMouseEvent(TQtWidget* theWidget) {
  theWidget->DisableSignalEvents(kMousePressEvent);
  theWidget->DisableSignalEvents(kMouseMoveEvent);
  theWidget->DisableSignalEvents(kMouseReleaseEvent);
  theWidget->DisableSignalEvents(kMouseDoubleClickEvent);
  theWidget->DisableSignalEvents(kKeyPressEvent);
  theWidget->DisableSignalEvents(kEnterEvent);
  theWidget->DisableSignalEvents(kLeaveEvent);
}












