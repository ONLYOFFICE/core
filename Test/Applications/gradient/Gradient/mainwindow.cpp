#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPixmap>
#include <iostream>
#include "../../../../DesktopEditor/graphics/pro/Graphics.h"
#include "../../../../DesktopEditor/fontengine/ApplicationFontsWorker.h"
#include "../../../../DesktopEditor/common/Directory.h"
#include "../../../../PdfFile/PdfFile.h"

std::vector<Point> drawCircle1(int n, double cx, double cy, double r) {
    std::vector<Point> res;
    for (int i = 0; i < n; i++) {
        double x = cx + r * cos(i * 8 * atan(1) / n);
        double y = cy + r * sin(i * 8 * atan(1) / n);
        res.push_back({x, y});
    }
    return res;
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , img("test.png")
    , lable(new QLabel)
{
    ui->setupUi(this);
    ui->lable_test->setStyleSheet("QLabel { background-color : white;}");
    points = {{0, 0}, {105,  0}, {105, 105}, {0, 105}};
    ui->stackedWidget->setCurrentIndex(0);
    ui->statusbar->showMessage("Linear");
}



MainWindow::~MainWindow()
{
    delete ui;
}


void GenerateImg(QImage &img, std::vector<Point> &points, Info &info) {

    NSGraphics::IGraphicsRenderer* pRasterRenderer = NSGraphics::Create();
    NSFonts::IFontManager *fmp = NSFonts::NSFontManager::Create();
    pRasterRenderer->SetFontManager(fmp);
    int nRasterW = img.size().width();
    int nRasterH = img.size().height();
    BYTE* pData = new BYTE[4 * nRasterW * nRasterH];

    unsigned int back = 0xffffff;
    unsigned int* pData32 = (unsigned int*)pData;
    unsigned int* pData32End = pData32 + nRasterW * nRasterH ;
    //дефолтный тон должен быть прозрачным, а не белым
    while (pData32 < pData32End)
        *pData32++ = back;

    CBgraFrame oFrame;
    oFrame.put_Data(pData);
    oFrame.put_Width(nRasterW);
    oFrame.put_Height(nRasterH);
    oFrame.put_Stride(4 * nRasterW);

    pRasterRenderer->CreateFromBgraFrame(&oFrame);
    pRasterRenderer->SetSwapRGB(true);

    double dW_MM = nRasterW * 25.4 / 96;
    double dH_MM = nRasterH * 25.4 / 96;

    pRasterRenderer->put_Width(dW_MM);
    pRasterRenderer->put_Height(dH_MM);

    NSStructures::GradientInfo ginfo = info.ginfo;
    //ginfo.reflected = true;
    ginfo.shading.f_type = NSStructures::ShadingInfo::UseNew;
    pRasterRenderer->put_BrushGradInfo(ginfo);
    auto a = info.c;
    auto b = info.p;
    LONG *c = a.data();
    double *p = b.data();
            pRasterRenderer->CommandDrawText(L"Test", 10, 10, 345, 345);
    pRasterRenderer->put_BrushType(info.gradient_type);
    pRasterRenderer->put_BrushGradientColors(c, p, info.n_colors);
    pRasterRenderer->PathCommandStart();
    pRasterRenderer->BeginCommand(c_nPathType);
    if (points.size() > 0) {
        pRasterRenderer->PathCommandMoveTo(points[0].x, points[0].y);
        for (uint i = 1; i < points.size(); i++) {
            pRasterRenderer->PathCommandLineTo(points[i].x, points[i].y) ;
        }
    }
     pRasterRenderer->Fill();
    //pRasterRenderer->DrawPath(c_nStroke);
    pRasterRenderer->EndCommand(c_nPathType);
    pRasterRenderer->PathCommandEnd();
    pData32 = (unsigned int*)pData;
    for (long i = 0; i < img.size().height(); i++) {
        for (long j = 0; j < img.size().width(); j++) {
            img.setPixelColor(j, i, QColor(pData32[j + i * nRasterW]));
        }
    }
}

void MainWindow::on_actionLinear_Gradient_triggered()
{
    ui->stackedWidget->setCurrentIndex(0);
    ui->statusbar->showMessage("Linear");
    info.gradient_type = c_BrushTypePathNewLinearGradient;
}


void MainWindow::on_actionRadial_Gradient_triggered()
{
    ui->stackedWidget->setCurrentIndex(1);
    ui->statusbar->showMessage("Radial");
    info.gradient_type = c_BrushTypePathRadialGradient;
}

void MainWindow::on_BAW_Colorspace_Radio_Button_clicked()
{
    info.colorspace = BlackAndWhite;
}

void MainWindow::on_RAB_Colorspace_Radio_Button_clicked()
{
    info.colorspace = RedAndBlue;
}

void MainWindow::on_Pastel_Colorspace_Radio_Button_clicked()
{
    info.colorspace = Pastel;
}
void MainWindow::on_Rainbow_Colorspace_Radio_Button_clicked()
{
    info.colorspace = Rainbow;
}

void MainWindow::on_pushButton_clicked()
{
    if (info.gradient_type == c_BrushTypePathNewLinearGradient) {
        if (ui->First_X_Coordinate_Input->text() == "") {
            ui->statusbar->showMessage("First X coordinate = NULL");
            return;
        }
        info.p0.x = ui->First_X_Coordinate_Input->text().toInt();
        if (ui->First_Y_Coordinate_Input->text() == "") {
            ui->statusbar->showMessage("First Y coordinate = NULL");
            return;
        }
        info.p0.y = ui->First_Y_Coordinate_Input->text().toInt();
        if (ui->Second_X_Coordinate_Input->text() == "") {
            ui->statusbar->showMessage("First X coordinate = NULL");
            return;
        }
        info.p1.x = ui->Second_X_Coordinate_Input->text().toInt();
        if (ui->Second_Y_Coordinate_Input->text() == "") {
            ui->statusbar->showMessage("First X coordinate = NULL");
            return;
        }
        info.p1.y = ui->Second_Y_Coordinate_Input->text().toInt();
        info.ginfo = NSStructures::GInfoConstructor::get_linear(info.p0, info.p1, 0, 1, info.cont_b, info.cont_f);
    } else if (info.gradient_type == c_BrushTypePathRadialGradient) {
        if (ui->First_Center_X_Coordinate_Input->text() == "") {
            ui->statusbar->showMessage("First Center X coordinate = NULL");
            return;
        }
        info.c0.x = ui->First_Center_X_Coordinate_Input->text().toInt();
        if (ui->First_Center_Y_Coordinate_Input->text() == "") {
            ui->statusbar->showMessage("First Center Y coordinate = NULL");
            return;
        }
        info.c0.y = ui->First_Center_Y_Coordinate_Input->text().toInt();
        if (ui->Second_Center_X_Coordinate_Input->text() == "") {
            ui->statusbar->showMessage("First Center X coordinate = NULL");
            return;
        }
        info.c1.x = ui->Second_Center_X_Coordinate_Input->text().toInt();
        if (ui->Second_Center_Y_Coordinate_Input->text() == "") {
            ui->statusbar->showMessage("First Center X coordinate = NULL");
            return;
        }
        info.c1.y = ui->Second_Center_Y_Coordinate_Input->text().toInt();
        if (ui->First_Radius_Input->text() == "") {
            ui->statusbar->showMessage("First Radius = NUll");
            return;
        }
        info.r0 = ui->First_Radius_Input->text().toInt();
        if (ui->Second_Radius_Input->text() == "") {
            ui->statusbar->showMessage("Second Radius = NULL");
            return;
        }
        info.r1 = ui->Second_Radius_Input->text().toInt();
        info.ginfo = NSStructures::GInfoConstructor::get_radial(info.c0, info.c1, info.r0, info.r1,
                                                                 0, 1, info.cont_b, info.cont_f);
    }

    if (info.colorspace == NoColorspaceType) {
        ui->statusbar->showMessage("Colorspace - NULL");
        return;
    } else if (info.colorspace == Rainbow) {
        info.c = {(LONG)0xFFff0000, (LONG)0xFFffa500, (LONG)0xFFffff00, (LONG)0xFF008000, (LONG)0xFF0000ff, (LONG)0xFFFF00FF};
        info.p = {0.0,0.2,0.4,0.6,0.8,1};
        info.n_colors = 6;
        info.ginfo.shading.function.set_linear_interpolation({0xFFff0000, 0xFFffa500, 0xFFffff00, 0xFF008000, 0xFF0000ff, 0xFFFF00FF}
                                                             , {0.0f,0.2f,0.4f,0.6f,0.8f,1.0f});
    } else if (info.colorspace == BlackAndWhite) {
        info.c = {(LONG)0xFFFFFFFF, (LONG)0xFF000000};
        info.p = {0.0, 1};
        info.n_colors = 2;
        info.ginfo.shading.function.set_linear_interpolation({0xFFFFFFFF, 0xFF000000}, {0.0f, 1.0f});
    } else if (info.colorspace == RedAndBlue) {
        info.c = {(LONG)0xFFFF0000, (LONG)0xFF0000FF};
        info.p = {0.0, 1};
        info.n_colors = 2;
        info.ginfo.shading.function.set_linear_interpolation({0xFFFF0000, 0xFF0000FF}, {0.0f, 1.0f});
    } else if (info.colorspace == Pastel) {
        info.c = {(LONG)0xfff39189, (LONG)0xff046582};
        info.p = {0.0, 1};
        info.n_colors = 2;
        info.ginfo.shading.function.set_linear_interpolation({0xfff39189, 0xff046582}, {0.0f, 1.0f});
    }

    QImage pm(400, 400, QImage::Format_RGB888);
    GenerateImg(pm,  points, info);
    ui->lable_test->setPixmap(QPixmap::fromImage(pm));
    ui->lable_test->setScaledContents(true);
}


void MainWindow::on_Continue_Shading_Forward_clicked(bool checked)
{
    info.cont_f = checked;
}


void MainWindow::on_checkBox_2_clicked(bool checked)
{
    info.cont_b = checked;
}


void MainWindow::on_Continue_Shading_Forward_2_clicked(bool checked)
{
    info.cont_f = checked;
}


void MainWindow::on_Continue_Shading_Backward_2_clicked(bool checked)
{
    info.cont_b = checked;
}

