#ifndef GEMAPV_H
#define GEMAPV_H

#include "TCanvas.h"
#include "GemHistContainer.h"
#include "GemHistContainer.h"
#include "GemView.h"
class GemHistContainer;
class GemAPV
{
 public:
  GemAPV();
  GemAPV(int id, double x, double y, double width, double height, int color);
  ~GemAPV();
  inline int getModuleID() const { return fModuleID; };
  void connectCanvas(GemHistContainer *c1);
  void SetBasicInfo(TString plane, int fec, int apv_id);
  bool IsActive() const { return fIsActive; }
 protected:
 private:
  int fModuleID;
  double fTransparent;
  GemHistContainer *fHistContainer;
  TString fPlane;
  int fFECID;
  int fIndex;
  int fAPVID;
  bool fIsActive;
};


#endif
