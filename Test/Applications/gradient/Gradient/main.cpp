#include "mainwindow.h"

#include <QApplication>
#include "Graphics.h"
int main(int argc, char *argv[])
{
    NSGraphics::Create();
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
