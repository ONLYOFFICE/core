#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <functional>

#define URL L"http://download.onlyoffice.com/install/desktop/editors/windows/onlyoffice/updates/editors_update_x64.exe"

MainWindow::MainWindow(QWidget *parent):
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    downloader = new Downloader(URL, false);
    downloader->SetEvent_OnComplete(std::bind(&MainWindow::onLoadUpdateFinished, this, std::placeholders::_1));
    downloader->SetEvent_OnProgress(std::bind(&MainWindow::onProgress, this, std::placeholders::_1));
}

MainWindow::~MainWindow()
{
    delete downloader;
    delete ui;
}

void MainWindow::onProgress(const int percent)
{
    qDebug() << "Precent... " << percent;
    QMetaObject::invokeMethod(this, "onProgressSlot", Qt::QueuedConnection, Q_ARG(int, percent));
}

void MainWindow::onProgressSlot(int percent)
{
    ui->textEdit->setText(QString::number(percent));
    ui->progressBar->setValue(percent);
}

void MainWindow::onLoadUpdateFinished(int error)
{
    qDebug() << "Loading finished...";
    QMetaObject::invokeMethod(this, "onLoadFinishedSlot", Qt::QueuedConnection, Q_ARG(int, error));
}

void MainWindow::onLoadFinishedSlot(int error)
{
    if (error == 0) {
        ui->textEdit->setText("Loading finished...");
    } else {
        qDebug() << "Download error: " << error;
        ui->textEdit->setText("Download error");
    }

    const QString path = QString::fromStdWString(downloader->GetFilePath());
    if (QDir().exists(path)) QDir().remove(path);
}

void MainWindow::on_buttonStart_clicked()
{
    qDebug() << "Start loading...";
    ui->progressBar->setValue(0);
//    const QString tmp_file = QDir::tempPath() + QDir::separator() + QString("temp.tmp");
    const QString tmp_file = QString("C:\\Users\\Vladimir.Gorshenkov\\NewControl\\core\\Test\\Applications\\TestDownloader\\temp.tmp");
    ui->textEdit->setText(QString("Start loading: %1 to %2").arg(QString::fromStdWString(URL), tmp_file));
    downloader->SetFilePath(tmp_file.toStdWString());
    downloader->Start(0);
}

void MainWindow::on_buttonCancel_clicked()
{
    qDebug() << "Loading cancel...";
    ui->progressBar->setValue(0);
    ui->textEdit->setText("Loading cancel...");
    const QString path = QString::fromStdWString(downloader->GetFilePath());
    downloader->Cancel();
//    if (QDir().exists(path)) QDir().remove(path);

    qDebug() << "Stop finished";
}


