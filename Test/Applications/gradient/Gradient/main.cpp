#include "mainwindow.h"
#include <iostream>
#include <QApplication>
#include "../../../../DesktopEditor/graphics/pro/Graphics.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv );
    MainWindow w;
    w.show();
    return a.exec();
}
