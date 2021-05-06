#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
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


    void on_OpenFileButton_clicked();

    void on_verticalScrollBar_sliderMoved(int position);

    void on_horizontalScrollBar_sliderMoved(int position);

    void on_ScaleSlider_sliderMoved(int position);

private:
    int RenderOnByteData(int nPage, BYTE **pBgraData, int *w, int *h);

    QImage pm;
    QString filename;
    int pos_x, pos_y;
    double scale_factor;
    Ui::MainWindow *ui;
    CApplicationFontsWorker oWorker;
    NSFonts::IApplicationFonts* pFonts;
    PdfReader::CPdfReader *PDFREADER;

};
#endif // MAINWINDOW_H
