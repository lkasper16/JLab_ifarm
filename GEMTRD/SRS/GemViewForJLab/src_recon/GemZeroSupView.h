#ifndef GEMZEROSUPVIEW_H
#define GEMZEROSUPVIEW_H
#include <QMainWindow>
#include <QWidget>
#include <QSplitter>
#include <Qt>
#include <QVBoxLayout>
#include <QTimer>
#include <map>
#include <QPushButton>
#include <QSpinBox>
#include <QTabWidget>
#include <vector>
#include <QPushButton>
#include <QTextEdit>
#include <QtGui>
#include <QTextCursor>
#include <QTextBlockFormat>
#include <QGridLayout>
#include <vector>
#
#include "GemGraphicsScene.h"
#include "GemMapping.h"

using namespace std;


class GemGraphicsScene;
class GemZeroSupView : public QMainWindow
{
  public:
  GemZeroSupView(int onlineMode = 1);
  ~GemZeroSupView();

  void Clear(int plane, int type);
  void DrawZeroSupHist(int plane, TH1F* preZeroSupHist, TH1F *zeroSupHist);
  void DrawADCHist(int plane, TH1F* theHist);
  void DrawHitCountHist(int plane, TH1F* theHist);
  void DrawHitUniformityHist(int plane, TH1F* theHist);
  void DrawClusterCountHist(int plane, TH1F* theHist);
  void DrawClusterUniformityHist(int plane, TH1F* theHist);
  void DrawClusterSizeHist(int plane, TH1F* theHist);
  void DrawClusterMultiplicityHist(int plane, TH1F* theHist);
  void DisableMouseEvent(TQtWidget* theWidget);
  
  private:
  void InitGUI();
  void InitZeroSupHist();
  void InitToolBox() ;
  //  void InitRootGraphics() ;

  GemView* fMaster;
  GemMapping *fMapping;

  QSpinBox    *fEventSpin;
  QPushButton *fAutoMode;
  QPushButton *fAnalyzer;
  QPushButton *fEnableRootFile;

  QWidget *fMainWidget;
  QWidget *fZeroSupWidget;
  QWidget *fRawDataWidget;
  QWidget *fADCWidget;
  QWidget *fHitCountWidget;
  QWidget *fHitUniformityWidget;
  QWidget *fClusterCountWidget;
  QWidget *fClusterUniformityWidget;
  QWidget *fClusterSizeWidget;
  QWidget *fClusterMultiplicityWidget;

  QTabWidget *fTabWidget;
  
  QGridLayout *fLayoutForZeroSup;
  QGridLayout *fLayoutForRawData;
  QGridLayout *fLayoutForADC;
  QGridLayout *fLayoutForHitCount;
  QGridLayout *fLayoutForHitUniformity;
  QGridLayout *fLayoutForClusterCount;
  QGridLayout *fLayoutForClusterUniformity;
  QGridLayout *fLayoutForClusterSize;
  QGridLayout *fLayoutForClusterMultiplicity;
  
  QVBoxLayout *fLeftSplitter;
  QVBoxLayout *fRightSplitter;
  QHBoxLayout *fLayout;
  QHBoxLayout *fToolBox;

  int fWidth; //width of the QMainWindow
  int fHeight;//height of the QMaintWindow
  int fXMaxBin;
  int fYMaxBin;
  int fXMinBin;
  int fYMinBin;
  bool fIsOnlineMode;

  TQtWidget **fZeroSupDisplay;
  TQtWidget **fRawDataDisplay;
  TQtWidget **fADCDisplay;
  TQtWidget **fHitCountDisplay;
  TQtWidget **fHitUniformityDisplay;
  TQtWidget **fClusterCountDisplay;
  TQtWidget **fClusterUniformityDisplay;
  TQtWidget **fClusterSizeDisplay;
  TQtWidget **fClusterMultiplicityDisplay;
};
#endif
