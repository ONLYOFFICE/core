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
    ui->widget->setGeometry(10, 10, this->width() - 20, this->height() - ui->horizontalSlider->height() - 10);

    ui->horizontalSlider->setGeometry(10,
                                      this->height() - ui->horizontalSlider->height(),
                                      500,
                                      ui->horizontalSlider->height());

    ui->pushButton->setGeometry(520,
                                this->height() - ui->horizontalSlider->height(),
                                60,
                                ui->horizontalSlider->height());
}

void MainWindow::closeEvent(QCloseEvent* e)
{
    ui->widget->closeEvent(e);
}

void MainWindow::on_horizontalSlider_valueChanged(int value)
{
    ui->widget->SetZoom((double)value / 100.0);
}

void MainWindow::on_pushButton_clicked()
{
    ui->widget->ChangeCountPagesInBlock();
}
