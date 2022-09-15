#ifndef GEMGRAPHICSSCENE_H
#define GEMGRAPHICSSCENE_H
#include "TQtWidget.h"
#include "TPaveText.h"
#include "GemAPV.h"
#include "hardcode.h"
#include <QGraphicsScene>
#include <QPainter>
#include <QPaintEvent>
#include <QWidget>
#include <QGraphicsView>
#include <QHBoxLayout>
#include <QGraphicsTextItem>
#include <vector>
#include "GemMapping.h"
using namespace std;

class GemAPV;
class GemGraphicsScene : public QWidget
{
   //Q_OBJECT

 public:
  GemGraphicsScene(int width, int height);
  ~GemGraphicsScene();
  GemAPV *getGemAPV(int n);
  void drawGemAPV();
	void DrawGemHit(vector<float> *x1, vector<float> *y1, vector<float> *x2, vector<float> *y2);
	void RemoveGemHit();
	void AccumulateHits(bool is ) { fAccumulateHits = is; }
	bool IsAccumulating() const { return fAccumulateHits; }
	void GEMToLabFrame(float x, float y, float *labX, float *labY, int type);
	
	protected:
  void wheelEvent(QWheelEvent *event);
 
 private:
  void drawGemFrame();
  void drawGemCoordinate();
  void calcAPVLocation(int i, double &x, double &y, double &width, double &height);
   //GemAPV *getGemAPV(int n);

  GemMapping *fMapping;
  double fYGEMSize;
  double fXGEMSize;
  double fHeight;
  double fWidth;
  double fYmargin;
  double fXmargin;
  double fXinterval;
  double fYinterval;
  double fXScaletoGEM;
  double fYScaletoGEM;
  double fHitRadius;
  int    fNline;
  bool   fAccumulateHits;
  QHBoxLayout    *fGemLayout;
  QGraphicsScene *fGemScene;
  QGraphicsView  *fGemView;
  vector<GemAPV*> fAPV;
  QGraphicsTextItem *fText[4];
  std::vector<QGraphicsItem*> fHitStorageVector;
  
};



#endif
