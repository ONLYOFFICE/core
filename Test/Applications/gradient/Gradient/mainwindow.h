#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <cmath>
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

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
