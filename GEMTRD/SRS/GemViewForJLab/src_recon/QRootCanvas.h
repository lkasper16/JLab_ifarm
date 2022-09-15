#ifndef Q_ROOT_CANVAS_H
#define Q_ROOT_CANVAS_H

#include <QWidget>
#include "TCanvas.h"

class QPaintEvent;
class QResizeEvent;
class QMouseEvent;

class QRootCanvas : public QWidget
{
    Q_OBJECT

public:
    QRootCanvas( QWidget *parent = 0);
    virtual ~QRootCanvas() {}
    void Refresh() {fCanvas->Modified(); fCanvas->Update();};
    TCanvas* GetCanvas() { return fCanvas; };

protected:
    TCanvas *fCanvas;

    virtual void mouseMoveEvent( QMouseEvent *e );
    virtual void mousePressEvent( QMouseEvent *e );
    virtual void mouseReleaseEvent( QMouseEvent *e );
    virtual void paintEvent( QPaintEvent *e );
    virtual void resizeEvent( QResizeEvent *e );
};

#endif
