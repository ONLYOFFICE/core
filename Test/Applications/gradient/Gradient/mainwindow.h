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
    // Для теста адаптора все вынес в инфо.
    float r0, r1;
    NSStructures::Point c0, c1;
    NSStructures::Point p0, p1;
    bool cont_b, cont_f;
    std::vector<NSStructures::Point> triangle ={{100, 100}, {300, 200}, {200, 350}};


    NSStructures::GradientInfo ginfo;
    int gradient_type;
    std::vector<LONG> c;
    std::vector<double> p;
    int n_colors;
    Info() : gradient_type(c_BrushTypePathNewLinearGradient) {
        c = {(LONG)0xFFff0000, (LONG)0xFFffa500, (LONG)0xFFffff00, (LONG)0xFF008000, (LONG)0xFF0000ff, (LONG)0xFFFF00FF};
        p = {0.0,0.2,0.4,0.6,0.8,1};
        n_colors = 6;
        ginfo.shading.shading_type = NSStructures::ShadingInfo::Parametric;



        r0 = 0;
        r1 = 100;
        c0 = {200, 200};
        c1 = {200, 200};
        p0 = {0, 0};
        p1 = {400, 400};
        cont_b = cont_f = false;
    };
    ~Info() {
    }
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
    std::vector<Point> points;
    Info info;

private slots:
    void on_RenderPic_clicked();

    void on_GradientType_itemDoubleClicked(QListWidgetItem *item);

    void on_GradientType_itemClicked(QListWidgetItem *item);

    void on_ColorSpaces_itemClicked(QListWidgetItem *item);

    void on_ColorSpaces_itemDoubleClicked(QListWidgetItem *item);

    void on_Point1X_sliderMoved(int position);

    void on_Point1Y_sliderMoved(int position);

    void on_Point2X_sliderMoved(int position);

    void on_Point2Y_sliderMoved(int position);

    void on_CenterX0_valueChanged(int value);

    void on_CenterY0_valueChanged(int value);

    void on_CenterX1_valueChanged(int value);

    void on_CenterY1_valueChanged(int value);

    void on_r0slider_valueChanged(int value);

    void on_r1slider_valueChanged(int value);

    void on_ContinueForvard_clicked(bool checked);

    void on_ContinueBack_clicked(bool checked);

    void on_TrianglePoint1X_sliderMoved(int position);

    void on_TrianglePoint1Y_sliderMoved(int position);

    void on_TrianglePoint2X_sliderMoved(int position);

    void on_TrianglePoint2Y_sliderMoved(int position);

    void on_TrianglePoint3X_sliderMoved(int position);

    void on_TrianglePoint3Y_sliderMoved(int position);

    void on_LeftButton_clicked();

    void on_RightButton_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
