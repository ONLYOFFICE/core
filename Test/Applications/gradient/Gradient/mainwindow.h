/*
 * (c) Copyright Ascensio System SIA 2010-2024
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at 20A-6 Ernesta Birznieka-Upish
 * street, Riga, Latvia, EU, LV-1050.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */

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
