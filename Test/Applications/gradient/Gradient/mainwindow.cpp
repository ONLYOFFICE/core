#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPixmap>
#include "../../../../DesktopEditor/graphics/pro/Graphics.h"

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
    QImage pm(1000, 1000,QImage::Format_RGB888);
//    std::vector<Point> points {{40, 40}, {150, 40}, {140, 140}, {40, 140}};
//    std::vector<Point> points {{150, 170}, {100, 160}, {150, 150}, {160, 100}, {170, 150}, {230, 160}, {170, 170}, {150, 170}};
    std::vector<Point> points;
    points = drawCircle1(100, 130.,100.,40);
    GenerateImg(pm, c_BrushTypePathDiamondGradient, 0, points);
    //setColor2(pm, 0x0000FF);
   //pm.invertPixels();
    ui->lable_test->setPixmap(QPixmap::fromImage(pm));
    ui->lable_test->setScaledContents(true);
    ui->lable_test->resize(pm.size());
    this->resize(pm.size());
    pm.save("test.bmp");
}

MainWindow::~MainWindow()
{
    delete ui;
}


void GenerateImg(QImage &img, int grad, double angle, std::vector<Point> points) {
    NSGraphics::IGraphicsRenderer* pRasterRenderer = NSGraphics::Create();
    pRasterRenderer->SetFontManager(NULL);
    int nRasterW = img.size().width();
    int nRasterH = img.size().height();
    BYTE* pData = new BYTE[4 * nRasterW * nRasterH];

    unsigned int back = 0xffffff;
    unsigned int* pData32 = (unsigned int*)pData;
    unsigned int* pData32End = pData32 + nRasterW * nRasterH;
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

    NSStructures::GradientInfo ginfo;
    ginfo.periods = 0.5;
    ginfo.discrete_step = 0.00;
    ginfo.setAngleDegrees(-45);
    ginfo.xsize = 1;
    pRasterRenderer->put_BrushGradInfo(ginfo);
    pRasterRenderer->put_PenColor(0xFF000000);
    //pRasterRenderer->put_BrushColor1(0xFF00FF00);
    LONG c[] = {0xFFff0000, 0xFFffa500, 0xFFffff00, 0xFF008000, 0xFF0000ff, 0xFFFF00FF};
    double p[] = {0.0,0.2,0.4,0.6,0.8,1};
    pRasterRenderer->put_BrushLinearAngle(angle);
    pRasterRenderer->put_BrushType(grad);
    pRasterRenderer->put_BrushGradientColors(c, p, 6);
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
