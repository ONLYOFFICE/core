#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtPrintSupport/QPrintDialog>
#include <QtPrintSupport/QPrinter>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    ui->textEdit->setText(QString::fromStdString(this->GetLanguage()));
}

#include "XKeyboard.h"
std::string MainWindow::GetLanguage()
{
    XKeyboard keyboard;
    std::string cGrpName = keyboard.currentGroupName();
    return cGrpName;
}
