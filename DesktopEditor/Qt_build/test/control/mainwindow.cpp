#include "mainwindow.h"
#include "ui_mainwindow.h"

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

void MainWindow::resizeEvent(QResizeEvent* e)
{
    ui->widget->setGeometry(10, 10, this->width() - 20, this->height() - 60);
}

void MainWindow::closeEvent(QCloseEvent* e)
{
    ui->widget->closeEvent(e);
}
