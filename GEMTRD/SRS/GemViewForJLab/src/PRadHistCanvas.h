
#ifndef PRAD_HIST_CANVAS_H
#define PRAD_HIST_CANVAS_H

#include <QWidget>

class QRootCanvas;
class QTimer;
class TObject;
class TCanvas;
class TF1;

class PRadHistCanvas : public QWidget
{
    Q_OBJECT

public:
    PRadHistCanvas( QWidget *parent = 0);
    virtual ~PRadHistCanvas() {}
    void UpdateHist(TObject *hist);

public slots:
    void rootInnerLoop();

protected:
    QRootCanvas *canvas1;
    QTimer *rootTimer;
};

#endif
