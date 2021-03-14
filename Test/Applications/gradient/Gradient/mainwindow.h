#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <cmath>
#include <QListWidgetItem>
#include "../../../../DesktopEditor/graphics/pro/Graphics.h"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

struct Point {
    Point(double _x = 0, double _y = 0) : x(_x), y(_y){}
    double x, y;
};
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

struct Info {
    NSStructures::GradientInfo ginfo;
    int gradient_type;
    Info() : gradient_type(c_BrushTypePathNewLinearGradient) {};
};

void GenerateImg(QImage &img, int grad = 1,double angle = 0,std::vector<Point> points = {});
std::vector<Point> drawCircle1(int n, double cx, double cy, double r);
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QImage img;
    QLabel *lable;
    Info info;
    std::vector<Point> points;

private slots:
    void on_RenderPic_clicked();

    void on_AngleSlider_sliderMoved(int position);

    void on_OffsetSlider_sliderMoved(int position);

    void on_PeriodicCheckBox_clicked();

    void on_StretchSlide_sliderMoved(int position);

    void on_DiscreteStepSlider_sliderMoved(int position);

    void on_PathType_itemDoubleClicked(QListWidgetItem *item);

    void on_PathType_itemClicked(QListWidgetItem *item);

    void on_GradientType_itemDoubleClicked(QListWidgetItem *item);

    void on_GradientType_itemClicked(QListWidgetItem *item);

    void on_LittleRadiusSlider_sliderMoved(int position);

    void on_LargeRadiusSlider_sliderMoved(int position);

    void on_XSizeSlider_sliderMoved(int position);

    void on_ycenterSlider_sliderMoved(int position);

    void on_xcenterSlider_sliderMoved(int position);

    void on_YSizeSlider_sliderMoved(int position);

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
