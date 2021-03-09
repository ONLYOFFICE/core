#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPixmap>

struct Color {
    uint r,g,b;
    Color() {
        r = b = g = 0;
    }
    Color(uint rgb) {
        b = rgb % 0x100;
        g = (rgb / 0x100) % 0x100;
        r = rgb / 0x100 / 0x100;
    }
    uint get_color() {
        return b + g * 0x100 + r * 0x10000;
    }
};

void setColor2(QImage &img, uint color) {
    for (int i = 0; i < img.size().height(); i++) {
        for (int j = 0; j < img.size().width(); j++) {
            img.setPixelColor(j, i, QColor(color));
        }
    }
}

void setLinearGrad(QImage &img, uint color1, uint color2) {
    Color c1(color1);
    Color c2(color2);
    for (int i = 0; i < img.size().height(); i++) {
        for (int j = 0; j < img.size().width(); j++) {
            Color cr;
            cr.r = (j * c2.r + (img.size().width() - j - 1) * c1.r) / img.size().width();
            cr.b = (j * c2.b + (img.size().width() - j - 1) * c1.b) / img.size().width();
            cr.g = (j * c2.g + (img.size().width() - j - 1) * c1.g) / img.size().width();
            img.setPixelColor(j, i, QColor(cr.get_color()));
        }
    }
}


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , img("test.png")
    , lable(new QLabel)
{
    ui->setupUi(this);
    QImage pm(600, 600,QImage::Format_RGB888);
    setLinearGrad(pm, 0xFF0000, 0x00FF00);
    //setColor2(pm, 0x0000FF);
   //pm.invertPixels();
    ui->lable_test->setPixmap(QPixmap::fromImage(pm));
    ui->lable_test->setScaledContents(true);
    ui->lable_test->resize(pm.size());
    this->resize(pm.size());
}

MainWindow::~MainWindow()
{
    delete ui;
}

