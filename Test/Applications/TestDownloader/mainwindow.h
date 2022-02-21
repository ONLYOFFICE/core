#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDir>
#include <QDebug>
#include "../../../../core/Common/Network/FileTransporter/include/FileTransporter.h"


//#define URL L"http://nct.onlyoffice.com/sh/XHh"
typedef std::wstring WString;
typedef NSNetwork::NSFileTransport::CFileDownloader Downloader;


QT_BEGIN_NAMESPACE
    namespace Ui {class MainWindow;}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    MainWindow(QWidget *parent = nullptr);

    ~MainWindow();

private slots:

    void on_buttonStart_clicked();
    void on_buttonCancel_clicked();

    void onProgressSlot(int percent);
    void onLoadFinishedSlot(int error);

private:

    void onProgress(const int percent);
    void onLoadUpdateFinished(int error);

    Downloader *downloader;

    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
