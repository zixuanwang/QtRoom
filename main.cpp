#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainWindow w;
    w.show();
//    QWidget vBox;
//    vBox.setWindowTitle( "People Plot" );

//    PeoplePlot *plot = new PeoplePlot( &vBox );
//    plot->setTitle( "History" );

//    const int margin = 5;
//    plot->setContentsMargins( margin, margin, margin, margin );

//    QString info( "Press the legend to en/disable a curve" );

//    QLabel *label = new QLabel( info, &vBox );

//    QVBoxLayout *layout = new QVBoxLayout( &vBox );
//    layout->addWidget( plot );
//    layout->addWidget( label );

//    vBox.resize( 600, 400 );
//    vBox.show();
    return app.exec();
}
