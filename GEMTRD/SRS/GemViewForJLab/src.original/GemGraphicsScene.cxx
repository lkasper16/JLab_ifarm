#include "GemGraphicsScene.h"
#include "assert.h"
#include "GemAPV.h"
#include <QPaintEvent>
#include <QPalette>
#include <QHBoxLayout>
#include <QPen>
#include <QBrush>
#include <QList>
using namespace std;

//____________________________________________________________________________
GemGraphicsScene::GemGraphicsScene(int width, int height) : fHeight(0.95*height), fWidth(0.95*width), fNline(7),
fAccumulateHits(false)
{ 
 
  fXGEMSize = 556.8;
  fYGEMSize = 1228.8;
  fXmargin   =  0.1*fWidth;
  fYmargin   =  0.1*fHeight;
  fXinterval = (fWidth  - 2*fXmargin)/(fNline-1);
  fYinterval = (fHeight - 2*fYmargin)/(fNline-1);
  this->setMinimumHeight(fHeight);
  this->setMinimumWidth(fWidth);
   
   fGemScene = new QGraphicsScene(0,0,fWidth, fHeight);
   fGemScene->setBackgroundBrush(QColor(255, 255, 238));
   fGemView = new QGraphicsView();
   fGemView->setScene(fGemScene);
	 fGemView->fitInView(0, 0, 1.2*fWidth, 1.2*fHeight, Qt::KeepAspectRatio);   

   fGemLayout = new QHBoxLayout;
   fGemLayout->addWidget(fGemView);
   this->setLayout(fGemLayout);

   drawGemFrame();
   drawGemAPV();
   drawGemCoordinate();
 }
//___________________________________________________________

GemGraphicsScene::~GemGraphicsScene()
{}
//___________________________________________________________
void GemGraphicsScene::drawGemFrame()
{
  double Pi = atan(1)*4.;
  QPen pen1(Qt::black, 2, Qt::SolidLine, Qt::RoundCap);
  QPen pen2(Qt::black, 1, Qt::DashLine, Qt::RoundCap);
	QPen pen3(Qt::red, 2, Qt::SolidLine, Qt::RoundCap);	


  fGemScene->addLine(fXmargin, fYmargin, fWidth-fXmargin, fYmargin, pen1);
  fGemScene->addLine(fWidth-fXmargin, fYmargin, fWidth-fXmargin, fHeight-fYmargin, pen1);
  fGemScene->addLine(fXmargin, fHeight-fYmargin, fWidth-fXmargin, fHeight-fYmargin, pen1);
  fGemScene->addLine(fXmargin, fYmargin, fXmargin, fHeight-fYmargin, pen1);

	double factor = 1.035;
  for (int i=0; i<6; i++)
    {
      fGemScene->addLine(fXmargin,fYmargin+fYinterval*i, fWidth-fXmargin, fYmargin+fYinterval*i, pen2);
    }

  for (int i=0; i<4; i++)
    {
			fGemScene->addLine(fXmargin+factor*fXinterval*i, fYmargin, fXmargin+factor*fXinterval*i, fHeight-fYmargin, pen2);
			fGemScene->addLine(fWidth-fXmargin-i*factor*fXinterval, fYmargin, fWidth-fXmargin-i*factor*fXinterval, fHeight-fYmargin, pen2);			
		}
  fXScaletoGEM = 3*fXinterval*factor/fXGEMSize; //mm
  fYScaletoGEM = (fHeight - 2*fYmargin)/fYGEMSize; //mm 
	double xOverLap = fXmargin+factor*fXinterval*3 - (fWidth-fXmargin-3*factor*fXinterval);
	double xTip = fXmargin+factor*fXinterval*3 - xOverLap;
	double yTip = fYmargin+fYinterval*3 - xOverLap/2.;
	fHitRadius = 0.2*xOverLap;
	fGemScene->addEllipse(xTip, yTip, xOverLap, xOverLap, pen3);
  fGemScene->addLine(xTip+xOverLap/2.-sin(Pi/4.)*xOverLap/2.,
										 yTip+xOverLap/2.-sin(Pi/4.)*xOverLap/2.,
										 xTip+xOverLap/2.+sin(Pi/4.)*xOverLap/2.,
										 yTip+xOverLap/2.+sin(Pi/4.)*xOverLap/2., pen3);

	fGemScene->addLine(xTip+xOverLap/2.+sin(Pi/4.)*xOverLap/2.,
										 yTip+xOverLap/2.-sin(Pi/4.)*xOverLap/2.,
										 xTip+xOverLap/2.-sin(Pi/4.)*xOverLap/2.,
										 yTip+xOverLap/2.+sin(Pi/4.)*xOverLap/2., pen3);
}


void GemGraphicsScene::calcAPVLocation(int i, double &x, double &y, double &width, double &height)
{
  //let we we define x and y is not the same as how they are defined in QT
  int nInX = 12;
	int nInY = 24;
  double totalX = 3*1.02*fXinterval;
  double totalY = fHeight - 2*fYmargin;
  double deltaX = totalX/nInX;
  double deltaY = totalY/nInY;
  double xOffset = 0.1*fXmargin;
  double yOffset = 0.1*fYmargin;
  if (i<24){ // first 24 APV, left hand size vertical direction from top to bottom
    width = 0.2*fXmargin;
    height = deltaY;
    x = fXmargin-xOffset-0.2*fXmargin;
    y = fYmargin + (i)*deltaY;
    
  }else if (i>=24 && i<36){ //APV 24 - 35, horizontal diretion of the left chamber, 7 on top, 5 on bottom
		width = deltaX;
		height = 0.2*fYmargin;
		x = fXmargin + (i-24)*deltaX;
		if ((i - 24) < 6 || i == 35){
			y = fYmargin - yOffset - 0.2*fYmargin;
		}else{
			y = fHeight - fYmargin + yOffset;		
		}
  }else if (i>=36 && i<60){ // APV 36 - 59, vertical direction of the right chamber, from bottom to top
    width = 0.2*fXmargin;
    height = deltaY;
    x = fWidth - fXmargin + xOffset;
    y = fHeight - fYmargin - (i-35)*deltaY;
  }else{ // last 12 APV, horizontal direction on the right chamber, 5 on top, 7 on bottom
   	width = deltaX;
		height = 0.2*fYmargin;
		x = fWidth - fXmargin - (i-59)*deltaX;
		if ((i - 60) < 6 || i == 71){
			y = fHeight - 8*yOffset + 0.2*fYmargin;
		}else{
			y = fYmargin - 4*yOffset - 0.2*fYmargin;		
		}
    
  }
}
//___________________________________________________________
void GemGraphicsScene::drawGemAPV()
{
  double x, y, width, height;
  for (int i=0; i<72; i++)
    {
      calcAPVLocation(i, x, y, width, height);
      if (i<36){
				fAPV.push_back(new GemAPV(i, x, y, width, height, Qt::red));
      }else{
				fAPV.push_back(new GemAPV(i, x, y, width, height, Qt::darkGreen));
      }
      
      fGemScene->addItem(fAPV[i]);
    }


}

//___________________________________________________________
GemAPV *GemGraphicsScene::getGemAPV(int n )
{
  assert(n<(int)fAPV.size() &&" requested Gem APV ID too large");
  return fAPV[n];
}
//____________________________________________________________
void GemGraphicsScene::wheelEvent(QWheelEvent *event)
{
	double numDegrees = -event->delta()/8.0;
	double numSteps = numDegrees / 15.0;
	double factor = std::pow(1.125, numSteps);

	fGemView->scale(factor, factor);
}
//_____________________________________________________________
void GemGraphicsScene::drawGemCoordinate()
{
  //oh boy, I hate hard-coding
  double pi = atan(1.)*4.;
  QPen pen1(Qt::red, 2, Qt::SolidLine, Qt::RoundCap);
  QPen pen2(Qt::darkGreen, 2, Qt::SolidLine, Qt::RoundCap);
  QFont aFont("times", 12);
  aFont.setItalic(true);
  //pRadGEM1Y
  fGemScene->addLine(fXmargin/4., fYmargin, fXmargin/4., fHeight-fYmargin, pen1);
  fGemScene->addLine(fXmargin/4., fHeight-fYmargin, 
  fXmargin/4.-0.015*(fHeight)*sin(pi/4.), (fHeight-fYmargin)-0.015*(fHeight)*sin(pi/4.), pen1);
  fGemScene->addLine(fXmargin/4., fHeight-fYmargin, 
  fXmargin/4.+0.015*(fHeight)*sin(pi/4.), (fHeight-fYmargin)-0.015*(fHeight)*sin(pi/4.), pen1);
  fText[0] = new QGraphicsTextItem;
  fText[0]->setPos(-0.3*fXmargin, 0.6*fHeight);
  fText[0]->setHtml("<font color=\"red\">pRadGEM1Y<font color=\"red\">");
  fText[0]->rotate(-90.);
  fText[0]->setFont(aFont);
  fGemScene->addItem(fText[0]);
  
  //pRadGEM1X
  fGemScene->addLine(fXmargin, fHeight-fYmargin/4., fWidth/2., fHeight-fYmargin/4., pen1);
  fGemScene->addLine(fWidth/2., fHeight-fYmargin/4., 
  fWidth/2.-0.015*(fHeight)*sin(pi/4.), (fHeight-fYmargin/4.)-0.015*(fHeight)*sin(pi/4.), pen1);
  fGemScene->addLine(fWidth/2., fHeight-fYmargin/4., 
  fWidth/2.-0.015*(fHeight)*sin(pi/4.), (fHeight-fYmargin/4.)+0.015*(fHeight)*sin(pi/4.), pen1);
  fText[1] = new QGraphicsTextItem;
  fText[1]->setPos(0.2*fWidth, 0.98*fHeight);
  fText[1]->setHtml("<font color=\"red\">pRadGEM1X<font color=\"red\">");
  fText[1]->rotate(0);
  fText[1]->setFont(aFont);
  fGemScene->addItem(fText[1]);
  
  //pRadGEM2Y
  fGemScene->addLine(fWidth-fXmargin/5., fHeight-fYmargin, fWidth-fXmargin/5., fYmargin, pen2);
  fGemScene->addLine(fWidth-fXmargin/5., fYmargin, 
  fWidth-fXmargin/5.-0.015*(fHeight)*sin(pi/4.), (fYmargin)+0.015*(fHeight)*sin(pi/4.), pen2);
  fGemScene->addLine(fWidth-fXmargin/4., fYmargin, 
  fWidth-fXmargin/5.+0.015*(fHeight)*sin(pi/4.), (fYmargin)+0.015*(fHeight)*sin(pi/4.), pen2);
  fText[2] = new QGraphicsTextItem;
  fText[2]->setPos(1.04*fWidth, 0.4*fHeight);
  fText[2]->setHtml("<font color=\"darkGreen\">pRadGEM2Y<font color=\"darkGreen\">");
  fText[2]->rotate(90);
  fText[2]->setFont(aFont);
  fGemScene->addItem(fText[2]);
  
  //pRadGEM2X
  fGemScene->addLine(fWidth-fXmargin, fYmargin/4., fWidth/2., fYmargin/4., pen2);
  fGemScene->addLine(fWidth/2., fYmargin/4.,
  fWidth/2.+0.015*(fHeight)*sin(pi/4.), (fYmargin/4.)-0.015*(fHeight)*sin(pi/4.), pen2);
  fGemScene->addLine(fWidth/2., fYmargin/4., 
  fWidth/2.+0.015*(fHeight)*sin(pi/4.), (fYmargin/4.)+0.015*(fHeight)*sin(pi/4.), pen2);
  fText[3] = new QGraphicsTextItem;
  fText[3]->setPos(0.6*fWidth, -0.04*fHeight);
  fText[3]->setHtml("<font color=\"darkGreen\">pRadGEM2X<font color=\"darkGreen\">");
  fText[3]->rotate(0);
  fText[3]->setFont(aFont);
  fGemScene->addItem(fText[3]);
  
  
}
//___________________________________________________________________________________________________
void GemGraphicsScene::DrawGemHit(vector<float> *x1, vector<float> *y1, vector<float> *x2, vector<float> *y2)
{
  assert(x1->size() == y1->size());
  assert(x2->size() == y2->size());
  
  if (!fAccumulateHits){
    RemoveGemHit();
  }
  QBrush theBrush1(Qt::red, Qt::SolidPattern);
  QBrush theBrush2(Qt::darkGreen, Qt::SolidPattern);
  QPen thePen1(Qt::red, 1);
  QPen thePen2(Qt::darkGreen, 1);
  float thisRadius = fHitRadius;
  if (fAccumulateHits) thisRadius = 0.2*fHitRadius;
  for (unsigned int i=0; i<x1->size(); i++){
    float xLab, yLab;
    GEMToLabFrame(x1->at(i), y1->at(i), &xLab, &yLab, 0);
    
    fHitStorageVector.push_back(fGemScene->addEllipse(xLab, yLab, 2*thisRadius, 2*thisRadius, thePen1, theBrush1)); 
  }
  for (unsigned int i=0; i<x2->size(); i++){
    float xLab, yLab;
    GEMToLabFrame(x2->at(i), y2->at(i), &xLab, &yLab, 1);
    fHitStorageVector.push_back(fGemScene->addEllipse(xLab, yLab, 2*thisRadius, 2*thisRadius, thePen2, theBrush2)); 
  }
  
  
}
//____________________________________________________________________________________________________
void GemGraphicsScene::RemoveGemHit()
{
  for (unsigned int i=0; i<fHitStorageVector.size(); i++){
    fGemScene->removeItem(fHitStorageVector.at(i));
  }
  fHitStorageVector.clear();
  std::vector<QGraphicsItem*>().swap(fHitStorageVector);
  assert(fHitStorageVector.size() == 0);
}
//___________________________________________________________________________________________________
void GemGraphicsScene::GEMToLabFrame(float x, float y, float *labX, float *labY, int type)
{
  if (type == 0){
    float yStart = fYmargin-fHitRadius;
    float xStart = fXmargin-fHitRadius;
    *labX = xStart + (x + fXGEMSize/2.)*fXScaletoGEM;
    *labY = yStart + (y + fYGEMSize/2.)*fYScaletoGEM;
  }else{
    float yStart = fHeight-fYmargin-fHitRadius;
    float xStart = fWidth-fXmargin-fHitRadius;
    *labX = xStart - (x + fXGEMSize/2.)*fXScaletoGEM;
    *labY = yStart - (y + fYGEMSize/2.)*fYScaletoGEM;
  }
}

















