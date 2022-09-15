#ifndef GEMAPV_H
#define GEMAPV_H

#include <QGraphicsRectItem>
#include <QRectF>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QWidget>
#include <QColor>
#include <QString>
#include <QFont>
#include "TQtWidget.h"
#include "TCanvas.h"
#include "GemHistContainer.h"
#include "GemHistContainer.h"
#include "GemView.h"
class GemHistContainer;
class GemAPV : public QGraphicsRectItem
{
    public:
    GemAPV();
    GemAPV(int id, double x, double y, double width, double height, QColor color);
    ~GemAPV();
    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem* option, QWidget *widget);
    QRectF boundingRect() const;
    void turnOnModuleColor(bool on);
    inline int getModuleID() const { return fModuleID; };
    void connectCanvas(GemHistContainer *c1);
		void SetBasicInfo(TString plane, int fec, int apv_id);
		bool IsActive() const { return fIsActive; }
    protected:
    virtual void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
		virtual void mousePressEvent(QGraphicsSceneMouseEvent * event);
    private:
    int fModuleID;
    double fTransparent;
    QColor fDefaultColor;
    QColor fCurrentColor;
    QRectF *fTextBox;
    GemHistContainer *fHistContainer;
    QString fStringID;
    QFont   fFont;
		TString fPlane;
		int fFECID;
		int fIndex;
		int fAPVID;
		bool fIsActive;
};


#endif
