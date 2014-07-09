#include "mainwindow.h"
#include <QApplication>
#include "GL/glx.h"

int main(int argc, char *argv[])
{
    /*
    int err = XInitThreads();
    QApplication::setAttribute( Qt::AA_X11InitThreads );
    */

    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
