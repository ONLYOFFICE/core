#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPixmap>
#include <iostream>
#include "../../../../DesktopEditor/graphics/pro/Graphics.h"

std::vector<Point> drawCircle1(int n, double cx, double cy, double r) {
    std::vector<Point> res;
    for (int i = 0; i < n; i++) {
        double x = cx + r * cos(i * 8 * atan(1) / n);
        double y = cy + r * sin(i * 8 * atan(1) /  n);
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
}



MainWindow::~MainWindow()
{
    delete ui;
}


void GenerateImg(QImage &img, std::vector<Point> &points, Info &info) {
    NSGraphics::IGraphicsRenderer* pRasterRenderer = NSGraphics::Create();
    pRasterRenderer->SetFontManager(NULL);
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
    pRasterRenderer->put_BrushType(info.gradient_type);
    pRasterRenderer->put_BrushGradientColors(c, p, info.n_colors);
    pRasterRenderer->PathCommandStart();
    pRasterRenderer->BeginCommand(c_nPathType);
    if (points.size() > 0) {
        pRasterRenderer->PathCommandMoveTo(points[0].x, points[0].y);
        for (uint i = 1; i < points.size(); i++) {
            pRasterRenderer->PathCommandLineTo(points[i].x, points[i].y);
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

void MainWindow::on_RenderPic_clicked()
{

    QImage pm(400, 400,QImage::Format_RGB888);
    GenerateImg(pm,  points, info);
    //setColor2(pm, 0x0000FF);
   //pm.invertPixels();
    ui->lable_test->setPixmap(QPixmap::fromImage(pm));
    ui->lable_test->setScaledContents(true);
    ui->lable_test->resize(pm.size());
    pm.save("test.bmp");
}



void MainWindow::on_AngleSlider_sliderMoved(int position)
{
    double angleT = 360. / 100 * position;
    ui->lable_angle->setText(std::to_string(angleT).c_str());
    info.ginfo.setAngleDegrees(angleT);
    this->on_RenderPic_clicked();
}

void MainWindow::on_OffsetSlider_sliderMoved(int position)
{
    double offset = ((double)position - 50.0) / 25.0;
    ui->lable_offset->setText(std::to_string(offset).c_str());
    info.ginfo.linoffset = offset;
    this->on_RenderPic_clicked();
}

void MainWindow::on_StretchSlide_sliderMoved(int position)
{
    double stretch = pow(2.0, (position - 50.) / 25.0);
    ui->lable_stretch->setText(std::to_string(stretch).c_str());
    info.ginfo.linstretch = stretch;
    this->on_RenderPic_clicked();
}

void MainWindow::on_DiscreteStepSlider_sliderMoved(int position)
{
    if (position == 0) {
        info.ginfo.discrete_step = 0.0;
        ui->label_discrete->setText("Continious");
        this->on_RenderPic_clicked();
        return;
    }
    info.ginfo.setStepByNum(position);
    ui->label_discrete->setText(std::to_string(position).c_str());
    this->on_RenderPic_clicked();
}

void MainWindow::on_PathType_itemDoubleClicked(QListWidgetItem *item)
{
    int tmp = info.gradient_type;
    on_PathType_itemClicked(item);
    this->on_RenderPic_clicked();
    //info.gradient_type = tmp;
}

void MainWindow::on_PathType_itemClicked(QListWidgetItem *item)
{
    if (item->text() == "Circle") {
        points = drawCircle1(100, 53, 53, 50);
    }
    else if (item->text() == "Square") {
        points = {{0, 0}, {105, 0}, {105, 105}, {0, 105}};
    }
    else if (item->text() == "Triangle") {
        points = {{5, 10}, {40, 100}, {100, 1}};
        /*
         * Пока чтобы тестить прямую интерполяцию цвета, нужно редактировать код
         * И вычтавить  info.ginfo.shading.shading_type как NSStructures::ShadingInfo::TriangleInterpolation
         * Чтобы тестить параметрическую нужно выставить NSStructures::ShadingInfo::Parametric
         * С опцией Parametric можно тестить другие градиенты.
         *
        */
        info.gradient_type = c_BrushTypeTensorCurveGradient;
        info.ginfo.shading.shading_type = NSStructures::ShadingInfo::Parametric;
        info.ginfo.shading.triangle = {{5 * 3.84, 10 * 3.84}, {40 * 3.84, 100 * 3.84}, {100 * 3.84, 1 * 3.84}};
        info.ginfo.shading.triangle_parameters = {0.0f, 0.5f,  1.0f} ;
        info.ginfo.shading.triangle_colors = {{255,0,0, 255}, {255,255,0,255 },  {0,255,0,255}};

        info.ginfo.shading.patch = {
            {{10, 80}, {20, 70}, {0, 20}, {10,10}},
            {{25, 75}, {40, 60}, {40, 40}, {20, 0}},
            {{90, 70}, {60,60}, {60, 40}, {70, 20}},
            {{80,80}, {70, 70}, {90, 20}, {80,80}}
        };
        info.ginfo.shading.patch_parameters = {
            {0 , 1},
            {1, 0.5}
        };
        info.ginfo.shading.patch_colors = {
            {{255, 0, 0}, {0, 255, 255}},
            {{0, 255, 0}, {0, 0, 255}}
        };
        float mult = 4.0;
        for (int i = 0; i < info.ginfo.shading.patch.size(); i++)
        {
            for (int j = 0; j < info.ginfo.shading.patch[0].size(); j++)
            {
                info.ginfo.shading.patch[i][j] = info.ginfo.shading.patch[i][j]  * mult;
            }
        }
    }
}

void MainWindow::on_GradientType_itemDoubleClicked(QListWidgetItem *item)
{
    on_GradientType_itemClicked(item);
    on_RenderPic_clicked();
}

void MainWindow::on_GradientType_itemClicked(QListWidgetItem *item)
{
    if (item->text() == "Linear") {
        info.gradient_type = c_BrushTypePathNewLinearGradient;
    }
    else if (item->text() == "Radial") {
        info.gradient_type = c_BrushTypePathRadialGradient;
    }
    else if (item->text() == "Diamond") {
        info.gradient_type = c_BrushTypePathDiamondGradient;
    }
    else if (item->text() == "Conical") {
        info.gradient_type = c_BrushTypePathConicalGradient;
    }
}


void MainWindow::on_LittleRadiusSlider_sliderMoved(int position)
{
    double llr = (double)position / 100.0;
    ui->lable_little_radius->setText(std::to_string(llr).c_str());
    info.ginfo.littleRadius = llr;
    this->on_RenderPic_clicked();
}

void MainWindow::on_LargeRadiusSlider_sliderMoved(int position)
{
    double lgr = (double)position / 100.0;
    ui->lable_large_radius->setText(std::to_string(lgr).c_str());
    info.ginfo.largeRadius = lgr;
    this->on_RenderPic_clicked();
}

void MainWindow::on_xcenterSlider_sliderMoved(int position)
{
    double xc = ((double)position - 50.0) / 25.0;
    ui->lable_xcenter->setText(std::to_string(xc).c_str());
    info.ginfo.centerX = xc;
    this->on_RenderPic_clicked();
}

void MainWindow::on_ycenterSlider_sliderMoved(int position)
{
    double yc = ((double)position - 50.0) / 25.0;
    ui->lable_ycenter->setText(std::to_string(yc).c_str());
    info.ginfo.centerY = yc;
    this->on_RenderPic_clicked();
}

void MainWindow::on_XSizeSlider_sliderMoved(int position)
{
    double xsize = pow(2.0, (position - 50.) / 25.0);
    ui->lable_xsize->setText(std::to_string(xsize).c_str());
    info.ginfo.xsize = xsize;
    this->on_RenderPic_clicked();
}

void MainWindow::on_YSizeSlider_sliderMoved(int position)
{
    double ysize = pow(2.0, (position - 50.) / 25.0);
    ui->lable_ysize->setText(std::to_string(ysize).c_str());
    info.ginfo.ysize = ysize;
    this->on_RenderPic_clicked();
}

void MainWindow::on_ColorSpaces_itemClicked(QListWidgetItem *item)
{

    if (item->text() == "Rainbow") {
        info.c = {(LONG)0xFFff0000, (LONG)0xFFffa500, (LONG)0xFFffff00, (LONG)0xFF008000, (LONG)0xFF0000ff, (LONG)0xFFFF00FF};
        info.p = {0.0,0.2,0.4,0.6,0.8,1};
        info.n_colors = 6;
        info.ginfo.shading.function.set_linear_interpolation({0xFFff0000, 0xFFffa500, 0xFFffff00, 0xFF008000, 0xFF0000ff, 0xFFFF00FF}
                                                             , {0.0f,0.2f,0.4f,0.6f,0.8f,1.0f});
    }
    else if (item->text() == "Black and white") {
        info.c = {(LONG)0xFFFFFFFF, (LONG)0xFF000000};
        info.p = {0.0, 1};
        info.n_colors = 2;
        info.ginfo.shading.function.set_linear_interpolation({0xFFFFFFFF, 0xFF000000}, {0.0f, 1.0f});
    }
    else if (item->text() == "Red Blue") {
        info.c = {(LONG)0xFFFF0000, (LONG)0xFF0000FF};
        info.p = {0.0, 1};
        info.n_colors = 2;
        info.ginfo.shading.function.set_linear_interpolation({0xFFFF0000, 0xFF0000FF}, {0.0f, 1.0f});
    }
    else if (item->text() == "Pastel") {
        info.c = {(LONG)0xfff39189, (LONG)0xff046582};
        info.p = {0.0, 1};
        info.n_colors = 2;
        info.ginfo.shading.function.set_linear_interpolation({0xfff39189, 0xff046582}, {0.0f, 1.0f});
    }
}

void MainWindow::on_ColorSpaces_itemDoubleClicked(QListWidgetItem *item)
{
    on_ColorSpaces_itemClicked(item);
    on_RenderPic_clicked();
}

void MainWindow::on_Reflected_CheckBox_clicked(bool checked)
{
    info.ginfo.reflected = checked;
    on_RenderPic_clicked();
}

void MainWindow::on_PeriodicCheckBox_clicked(bool checked)
{
    info.ginfo.periodic = checked;
    on_RenderPic_clicked();
}
