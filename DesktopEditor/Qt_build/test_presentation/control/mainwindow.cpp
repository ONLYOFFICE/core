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
     ui->widget->setGeometry(0, 0, this->width(), this->height() - 50);
}

void MainWindow::closeEvent(QCloseEvent* e)
{
    ui->widget->closeEvent(e);
}

void MainWindow::keyPressEvent(QKeyEvent *e)
{
    ui->widget->keyPressEvent(e);
}

void MainWindow::mousePressEvent(QMouseEvent *e)
{
    ui->widget->mousePressEvent(e);
}
void MainWindow::mouseReleaseEvent(QMouseEvent *e)
{
    ui->widget->mouseReleaseEvent(e);
}
void MainWindow:: mouseMoveEvent(QMouseEvent *e)
{
    ui->widget->mouseMoveEvent(e);
}


/*void MainWindow::on_horizontalSlider_valueChanged(int value)
{
    ui->widget->SetZoom((double)value / 100.0);
}

void MainWindow::on_pushButton_clicked()
{
}*/
