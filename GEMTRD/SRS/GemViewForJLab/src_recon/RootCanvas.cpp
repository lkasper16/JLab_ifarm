//============================================================================//
//    Below is the class to handle root canvas embedded as a QWidget          //
//    Based on TRootCanvas, Denis Bertini, M. Al-Turany                      //
//    A brief instruction on embeded root canvas can be found at              //
//    https://root.cern.ch/how/how-embed-tcanvas-external-applications        //
//    Chao Peng                                                               //
//    02/28/2016                                                              //
//============================================================================//

#include <QEvent>
#include <QMouseEvent>
#include <QPainter>
#include "RootCanvas.h"


RootCanvas::RootCanvas(QWidget *parent)
: QWidget(parent, 0), fCanvas(0)
{
    // set options needed to properly update the canvas when resizing the widget
    // and to properly handle context menus and mouse move events
    setAttribute(Qt::WA_PaintOnScreen, true);
    setAttribute(Qt::WA_OpaquePaintEvent, true);
//    setUpdatesEnabled(kFALSE);
    setMouseTracking(kTRUE);

    // register the QWidget in TVirtualX, giving its native window id
    int wid = gVirtualX->AddWindow((ULong_t)winId(), width(), height());

    // create the ROOT TCanvas, giving as argument the QWidget registered id
    fCanvas = new TCanvas("Root Canvas", width(), height(), wid);
}

RootCanvas::~RootCanvas()
{
    delete fCanvas;
}

void RootCanvas::mouseMoveEvent(QMouseEvent *e)
{
    // Handle mouse move events.
    if(fCanvas) {
        if(e->buttons() & Qt::LeftButton) {
            fCanvas->HandleInput(kButton1Motion, e->x(), e->y());
        } else if(e->buttons() & Qt::MidButton) {
            fCanvas->HandleInput(kButton2Motion, e->x(), e->y());
        } else if(e->buttons() & Qt::RightButton) {
            fCanvas->HandleInput(kButton3Motion, e->x(), e->y());
        } else {
            fCanvas->HandleInput(kMouseMotion, e->x(), e->y());
        }
    }
}

void RootCanvas::mousePressEvent(QMouseEvent *e)
{
    // Handle mouse button press events.
    if(fCanvas) {
        switch (e->button())
        {
        case Qt::LeftButton:
            fCanvas->HandleInput(kButton1Down, e->x(), e->y());
            emit TObjectSelected(fCanvas->GetSelected(), fCanvas);
            break;
        case Qt::MidButton:
            fCanvas->HandleInput(kButton2Down, e->x(), e->y());
            break;
        case Qt::RightButton:
            // does not work properly on Linux...
            // ...adding setAttribute(Qt::WA_PaintOnScreen, true) 
            // seems to cure the problem
            fCanvas->HandleInput(kButton3Down, e->x(), e->y());
            break;
        default:
            break;
        }
    }
}

void RootCanvas::mouseReleaseEvent(QMouseEvent *e)
{
    // Handle mouse button release events.
    if(fCanvas) {
        switch (e->button())
        {
        case Qt::LeftButton:
            fCanvas->HandleInput(kButton1Up, e->x(), e->y());
            break;
        case Qt::MidButton:
            fCanvas->HandleInput(kButton2Up, e->x(), e->y());
            break;
        case Qt::RightButton:
            // does not work properly on Linux...
            // ...adding setAttribute(Qt::WA_PaintOnScreen, true) 
            // seems to cure the problem
            fCanvas->HandleInput(kButton3Up, e->x(), e->y());
            break;
        default:
            break;
        }
    }
}

void RootCanvas::mouseDoubleClickEvent(QMouseEvent *e)
{
    // Handle mouse button release events.
    if(fCanvas) {
        switch (e->button())
        {
        case Qt::LeftButton:
            fCanvas->HandleInput(kButton1Double, e->x(), e->y());
            break;
        case Qt::MidButton:
            fCanvas->HandleInput(kButton2Double, e->x(), e->y());
            break;
        case Qt::RightButton:
            fCanvas->HandleInput(kButton3Double, e->x(), e->y());
            break;
        default:
            break;
        }
    }
}

void RootCanvas::resizeEvent(QResizeEvent *e)
{
    // Handle resize events.
    QWidget::resizeEvent(e);
    fNeedResize = true;
}

void RootCanvas::paintEvent(QPaintEvent *)
{
    // Handle paint events.
    if(fCanvas) {
        QPainter p;
        p.begin(this);
        p.end();
        if(fNeedResize) {
            fCanvas->Resize();
            fNeedResize = false;
        }
        fCanvas->Update();
    }
}

void RootCanvas::leaveEvent(QEvent * /*e*/)
{
    if(fCanvas) fCanvas->HandleInput(kMouseLeave, 0, 0);
}

bool RootCanvas ::eventFilter(QObject *o, QEvent *e)
{
    switch(e->type())
    {
    case QEvent::Close:
        if(fCanvas) delete fCanvas;
        break;
    case QEvent::ChildRemoved:
    case QEvent::Destroy:
    case QEvent::Move:
    case QEvent::Paint:
        return false;
    default:
        break;
    }

    return QWidget::eventFilter(o, e);
}

void RootCanvas::SetFillColor(Color_t c)
{
    fCanvas->SetFillColor(c);
}

void RootCanvas::SetFrameFillColor(Color_t c)
{
    fCanvas->SetFrameFillColor(c);
}

void RootCanvas::SetGrid()
{
    fCanvas->SetGrid();
}

////////////////////////////////////////////////////////////////////////////////
/// Just a wrapper

void RootCanvas::cd(Int_t subpadnumber)
{
    fCanvas->cd(subpadnumber);
}

////////////////////////////////////////////////////////////////////////////////
/// Just a wrapper.

void RootCanvas::Browse(TBrowser *b)
{
   fCanvas->Browse(b);
}

////////////////////////////////////////////////////////////////////////////////
/// Just a wrapper.

void RootCanvas::Clear(Option_t *option)
{
   fCanvas->Clear(option);
}

////////////////////////////////////////////////////////////////////////////////
/// Just a wrapper.

void RootCanvas::Close(Option_t *option)
{
   fCanvas->Close(option);
}

////////////////////////////////////////////////////////////////////////////////
/// Just a wrapper.

void RootCanvas::Draw(Option_t *option)
{
   fCanvas->Draw(option);
}

////////////////////////////////////////////////////////////////////////////////
/// Just a wrapper.

TObject *RootCanvas::DrawClone(Option_t *option)
{
   return  fCanvas->DrawClone(option);
}

////////////////////////////////////////////////////////////////////////////////
/// Just a wrapper.

TObject *RootCanvas::DrawClonePad()
{
   return  fCanvas->DrawClonePad();
}

////////////////////////////////////////////////////////////////////////////////
/// Just a wrapper.

void RootCanvas::EditorBar()
{
   fCanvas->EditorBar();
}

////////////////////////////////////////////////////////////////////////////////
/// just a wrapper

void RootCanvas::EnterLeave(TPad *prevSelPad, TObject *prevSelObj)
{
   fCanvas->EnterLeave(prevSelPad, prevSelObj);
}

////////////////////////////////////////////////////////////////////////////////
/// just a wrapper

void RootCanvas::FeedbackMode(Bool_t set)
{
   fCanvas->FeedbackMode(set);
}

////////////////////////////////////////////////////////////////////////////////
/// just a wrapper

void RootCanvas::Flush()
{
   fCanvas->Flush();
}

////////////////////////////////////////////////////////////////////////////////
/// just a wrapper

void RootCanvas::UseCurrentStyle()
{
   fCanvas->UseCurrentStyle();
}

////////////////////////////////////////////////////////////////////////////////
/// just a wrapper

void RootCanvas::ForceUpdate()
{
   fCanvas->ForceUpdate() ;
}

////////////////////////////////////////////////////////////////////////////////
/// just a wrapper

const char *RootCanvas::GetDISPLAY()
{
   return fCanvas->GetDISPLAY() ;
}

////////////////////////////////////////////////////////////////////////////////
/// just a wrapper

TContextMenu *RootCanvas::GetContextMenu()
{
   return  fCanvas->GetContextMenu() ;
}

////////////////////////////////////////////////////////////////////////////////
/// just a wrapper

Int_t RootCanvas::GetDoubleBuffer()
{
   return fCanvas->GetDoubleBuffer();
}

////////////////////////////////////////////////////////////////////////////////
/// just a wrapper

Int_t RootCanvas::GetEvent()
{
   return fCanvas->GetEvent();
}

////////////////////////////////////////////////////////////////////////////////
/// just a wrapper

Int_t RootCanvas::GetEventX()
{
   return fCanvas->GetEventX() ;
}

////////////////////////////////////////////////////////////////////////////////
/// just a wrapper

Int_t RootCanvas::GetEventY()
{
   return fCanvas->GetEventY() ;
}

////////////////////////////////////////////////////////////////////////////////
/// just a wrapper

Color_t RootCanvas::GetHighLightColor()
{
   return fCanvas->GetHighLightColor() ;
}

////////////////////////////////////////////////////////////////////////////////
/// just a wrapper

TVirtualPad *RootCanvas::GetPadSave()
{
   return fCanvas->GetPadSave();
}

////////////////////////////////////////////////////////////////////////////////
/// just a wrapper

TObject *RootCanvas::GetSelected()
{
   return fCanvas->GetSelected() ;
}

////////////////////////////////////////////////////////////////////////////////
/// just a wrapper

Option_t *RootCanvas::GetSelectedOpt()
{
   return fCanvas->GetSelectedOpt();
}

////////////////////////////////////////////////////////////////////////////////
/// just a wrapper

TVirtualPad *RootCanvas::GetSelectedPad()
{
   return fCanvas->GetSelectedPad();
}

////////////////////////////////////////////////////////////////////////////////
/// just a wrapper

Bool_t RootCanvas::GetShowEventStatus()
{
   return fCanvas->GetShowEventStatus() ;
}

////////////////////////////////////////////////////////////////////////////////
/// just a wrapper

Bool_t RootCanvas::GetAutoExec()
{
   return fCanvas->GetAutoExec();
}

////////////////////////////////////////////////////////////////////////////////
/// just a wrapper

Size_t RootCanvas::GetXsizeUser()
{
   return fCanvas->GetXsizeUser();
}

////////////////////////////////////////////////////////////////////////////////
/// just a wrapper

Size_t RootCanvas::GetYsizeUser()
{
   return fCanvas->GetYsizeUser();
}

////////////////////////////////////////////////////////////////////////////////
/// just a wrapper

Size_t RootCanvas::GetXsizeReal()
{
   return fCanvas->GetXsizeReal();
}

////////////////////////////////////////////////////////////////////////////////
/// just a wrapper

Size_t RootCanvas::GetYsizeReal()
{
   return fCanvas->GetYsizeReal();
}

////////////////////////////////////////////////////////////////////////////////
/// just a wrapper

Int_t RootCanvas::GetCanvasID()
{
   return fCanvas->GetCanvasID();
}

////////////////////////////////////////////////////////////////////////////////
/// just a wrapper

Int_t RootCanvas::GetWindowTopX()
{
   return fCanvas->GetWindowTopX();
}

////////////////////////////////////////////////////////////////////////////////
/// just a wrapper

Int_t RootCanvas::GetWindowTopY()
{
   return fCanvas->GetWindowTopY();
}

////////////////////////////////////////////////////////////////////////////////
/// just a wrapper

UInt_t RootCanvas::GetWindowWidth()
{
   return fCanvas->GetWindowWidth() ;
}

////////////////////////////////////////////////////////////////////////////////
/// just a wrapper

UInt_t RootCanvas::GetWindowHeight()
{
   return fCanvas->GetWindowHeight();
}

////////////////////////////////////////////////////////////////////////////////
/// just a wrapper

UInt_t RootCanvas::GetWw()
{
   return fCanvas->GetWw();
}

////////////////////////////////////////////////////////////////////////////////
/// just a wrapper

UInt_t RootCanvas::GetWh()
{
   return fCanvas->GetWh() ;
}

////////////////////////////////////////////////////////////////////////////////
/// just a wrapper

void RootCanvas::GetCanvasPar(Int_t &wtopx, Int_t &wtopy, UInt_t &ww, UInt_t &wh)
{
   fCanvas->GetCanvasPar(wtopx, wtopy, ww, wh);
}

////////////////////////////////////////////////////////////////////////////////
/// just a wrapper

void RootCanvas::HandleInput(EEventType button, Int_t x, Int_t y)
{
   fCanvas->HandleInput(button, x, y);
}

////////////////////////////////////////////////////////////////////////////////
/// just a wrapper

Bool_t RootCanvas::HasMenuBar()
{
   return fCanvas->HasMenuBar() ;
}

////////////////////////////////////////////////////////////////////////////////
/// just a wrapper

void RootCanvas::Iconify()
{
   fCanvas->Iconify();
}

////////////////////////////////////////////////////////////////////////////////
/// just a wrapper

Bool_t RootCanvas::IsBatch()
{
   return fCanvas->IsBatch() ;
}

////////////////////////////////////////////////////////////////////////////////
/// just a wrapper

Bool_t RootCanvas::IsRetained()
{
   return fCanvas->IsRetained();
}

////////////////////////////////////////////////////////////////////////////////
/// just a wrapper

void RootCanvas::ls(Option_t *option)
{
   fCanvas->ls(option);
}

////////////////////////////////////////////////////////////////////////////////
/// just a wrapper

void RootCanvas::MoveOpaque(Int_t set)
{
   fCanvas->MoveOpaque(set);
}

////////////////////////////////////////////////////////////////////////////////
/// just a wrapper

Bool_t RootCanvas::OpaqueMoving()
{
   return fCanvas->OpaqueMoving();
}

////////////////////////////////////////////////////////////////////////////////
/// just a wrapper

Bool_t RootCanvas::OpaqueResizing()
{
   return fCanvas->OpaqueResizing();
}

////////////////////////////////////////////////////////////////////////////////
/// just a wrapper

void RootCanvas::Paint(Option_t *option)
{
   fCanvas->Paint(option);
}

////////////////////////////////////////////////////////////////////////////////
/// just a wrapper

TPad *RootCanvas::Pick(Int_t px, Int_t py, TObjLink *&pickobj)
{
   return fCanvas->Pick(px, py, pickobj);
}

////////////////////////////////////////////////////////////////////////////////
/// just a wrapper

TPad *RootCanvas::Pick(Int_t px, Int_t py, TObject *prevSelObj)
{
   return fCanvas->Pick(px, py, prevSelObj);
}

////////////////////////////////////////////////////////////////////////////////
/// just a wrapper

void RootCanvas::Resize(Option_t *option)
{
   fCanvas->Resize(option);
}

////////////////////////////////////////////////////////////////////////////////
/// just a wrapper

void RootCanvas::ResizeOpaque(Int_t set)
{
   fCanvas->ResizeOpaque(set);
}

////////////////////////////////////////////////////////////////////////////////
/// just a wrapper

void RootCanvas::SaveSource(const char *filename, Option_t *option)
{
   fCanvas->SaveSource(filename, option);
}

////////////////////////////////////////////////////////////////////////////////
/// just a wrapper

void RootCanvas::SetCursor(ECursor cursor)
{
   fCanvas->SetCursor(cursor);
}

////////////////////////////////////////////////////////////////////////////////
/// just a wrapper

void RootCanvas::SetDoubleBuffer(Int_t mode)
{
   fCanvas->SetDoubleBuffer(mode);
}

////////////////////////////////////////////////////////////////////////////////
/// just a wrapper

void RootCanvas::SetWindowPosition(Int_t x, Int_t y)
{
   fCanvas->SetWindowPosition(x, y) ;
}

////////////////////////////////////////////////////////////////////////////////
/// just a wrapper

void RootCanvas::SetWindowSize(UInt_t ww, UInt_t wh)
{
   fCanvas->SetWindowSize(ww,wh) ;
}

////////////////////////////////////////////////////////////////////////////////
/// just a wrapper

void RootCanvas::SetCanvasSize(UInt_t ww, UInt_t wh)
{
   fCanvas->SetCanvasSize(ww, wh);
}

////////////////////////////////////////////////////////////////////////////////
/// just a wrapper

void RootCanvas::SetHighLightColor(Color_t col)
{
   fCanvas->SetHighLightColor(col);
}

////////////////////////////////////////////////////////////////////////////////
/// just a wrapper

void RootCanvas::SetSelected(TObject *obj)
{
   fCanvas->SetSelected(obj);
}

////////////////////////////////////////////////////////////////////////////////
/// just a wrapper

void RootCanvas::SetSelectedPad(TPad *pad)
{
   fCanvas->SetSelectedPad(pad);
}

////////////////////////////////////////////////////////////////////////////////
/// just a wrapper

void RootCanvas::Show()
{
   fCanvas->Show() ;
}

////////////////////////////////////////////////////////////////////////////////
/// just a wrapper

void RootCanvas::Size(Float_t xsizeuser, Float_t ysizeuser)
{
   fCanvas->Size(xsizeuser, ysizeuser);
}

////////////////////////////////////////////////////////////////////////////////
/// just a wrapper

void RootCanvas::SetBatch(Bool_t batch)
{
   fCanvas->SetBatch(batch);
}

////////////////////////////////////////////////////////////////////////////////
/// just a wrapper

void RootCanvas::SetRetained(Bool_t retained)
{
   fCanvas->SetRetained(retained);
}

////////////////////////////////////////////////////////////////////////////////
/// just a wrapper

void RootCanvas::SetTitle(const char *title)
{
   fCanvas->SetTitle(title);
}

////////////////////////////////////////////////////////////////////////////////
/// just a wrapper

void RootCanvas::ToggleEventStatus()
{
   fCanvas->ToggleEventStatus();
}

////////////////////////////////////////////////////////////////////////////////
/// just a wrapper

void RootCanvas::ToggleAutoExec()
{
   fCanvas->ToggleAutoExec();
}

////////////////////////////////////////////////////////////////////////////////
/// just a wrapper

void RootCanvas::Update()
{
   fCanvas->Update();
}

////////////////////////////////////////////////////////////////////////////////
