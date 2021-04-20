#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "../../../DesktopEditor/graphics/pro/Graphics.h"
#include "../../../DesktopEditor/fontengine/ApplicationFontsWorker.h"
#include "../../../DesktopEditor/common/Directory.h"
#include "../../PdfReader.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_LeftButton_clicked();

    void on_RightButton_clicked();

    void on_RenderButton_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
