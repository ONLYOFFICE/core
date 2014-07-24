#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDir>
#include <QUuid>
#include <QWebFrame>
#include <QLayout>
#include <QDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QString sBasePath = QApplication::applicationDirPath();
    QString sDebug = "Debug";
    QString sRelease = "Release";

    int n1 = sBasePath.lastIndexOf(sDebug);
    int n2 = sBasePath.lastIndexOf(sRelease);
    if (n1 < 0 && n2 < 0)
        return;

    int nCount = 0;
    if (n1 > n2)
    {
        nCount = n1 + sDebug.length();
    }
    else
    {
        nCount = n2 + sRelease.length();
    }

    if (nCount != sBasePath.length())
    {
        sBasePath = sBasePath.mid(0, nCount);
    }

    QString sFontsPath = sBasePath + "/../../../../../OfficeWeb/Fonts/native";
    QString sSdkPath = sBasePath + "/../../test_resources/sdk-all.js";
    QString sDocumentPath = sBasePath + "/../../test_resources/document/Editor.bin";

    std::wstring s1 = sFontsPath.toStdWString();
    std::wstring s2 = sSdkPath.toStdWString();
    std::wstring s3 = sDocumentPath.toStdWString();

    ui->widget->setGeometry(0, 0, this->width(), this->height());
    ui->widget->InitSDK(s1, s2);
    ui->widget->Init(s3);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::resizeEvent(QResizeEvent* e)
{
    ui->widget->setGeometry(0, 0, this->width(), this->height());
}

void MainWindow::closeEvent(QCloseEvent* e)
{
    ui->widget->closeEvent(e);
}
