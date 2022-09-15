#include <QApplication>
#include "GemView.h"
//#include "GemZeroSupView.h"
#include "TApplication.h"


/*void CreateStatic()
{
	QTextEdit GemView::fMessager;
	QTextCursor GemView::fTextCursor;
	QTextBlockFormat GemView::fTextBlockFormat;
}*/
int main( int argc, char ** argv ) {
    //TApplication *myRootApp = new TApplication("", &argc, argv);

  QApplication app( argc, argv );
  GemView  * gemView = new GemView;
    
    /*if (argc ==1 || (argc == 2 && QString("-h") == argv[1])) {
       qWarning("Usage: HelloClick [-h | [-style=[windows | platinum | cgi | kde ]");
    }*/
    //w->resize(640,480);
    //QDesktopWidget dw;
    //w->resize(QDesktopWidget().availableGeometry(this).size() * 0.7);

  gemView->show();      
  //  GemZeroSupView * zeroSupView= new GemZeroSupView(gemView->IsOnlineMode());
  //  if (gemView->IsZeroSupOn()) zeroSupView->show();
  //  gemView->ControlZeroSupView(zeroSupView);
     
  app.connect(gemView, SIGNAL( destroyed()),  &app, SLOT( quit() ) );
  //  app.connect(zeroSupView, SIGNAL( destroyed()),  &app, SLOT( quit() ) );
  return app.exec();
}
