#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPixmap>
#include <iostream>
#include "../../../../DesktopEditor/graphics/pro/Graphics.h"
#include "../../../../DesktopEditor/fontengine/ApplicationFontsWorker.h"
#include "../../../../DesktopEditor/common/Directory.h"
#include "../../../../PdfReader/PdfReader.h"

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



void MainWindow::on_RenderPic_clicked()
{
    /*
    CApplicationFontsWorker oWorker;
    oWorker.m_sDirectory = NSFile::GetProcessDirectory() + L"/fonts_cache";
    oWorker.m_bIsNeedThumbnails = false;
    if (!NSDirectory::Exists(oWorker.m_sDirectory))
        NSDirectory::CreateDirectory(oWorker.m_sDirectory);

    NSFonts::IApplicationFonts* pFonts = oWorker.Check();
    PdfReader::CPdfReader PDFREADER(pFonts);
    PDFREADER.LoadFromFile(L"test.pdf");
    int page = ui->lineEdit->text().toInt();
    PDFREADER.ConvertToRaster(page + 1, L"testpdf.bmp", 1);
    */

   //   QImage pm("testpdf.bmp");
    QImage pm(400, 400, QImage::Format_RGB888);
    GenerateImg(pm,  points, info);
    //setColor2(pm, 0x0000FF);
   //pm.invertPixels();
    ui->lable_test->setPixmap(QPixmap::fromImage(pm));
    ui->lable_test->setScaledContents(true);
   // ui->lable_test->resize(pm.size());
    //pm.save("test.bmp");

   // pFonts->Release();
}


void MainWindow::on_GradientType_itemDoubleClicked(QListWidgetItem *item)
{
    on_GradientType_itemClicked(item);
    on_RenderPic_clicked();
}

void MainWindow::on_GradientType_itemClicked(QListWidgetItem *item)
{
    points = {{0, 0}, {105,  0}, {105, 105}, {0, 105}};
    if (item->text() == "Linear") {
        info.gradient_type = c_BrushTypePathNewLinearGradient;
        info.ginfo = NSStructures::GInfoConstructor::get_linear(info.p0, info.p1, 0, 1, info.cont_b, info.cont_f);
    }
    else if (item->text() == "Radial") {
        info.gradient_type = c_BrushTypePathRadialGradient;
        info.ginfo = NSStructures::GInfoConstructor::get_radial(info.c0, info.c1, info.r0, info.r1,
                                                                0, 1, info.cont_b, info.cont_f);
    }
    else if (item->text() == "Triangle") {
        info.gradient_type = c_BrushTypeTriagnleMeshGradient;
        info.ginfo = NSStructures::GInfoConstructor::get_triangle(
                    info.triangle,
                    {{255, 0, 0}, {0, 255, 0}, {0, 0, 255}},
                    {}, false
                    );
        points = {};
        for (auto p : info.triangle)
        {
            points.push_back({p.x / 3.84, p.y / 3.84});
        }
    }
    else if (item->text() == "Functional" ) {
        info.gradient_type = c_BrushTypePathNewLinearGradient;
        info.ginfo = NSStructures::GInfoConstructor::get_functional(0, 1, 0, 1,
                                                                            {400, 0, 0, 400, 0, 0});
    }
    else if (item->text() == "TriangleParametric" ) {
        info.gradient_type = c_BrushTypeTriagnleMeshGradient;
        info.ginfo = NSStructures::GInfoConstructor::get_triangle(
                    info.triangle,
                    {{255, 0, 0}, {0, 255, 0}, {0, 0, 255}},
                    {0.f, 0.4f, 1.f}, true
                    );
        points = {};
        for (auto p : info.triangle)
        {
            points.push_back({p.x / 3.84, p.y / 3.84});
        }
    }
    else if (item->text() == "CoonsPatch" ) {
        info.gradient_type = c_BrushTypePathNewLinearGradient;
        info.ginfo = NSStructures::GInfoConstructor::get_curve(
                    {
                     {100, 300}, {50, 250}, {150, 150}, {100, 100},
                     {150, 50}, {250, 150}, {300, 100}, {250, 150},
                     {350, 250}, {300, 300}, {250, 350}, {150,250}
                    },
                    {0, 0.5, 1, 0.5},
                    {{0, 0, 255}, {255, 0, 255},
                     {255, 0, 0}, {0, 255, 0}},
                        false
                    );
    }
    else if (item->text() == "TesnorCoonsPatch" ) {
        info.gradient_type = c_BrushTypePathNewLinearGradient;
        info.ginfo = NSStructures::GInfoConstructor::get_tensor_curve(
                    {
                        {{100, 300}, {150, 250}, {50, 150}, {100,100}},
                        {{150, 250}, {170, 230}, {170, 170}, {50, 150}},
                        {{350, 250}, {230, 230}, {230, 170}, {150, 250}},
                        {{300, 300}, {250, 250}, {350, 150}, {300, 100}}
                    },
                    {{0, 0.5}, {1, 0.5}},
                    {{{0, 0, 255}, {255, 0, 255}}, {{255, 0, 0}, {0, 255, 0}}},
                    false
                    );
    }
    else if (item->text() == "CoonsPatchParametric") {
        info.gradient_type = c_BrushTypeCurveGradient;
        info.ginfo = NSStructures::GInfoConstructor::get_curve(
                    {
                     {100, 300}, {50, 250}, {150, 150}, {100, 100},
                     {150, 50}, {250, 150}, {300, 100}, {250, 150},
                     {350, 250}, {300, 300}, {250, 350}, {150,250}
                    },
                    {0, 0.5, 1, 0.5},
                    {{0, 0, 255}, {255, 0, 255},
                     {255, 0, 0}, {0, 255, 0}},
                        true
                    );
    }
    else if (item->text() == "TensorCoonsPatchParametric") {
        info.gradient_type = c_BrushTypeTensorCurveGradient;
        info.ginfo = NSStructures::GInfoConstructor::get_tensor_curve(
                    {
                        {{100, 300}, {150, 250}, {50, 150}, {100,100}},
                        {{150, 250}, {170, 230}, {170, 170}, {50, 150}},
                        {{350, 250}, {230, 230}, {230, 170}, {150, 250}},
                        {{300, 300}, {250, 250}, {350, 150}, {300, 100}}
                    },
                    {{0, 0.5}, {1, 0.5}},
                    {{{0, 0, 255}, {255, 0, 255}}, {{255, 0, 0}, {0, 255, 0}}},
                    true
                    );
    }
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


void MainWindow::on_Point1X_sliderMoved(int position)
{
    info.p0.x= position;
    if (info.gradient_type != c_BrushTypePathNewLinearGradient) return;
    info.ginfo = NSStructures::GInfoConstructor::get_linear(info.p0, info.p1, 0, 1, info.cont_b, info.cont_f);
    on_RenderPic_clicked();
}

void MainWindow::on_Point1Y_sliderMoved(int position)
{
    info.p0.y = position;
    if (info.gradient_type != c_BrushTypePathNewLinearGradient) return;
    info.ginfo = NSStructures::GInfoConstructor::get_linear(info.p0, info.p1, 0, 1, info.cont_b, info.cont_f);
    on_RenderPic_clicked();
}

void MainWindow::on_Point2X_sliderMoved(int position)
{
    info.p1.x = position;
    if (info.gradient_type != c_BrushTypePathNewLinearGradient) return;
    info.ginfo = NSStructures::GInfoConstructor::get_linear(info.p0, info.p1, 0, 1, info.cont_b, info.cont_f);
    on_RenderPic_clicked();
}

void MainWindow::on_Point2Y_sliderMoved(int position)
{
    info.p1.y = position;
    if (info.gradient_type != c_BrushTypePathNewLinearGradient) return;
    info.ginfo = NSStructures::GInfoConstructor::get_linear(info.p0, info.p1, 0, 1, info.cont_b, info.cont_f);
    on_RenderPic_clicked();
}


void MainWindow::on_CenterX0_valueChanged(int value)
{
    info.c0.x = value;
    if (info.gradient_type != c_BrushTypePathRadialGradient) return;
    info.ginfo = NSStructures::GInfoConstructor::get_radial(info.c0, info.c1, info.r0, info.r1,
                                                            0, 1, info.cont_b, info.cont_f);
    on_RenderPic_clicked();
}

void MainWindow::on_CenterY0_valueChanged(int value)
{
    info.c0.y = value;
    if (info.gradient_type != c_BrushTypePathRadialGradient) return;
    info.ginfo = NSStructures::GInfoConstructor::get_radial(info.c0, info.c1, info.r0, info.r1,
                                                            0, 1, info.cont_b, info.cont_f);
    on_RenderPic_clicked();
}

void MainWindow::on_CenterX1_valueChanged(int value)
{
    info.c1.x = value;
    if (info.gradient_type != c_BrushTypePathRadialGradient) return;
    info.ginfo = NSStructures::GInfoConstructor::get_radial(info.c0, info.c1, info.r0, info.r1,
                                                            0, 1, info.cont_b, info.cont_f);
    on_RenderPic_clicked();
}

void MainWindow::on_CenterY1_valueChanged(int value)
{
    info.c1.y= value;
    if (info.gradient_type != c_BrushTypePathRadialGradient) return;
    info.ginfo = NSStructures::GInfoConstructor::get_radial(info.c0, info.c1, info.r0, info.r1,
                                                            0, 1, info.cont_b, info.cont_f);
    on_RenderPic_clicked();
}

void MainWindow::on_r0slider_valueChanged(int value)
{
    info.r0 = value;
    if (info.gradient_type != c_BrushTypePathRadialGradient) return;
    info.ginfo = NSStructures::GInfoConstructor::get_radial(info.c0, info.c1, info.r0, info.r1,
                                                            0, 1, info.cont_b, info.cont_f);
    on_RenderPic_clicked();
}

void MainWindow::on_r1slider_valueChanged(int value)
{
    info.r1 = value;
    if (info.gradient_type != c_BrushTypePathRadialGradient) return;
    info.ginfo = NSStructures::GInfoConstructor::get_radial(info.c0, info.c1, info.r0, info.r1,
                                                            0, 1, info.cont_b, info.cont_f);
    on_RenderPic_clicked();
}

void MainWindow::on_ContinueForvard_clicked(bool checked)
{
    info.cont_f= checked;
    if (info.gradient_type == c_BrushTypePathNewLinearGradient)
    {
        info.ginfo = NSStructures::GInfoConstructor::get_linear(info.p0, info.p1, 0, 1, info.cont_b, info.cont_f);
        on_RenderPic_clicked();
    }
    if (info.gradient_type == c_BrushTypePathRadialGradient)
    {
        info.ginfo = NSStructures::GInfoConstructor::get_radial(info.c0, info.c1, info.r0, info.r1,
                                                                0, 1, info.cont_b, info.cont_f);
        on_RenderPic_clicked();
    }
}

void MainWindow::on_ContinueBack_clicked(bool checked)
{
    info.cont_b = checked;
    if (info.gradient_type == c_BrushTypePathNewLinearGradient)
    {
        info.ginfo = NSStructures::GInfoConstructor::get_linear(info.p0, info.p1, 0, 1, info.cont_b, info.cont_f);
        on_RenderPic_clicked();
    }
    if (info.gradient_type == c_BrushTypePathRadialGradient)
    {
        info.ginfo = NSStructures::GInfoConstructor::get_radial(info.c0, info.c1, info.r0, info.r1,
                                                                0, 1, info.cont_b, info.cont_f);
        on_RenderPic_clicked();
    }
}



void MainWindow::on_TrianglePoint1X_sliderMoved(int position)
{

    info.triangle[0].x = position;
    if (info.ginfo.shading.shading_type == NSStructures::ShadingInfo::TriangleInterpolation)
    {
        info.gradient_type = c_BrushTypeTriagnleMeshGradient;
        info.ginfo = NSStructures::GInfoConstructor::get_triangle(
                    info.triangle,
                    {{255, 0, 0}, {0, 255, 0}, {0, 0, 255}},
                    {}, false
                    );
        points = {};
        for (auto p : info.triangle)
        {
            points.push_back({p.x / 3.84, p.y / 3.84});
        }
        on_RenderPic_clicked();
        return;
    }
    else if (info.gradient_type == c_BrushTypeTriagnleMeshGradient)
    {
        info.gradient_type = c_BrushTypeTriagnleMeshGradient;
        info.ginfo = NSStructures::GInfoConstructor::get_triangle(
                    info.triangle,
                    {{255, 0, 0}, {0, 255, 0}, {0, 0, 255}},
                    {0.f, 0.4f, 1.f}, true
                    );
        points = {};
        for (auto p : info.triangle)
        {
            points.push_back({p.x / 3.84, p.y / 3.84});
        }
        on_RenderPic_clicked();
    }
}

void MainWindow::on_TrianglePoint1Y_sliderMoved(int position)
{
    info.triangle[0].y = position;
    if (info.ginfo.shading.shading_type == NSStructures::ShadingInfo::TriangleInterpolation)
    {
        info.gradient_type = c_BrushTypeTriagnleMeshGradient;
        info.ginfo = NSStructures::GInfoConstructor::get_triangle(
                    info.triangle,
                    {{255, 0, 0}, {0, 255, 0}, {0, 0, 255}},
                    {}, false
                    );
        points = {};
        for (auto p : info.triangle)
        {
            points.push_back({p.x / 3.84, p.y / 3.84});
        }
        on_RenderPic_clicked();
        return;
    }
    else if (info.gradient_type == c_BrushTypeTriagnleMeshGradient)
    {
        info.gradient_type = c_BrushTypeTriagnleMeshGradient;
        info.ginfo = NSStructures::GInfoConstructor::get_triangle(
                    info.triangle,
                    {{255, 0, 0}, {0, 255, 0}, {0, 0, 255}},
                    {0.f, 0.4f, 1.f}, true
                    );
        points = {};
        for (auto p : info.triangle)
        {
            points.push_back({p.x / 3.84, p.y / 3.84});
        }
        on_RenderPic_clicked();
    }
}

void MainWindow::on_TrianglePoint2X_sliderMoved(int position)
{
    info.triangle[1].x = position;
    if (info.ginfo.shading.shading_type == NSStructures::ShadingInfo::TriangleInterpolation)
    {
        info.gradient_type = c_BrushTypeTriagnleMeshGradient;
        info.ginfo = NSStructures::GInfoConstructor::get_triangle(
                    info.triangle,
                    {{255, 0, 0}, {0, 255, 0}, {0, 0, 255}},
                    {}, false
                    );
        points = {};
        for (auto p : info.triangle)
        {
            points.push_back({p.x / 3.84, p.y / 3.84});
        }
        on_RenderPic_clicked();
        return;
    }
    else if (info.gradient_type == c_BrushTypeTriagnleMeshGradient)
    {
        info.gradient_type = c_BrushTypeTriagnleMeshGradient;
        info.ginfo = NSStructures::GInfoConstructor::get_triangle(
                    info.triangle,
                    {{255, 0, 0}, {0, 255, 0}, {0, 0, 255}},
                    {0.f, 0.4f, 1.f}, true
                    );
        points = {};
        for (auto p : info.triangle)
        {
            points.push_back({p.x / 3.84, p.y / 3.84});
        }
        on_RenderPic_clicked();
    }
}

void MainWindow::on_TrianglePoint2Y_sliderMoved(int position)
{
    info.triangle[1].y = position;
    if (info.ginfo.shading.shading_type == NSStructures::ShadingInfo::TriangleInterpolation)
    {
        info.gradient_type = c_BrushTypeTriagnleMeshGradient;
        info.ginfo = NSStructures::GInfoConstructor::get_triangle(
                    info.triangle,
                    {{255, 0, 0}, {0, 255, 0}, {0, 0, 255}},
                    {}, false
                    );
        points = {};
        for (auto p : info.triangle)
        {
            points.push_back({p.x / 3.84, p.y / 3.84});
        }
        on_RenderPic_clicked();
        return;
    }
    else if (info.gradient_type == c_BrushTypeTriagnleMeshGradient)
    {
        info.gradient_type = c_BrushTypeTriagnleMeshGradient;
        info.ginfo = NSStructures::GInfoConstructor::get_triangle(
                    info.triangle,
                    {{255, 0, 0}, {0, 255, 0}, {0, 0, 255}},
                    {0.f, 0.4f, 1.f}, true
                    );
        points = {};
        for (auto p : info.triangle)
        {
            points.push_back({p.x / 3.84, p.y / 3.84});
        }
        on_RenderPic_clicked();
    }
}

void MainWindow::on_TrianglePoint3X_sliderMoved(int position)
{
    info.triangle[2].x = position;
    if (info.ginfo.shading.shading_type == NSStructures::ShadingInfo::TriangleInterpolation)
    {
        info.gradient_type = c_BrushTypeTriagnleMeshGradient;
        info.ginfo = NSStructures::GInfoConstructor::get_triangle(
                    info.triangle,
                    {{255, 0, 0}, {0, 255, 0}, {0, 0, 255}},
                    {}, false
                    );
        points = {};
        for (auto p : info.triangle)
        {
            points.push_back({p.x / 3.84, p.y / 3.84});
        }
        on_RenderPic_clicked();
        return;
    }
    else if (info.gradient_type == c_BrushTypeTriagnleMeshGradient)
    {
        info.gradient_type = c_BrushTypeTriagnleMeshGradient;
        info.ginfo = NSStructures::GInfoConstructor::get_triangle(
                    info.triangle,
                    {{255, 0, 0}, {0, 255, 0}, {0, 0, 255}},
                    {0.f, 0.4f, 1.f}, true
                    );
        points = {};
        for (auto p : info.triangle)
        {
            points.push_back({p.x / 3.84, p.y / 3.84});
        }
        on_RenderPic_clicked();
    }
}

void MainWindow::on_TrianglePoint3Y_sliderMoved(int position)
{
    info.triangle[2].y = position;
    if (info.ginfo.shading.shading_type == NSStructures::ShadingInfo::TriangleInterpolation)
    {
        info.gradient_type = c_BrushTypeTriagnleMeshGradient;
        info.ginfo = NSStructures::GInfoConstructor::get_triangle(
                    info.triangle,
                    {{255, 0, 0}, {0, 255, 0}, {0, 0, 255}},
                    {}, false
                    );
        points = {};
        for (auto p : info.triangle)
        {
            points.push_back({p.x / 3.84, p.y / 3.84});
        }
        on_RenderPic_clicked();
        return;
    }
    else if (info.gradient_type == c_BrushTypeTriagnleMeshGradient)
    {
        info.gradient_type = c_BrushTypeTriagnleMeshGradient;
        info.ginfo = NSStructures::GInfoConstructor::get_triangle(
                    info.triangle,
                    {{255, 0, 0}, {0, 255, 0}, {0, 0, 255}},
                    {0.f, 0.4f, 1.f}, true
                    );
        points = {};
        for (auto p : info.triangle)
        {
            points.push_back({p.x / 3.84, p.y / 3.84});
        }
        on_RenderPic_clicked();
    }
}

void MainWindow::on_LeftButton_clicked()
{
    int page = ui->lineEdit->text().toInt();
    ui->lineEdit->setText(QString::number(page - 1));
    on_RenderPic_clicked();
}

void MainWindow::on_RightButton_clicked()
{
    int page = ui->lineEdit->text().toInt();
    ui->lineEdit->setText(QString::number(page + 1));
    on_RenderPic_clicked();
}
