#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPixmap>
#include <iostream>

#include "../../../../DesktopEditor/graphics/pro/Graphics.h"
#include "../../../../PdfFile/PdfFile.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow), img("test.png"), lable(new QLabel)
{
	ui->setupUi(this);
	ui->lable_test->setStyleSheet("QLabel { background-color : white;}");
	ui->stackedWidget->setCurrentIndex(0);
	ui->stackedWidget_2->setCurrentIndex(0);
	ui->statusbar->showMessage("Linear");
	ui->Rainbow_Colorspace_Radio_Button->setChecked(true);
	info.colorspace = Rainbow;
}

MainWindow::~MainWindow()
{
	delete ui;
}

void CleanupFunction(void *data)
{
	delete[] data;
}

const int COORD_SIZE_MM = 100;

QImage GenerateImg(std::vector<Point> &points, Info &info, const int& w, const int& h)
{
	NSGraphics::IGraphicsRenderer *pRasterRenderer = NSGraphics::Create();
	NSFonts::IFontManager *fmp = NSFonts::NSFontManager::Create();
	pRasterRenderer->SetFontManager(fmp);
	int nRasterW = w;
	int nRasterH = h;
	BYTE *pData = new BYTE[4 * nRasterW * nRasterH];

	unsigned int back = 0xffffff;
	unsigned int *pData32 = (unsigned int *)pData;
	unsigned int *pData32End = pData32 + nRasterW * nRasterH;
	// дефолтный тон должен быть прозрачным, а не белым
	while (pData32 < pData32End)
		*pData32++ = back;

	CBgraFrame oFrame;
	oFrame.put_Data(pData);
	oFrame.put_Width(nRasterW);
	oFrame.put_Height(nRasterH);
	oFrame.put_Stride(4 * nRasterW);

	pRasterRenderer->CreateFromBgraFrame(&oFrame);
	pRasterRenderer->SetSwapRGB(false);

	double dW_MM = COORD_SIZE_MM;
	double dH_MM = COORD_SIZE_MM;

	pRasterRenderer->put_Width(dW_MM);
	pRasterRenderer->put_Height(dH_MM);

	NSStructures::GradientInfo ginfo = info.ginfo;
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
	if (points.size() > 0)
	{
		pRasterRenderer->PathCommandMoveTo(points[0].x, points[0].y);
		for (uint i = 1; i < points.size(); i++)
		{
			pRasterRenderer->PathCommandLineTo(points[i].x, points[i].y);
		}
	}
	pRasterRenderer->Fill();
	pRasterRenderer->EndCommand(c_nPathType);
	pRasterRenderer->PathCommandEnd();

	QImage img = QImage(pData, nRasterW, nRasterH, QImage::Format_RGBA8888, CleanupFunction);
	oFrame.put_Data(NULL);
	return img;
}

void MainWindow::on_actionLinear_Gradient_triggered()
{
	ui->stackedWidget->setCurrentIndex(0);
	ui->stackedWidget_2->setCurrentIndex(0);
	ui->statusbar->showMessage("Linear");
	info.gradient = Linear;
	info.gradient_type = c_BrushTypePathNewLinearGradient;
	points = {{0, 0}, {500, 0}, {500, 500}, {0, 500}};
}

void MainWindow::on_actionRadial_Gradient_triggered()
{
	ui->stackedWidget->setCurrentIndex(1);
	ui->stackedWidget_2->setCurrentIndex(1);
	ui->statusbar->showMessage("Radial");
	info.gradient = Radial;
	info.gradient_type = c_BrushTypePathRadialGradient;
	points = {{0, 0}, {500, 0}, {500, 500}, {0, 500}};
}

void MainWindow::on_actionTriangle_Gradient_triggered()
{
	ui->stackedWidget->setCurrentIndex(2);
	ui->stackedWidget_2->setCurrentIndex(2);
	ui->statusbar->showMessage("Triangle");
	info.gradient = Triangle;
	info.gradient_type = c_BrushTypeTriagnleMeshGradient;
	points = {{0, 0}, {500, 0}, {500, 500}, {0, 500}};
}

void MainWindow::on_actionTriangle_Parametric_Gradient_triggered()
{
	ui->stackedWidget->setCurrentIndex(2);
	ui->stackedWidget_2->setCurrentIndex(3);
	ui->statusbar->showMessage("Triangle Parametric");
	info.gradient = TriangleParametric;
	info.gradient_type = c_BrushTypeTriagnleMeshGradient;
	points = {{0, 0}, {500, 0}, {500, 500}, {0, 500}};
}

void MainWindow::on_actionCoons_Patch_Gradient_triggered()
{
	ui->stackedWidget->setCurrentIndex(3);
	ui->stackedWidget_2->setCurrentIndex(4);
	ui->statusbar->showMessage("Coons Patch");
	info.gradient = CoonsPatch;
	info.gradient_type = c_BrushTypePathNewLinearGradient;
	points = {{0, 0}, {500, 0}, {500, 500}, {0, 500}};
}

void MainWindow::on_actionCoons_Patch_Parametric_triggered()
{
	ui->stackedWidget->setCurrentIndex(3);
	ui->stackedWidget_2->setCurrentIndex(5);
	ui->statusbar->showMessage("Coons Patch Parametric");
	info.gradient = CoonsPatchParametric;
	info.gradient_type = c_BrushTypeCurveGradient;
	points = {{0, 0}, {500, 0}, {500, 500}, {0, 500}};
}

void MainWindow::on_actionTensor_Coons_Patch_Gradient_triggered()
{
	ui->stackedWidget->setCurrentIndex(4);
	ui->stackedWidget_2->setCurrentIndex(6);
	ui->statusbar->showMessage("Tensor Coons Patch");
	info.gradient = TensorCoonsPatch;
	info.gradient_type = c_BrushTypePathNewLinearGradient;
	points = {{0, 0}, {500, 0}, {500, 500}, {0, 500}};
}

void MainWindow::on_actionTensor_Coons_Patch_Parametric_triggered()
{
	ui->stackedWidget->setCurrentIndex(4);
	ui->stackedWidget_2->setCurrentIndex(7);
	ui->statusbar->showMessage("Tensor Coons Patch Parametric");
	info.gradient = TensorCoonsPatchParametric;
	info.gradient_type = c_BrushTypeTensorCurveGradient;
	points = {{0, 0}, {500, 0}, {500, 500}, {0, 500}};
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
	if (info.gradient == Linear)
	{
		if (ui->First_X_Coordinate_Input->text() == "")
		{
			ui->statusbar->showMessage("First X coordinate = NULL");
			return;
		}
		info.p0.x = ui->First_X_Coordinate_Input->text().toInt();
		if (ui->First_Y_Coordinate_Input->text() == "")
		{
			ui->statusbar->showMessage("First Y coordinate = NULL");
			return;
		}
		info.p0.y = ui->First_Y_Coordinate_Input->text().toInt();
		if (ui->Second_X_Coordinate_Input->text() == "")
		{
			ui->statusbar->showMessage("First X coordinate = NULL");
			return;
		}
		info.p1.x = ui->Second_X_Coordinate_Input->text().toInt();
		if (ui->Second_Y_Coordinate_Input->text() == "")
		{
			ui->statusbar->showMessage("First X coordinate = NULL");
			return;
		}
		info.p1.y = ui->Second_Y_Coordinate_Input->text().toInt();
		info.ginfo = NSStructures::GInfoConstructor::get_linear(info.p0, info.p1, 0, 1, info.cont_b, info.cont_f);
	}
	else if (info.gradient == Radial)
	{
		if (ui->First_Center_X_Coordinate_Input->text() == "")
		{
			ui->statusbar->showMessage("First Center X coordinate = NULL");
			return;
		}
		info.c0.x = ui->First_Center_X_Coordinate_Input->text().toInt();
		if (ui->First_Center_Y_Coordinate_Input->text() == "")
		{
			ui->statusbar->showMessage("First Center Y coordinate = NULL");
			return;
		}
		info.c0.y = ui->First_Center_Y_Coordinate_Input->text().toInt();
		if (ui->Second_Center_X_Coordinate_Input->text() == "")
		{
			ui->statusbar->showMessage("First Center X coordinate = NULL");
			return;
		}
		info.c1.x = ui->Second_Center_X_Coordinate_Input->text().toInt();
		if (ui->Second_Center_Y_Coordinate_Input->text() == "")
		{
			ui->statusbar->showMessage("First Center X coordinate = NULL");
			return;
		}
		info.c1.y = ui->Second_Center_Y_Coordinate_Input->text().toInt();
		if (ui->First_Radius_Input->text() == "")
		{
			ui->statusbar->showMessage("First Radius = NUll");
			return;
		}
		info.r0 = ui->First_Radius_Input->text().toInt();
		if (ui->Second_Radius_Input->text() == "")
		{
			ui->statusbar->showMessage("Second Radius = NULL");
			return;
		}
		info.r1 = ui->Second_Radius_Input->text().toInt();
		info.ginfo = NSStructures::GInfoConstructor::get_radial(info.c0, info.c1, info.r0, info.r1, 0, 1, info.cont_b, info.cont_f);
	}
	else if (info.gradient == Triangle || info.gradient == TriangleParametric)
	{
		if (ui->First_Vertex_X_Coordinate_Input->text() == "")
		{
			ui->statusbar->showMessage("First Vertex X coordinate = NULL");
			return;
		}
		info.triangle[0].x = ui->First_Vertex_X_Coordinate_Input->text().toInt();
		if (ui->First_Vertex_Y_Coordinate_Input->text() == "")
		{
			ui->statusbar->showMessage("First Vertex Y coordinate = NULL");
			return;
		}
		info.triangle[0].y = ui->First_Vertex_Y_Coordinate_Input->text().toInt();
		if (ui->Second_Vertex_X_Coordinate_Input->text() == "")
		{
			ui->statusbar->showMessage("Second Vertex X coordinate = NULL");
			return;
		}
		info.triangle[1].x = ui->Second_Vertex_X_Coordinate_Input->text().toInt();
		if (ui->Second_Vertex_Y_Coordinate_Input->text() == "")
		{
			ui->statusbar->showMessage("Second Vertex Y coordinate = NULL");
			return;
		}
		info.triangle[1].y = ui->Second_Vertex_Y_Coordinate_Input->text().toInt();
		if (ui->Third_Vertex_X_Coordinate_Input->text() == "")
		{
			ui->statusbar->showMessage("Third Vertex X coordinate = NULL");
			return;
		}
		info.triangle[2].x = ui->Third_Vertex_X_Coordinate_Input->text().toInt();
		if (ui->Third_Vertex_Y_Coordinate_Input->text() == "")
		{
			ui->statusbar->showMessage("Third Vertex Y coordinate = NULL");
			return;
		}
		info.triangle[2].y = ui->Third_Vertex_Y_Coordinate_Input->text().toInt();
		info.ginfo = NSStructures::GInfoConstructor::get_triangle(info.triangle,
																  {{255, 0, 0}, {0, 255, 0}, {0, 0, 255}},
																  {},
																  info.gradient == TriangleParametric);
		points = {};
		for (auto p : info.triangle)
		{
			points.push_back({p.x, p.y});
		}
	}
	else if (info.gradient == CoonsPatch || info.gradient == CoonsPatchParametric)
	{
		if (ui->First_Vertex_X_Coordinate_Input_3->text() == "")
		{
			ui->statusbar->showMessage("First Vertex X Coordinate = NULL");
			return;
		}
		info.curve[0].x = 5 * ui->First_Vertex_X_Coordinate_Input_3->text().toInt();
		if (ui->First_Vertex_Y_Coordinate_Input_3->text() == "")
		{
			ui->statusbar->showMessage("First Vertex Y Coordinate = NULL");
			return;
		}
		info.curve[0].y = 5 * ui->First_Vertex_Y_Coordinate_Input_3->text().toInt();
		if (ui->First_X_Coordinate_First_Edge->text() == "")
		{
			ui->statusbar->showMessage("First X Coordinate First Edge = NULL");
			return;
		}
		info.curve[1].x = 5 * ui->First_X_Coordinate_First_Edge->text().toInt();
		if (ui->First_Y_Coordinate_First_Edge->text() == "")
		{
			ui->statusbar->showMessage("First Y Coordinate First Edge = NULL");
			return;
		}
		info.curve[1].y = 5 * ui->First_Y_Coordinate_First_Edge->text().toInt();
		if (ui->Second_X_Coordinate_First_Edge->text() == "")
		{
			ui->statusbar->showMessage("Second X Coordinate First Edge = NULL");
			return;
		}
		info.curve[2].x = 5 * ui->Second_X_Coordinate_First_Edge->text().toInt();
		if (ui->Second_Y_Coordinate_First_Edge->text() == "")
		{
			ui->statusbar->showMessage("Second Y Coordinate First Edge = NULL");
			return;
		}
		info.curve[2].y = 5 * ui->Second_Y_Coordinate_First_Edge->text().toInt();
		if (ui->Second_Vertex_X_Coordinate_Input_3->text() == "")
		{
			ui->statusbar->showMessage("Second Vertex X Coordinate = NULL");
			return;
		}
		info.curve[3].x = 5 * ui->Second_Vertex_X_Coordinate_Input_3->text().toInt();
		if (ui->Second_Vertex_Y_Coordinate_Input_3->text() == "")
		{
			ui->statusbar->showMessage("Second Vertex Y Coordinate = NULL");
			return;
		}
		info.curve[3].y = 5 * ui->Second_Vertex_Y_Coordinate_Input_3->text().toInt();
		if (ui->First_X_Coordinate_Second_Edge->text() == "")
		{
			ui->statusbar->showMessage("First X Coordinate Second Edge = NULL");
			return;
		}
		info.curve[4].x = 5 * ui->First_X_Coordinate_Second_Edge->text().toInt();
		if (ui->First_Y_Coordinate_Second_Edge->text() == "")
		{
			ui->statusbar->showMessage("First Y Coordinate Second Edge = NULL");
			return;
		}
		info.curve[4].y = 5 * ui->First_Y_Coordinate_Second_Edge->text().toInt();
		if (ui->Second_X_Coordinate_Second_Edge->text() == "")
		{
			ui->statusbar->showMessage("Second X Coordinate Second Edge = NULL");
			return;
		}
		info.curve[5].x = 5 * ui->Second_X_Coordinate_Second_Edge->text().toInt();
		if (ui->Second_Y_Coordinate_Second_Edge->text() == "")
		{
			ui->statusbar->showMessage("Second Y Coordinate Second Edge = NULL");
			return;
		}
		info.curve[5].y = 5 * ui->Second_Y_Coordinate_Second_Edge->text().toInt();
		if (ui->Third_Vertex_X_Coordinate_Input_3->text() == "")
		{
			ui->statusbar->showMessage("Third Vertex X Coordinate = NULL");
			return;
		}
		info.curve[6].x = 5 * ui->Third_Vertex_X_Coordinate_Input_3->text().toInt();
		if (ui->Third_Vertex_Y_Coordinate_Input_3->text() == "")
		{
			ui->statusbar->showMessage("Third Vertex Y Coordinate = NULL");
			return;
		}
		info.curve[6].y = 5 * ui->Third_Vertex_Y_Coordinate_Input_3->text().toInt();
		if (ui->First_X_Coordinate_Third_Edge->text() == "")
		{
			ui->statusbar->showMessage("First X Coordinate Third Edge = NULL");
			return;
		}
		info.curve[7].x = 5 * ui->First_X_Coordinate_Third_Edge->text().toInt();
		if (ui->First_Y_Coordinate_Third_Edge->text() == "")
		{
			ui->statusbar->showMessage("First Y Coordinate Third Edge = NULL");
			return;
		}
		info.curve[7].y = 5 * ui->First_Y_Coordinate_Third_Edge->text().toInt();
		if (ui->Second_X_Coordinate_Third_Edge->text() == "")
		{
			ui->statusbar->showMessage("Second X Coordinate Third Edge = NULL");
			return;
		}
		info.curve[8].x = 5 * ui->Second_X_Coordinate_Third_Edge->text().toInt();
		if (ui->Second_Y_Coordinate_Third_Edge->text() == "")
		{
			ui->statusbar->showMessage("Second Y Coordinate Third Edge = NULL");
			return;
		}
		info.curve[8].y = 5 * ui->Second_Y_Coordinate_Third_Edge->text().toInt();
		if (ui->Fourth_Vertex_X_Coordinate_Input_3->text() == "")
		{
			ui->statusbar->showMessage("Fourth Vertex X Coordinate = NULL");
			return;
		}
		info.curve[9].x = 5 * ui->Fourth_Vertex_X_Coordinate_Input_3->text().toInt();
		if (ui->Fourth_Vertex_Y_Coordinate_Input_3->text() == "")
		{
			ui->statusbar->showMessage("Fourth Vertex Y Coordinate = NULL");
			return;
		}
		info.curve[9].y = 5 * ui->Fourth_Vertex_Y_Coordinate_Input_3->text().toInt();
		if (ui->First_X_Coordinate_Fourth_Edge->text() == "")
		{
			ui->statusbar->showMessage("First X Coordinate Fourth Edge = NULL");
			return;
		}
		info.curve[10].x = 5 * ui->First_X_Coordinate_Fourth_Edge->text().toInt();
		if (ui->First_Y_Coordinate_Fourth_Edge->text() == "")
		{
			ui->statusbar->showMessage("First Y Coordinate Fourth Edge = NULL");
			return;
		}
		info.curve[10].y = 5 * ui->First_Y_Coordinate_Fourth_Edge->text().toInt();
		if (ui->Second_X_Coordinate_Fourth_Edge->text() == "")
		{
			ui->statusbar->showMessage("Second X Coordinate Fourth Edge = NULL");
			return;
		}
		info.curve[11].x = 5 * ui->Second_X_Coordinate_Fourth_Edge->text().toInt();
		if (ui->Second_Y_Coordinate_Fourth_Edge->text() == "")
		{
			ui->statusbar->showMessage("Second Y Coordinate Fourth Edge = NULL");
			return;
		}
		info.curve[11].y = 5 * ui->Second_Y_Coordinate_Fourth_Edge->text().toInt();
		info.ginfo = NSStructures::GInfoConstructor::get_curve(
			info.curve,
			{0, 0.5, 1, 0.5},
			{{0, 0, 255}, {255, 0, 255},
			 {255, 0, 0}, {0, 255, 0}},
			info.gradient == CoonsPatchParametric
			);
	}
	else if (info.gradient == TensorCoonsPatch || info.gradient == TensorCoonsPatchParametric)
	{
		if (ui->First_X_Coordinate_First_Edge_3->text() == "")
		{
			ui->statusbar->showMessage("First X Coordinate First Edge = NULL");
			return;
		}
		info.tensorcurve[0][0].x = 5 * ui->First_X_Coordinate_First_Edge_3->text().toInt();
		if (ui->First_Y_Coordinate_First_Edge_3->text() == "")
		{
			ui->statusbar->showMessage("First Y Coordinate First Edge = NULL");
			return;
		}
		info.tensorcurve[0][0].y = 5 * ui->First_Y_Coordinate_First_Edge_3->text().toInt();
		if (ui->Second_X_Coordinate_First_Edge_3->text() == "")
		{
			ui->statusbar->showMessage("Second X Coordinate First Edge = NULL");
			return;
		}
		info.tensorcurve[0][1].x = 5 * ui->Second_X_Coordinate_First_Edge_3->text().toInt();
		if (ui->Second_Y_Coordinate_First_Edge_3->text() == "")
		{
			ui->statusbar->showMessage("Second Y Coordinate First Edge = NULL");
			return;
		}
		info.tensorcurve[0][1].y = 5 * ui->Second_Y_Coordinate_First_Edge_3->text().toInt();
		if (ui->Third_X_Coordinate_First_Edge_3->text() == "")
		{
			ui->statusbar->showMessage("Third X Coordinate First Edge = NULL");
			return;
		}
		info.tensorcurve[0][2].x = 5 * ui->Third_X_Coordinate_First_Edge_3->text().toInt();
		if (ui->Third_Y_Coordinate_First_Edge_3->text() == "")
		{
			ui->statusbar->showMessage("Third Y Coordinate First Edge = NULL");
			return;
		}
		info.tensorcurve[0][2].y = 5 * ui->Third_Y_Coordinate_First_Edge_3->text().toInt();
		if (ui->Fourth_X_Coordinate_First_Edge_3->text() == "")
		{
			ui->statusbar->showMessage("Fourth X Coordinate First Edge = NULL");
			return;
		}
		info.tensorcurve[0][3].x = 5 * ui->Fourth_X_Coordinate_First_Edge_3->text().toInt();
		if (ui->Fourth_Y_Coordinate_First_Edge_3->text() == "")
		{
			ui->statusbar->showMessage("Fourth Y Coordinate First Edge = NULL");
			return;
		}
		info.tensorcurve[0][3].y = 5 * ui->Fourth_Y_Coordinate_First_Edge_3->text().toInt();
		if (ui->First_X_Coordinate_Second_Edge_3->text() == "")
		{
			ui->statusbar->showMessage("First X Coordinate Second Edge = NULL");
			return;
		}
		info.tensorcurve[1][0].x = 5 * ui->First_X_Coordinate_Second_Edge_3->text().toInt();
		if (ui->First_Vertex_Y_Coordinate_Input_3->text() == "")
		{
			ui->statusbar->showMessage("First Y Coordinate Second Edge = NULL");
			return;
		}
		info.tensorcurve[1][0].y = 5 * ui->First_Y_Coordinate_Second_Edge_3->text().toInt();
		if (ui->Second_X_Coordinate_Second_Edge_3->text() == "")
		{
			ui->statusbar->showMessage("Second X Coordinate Second Edge = NULL");
			return;
		}
		info.tensorcurve[1][1].x = 5 * ui->Second_X_Coordinate_Second_Edge_3->text().toInt();
		if (ui->Second_Y_Coordinate_Second_Edge_3->text() == "")
		{
			ui->statusbar->showMessage("Second Y Coordinate Second Edge = NULL");
			return;
		}
		info.tensorcurve[1][1].y = 5 * ui->Second_Y_Coordinate_Second_Edge_3->text().toInt();
		if (ui->Third_X_Coordinate_Second_Edge_3->text() == "")
		{
			ui->statusbar->showMessage("Third X Coordinate Second Edge = NULL");
			return;
		}
		info.tensorcurve[1][2].x = 5 * ui->Third_X_Coordinate_Second_Edge_3->text().toInt();
		if (ui->Third_Y_Coordinate_Second_Edge_3->text() == "")
		{
			ui->statusbar->showMessage("Third Y Coordinate Second Edge = NULL");
			return;
		}
		info.tensorcurve[1][2].y = 5 * ui->Third_Y_Coordinate_Second_Edge_3->text().toInt();
		if (ui->Fourth_X_Coordinate_Second_Edge_3->text() == "")
		{
			ui->statusbar->showMessage("Fourth X Coordinate Second Edge = NULL");
			return;
		}
		info.tensorcurve[1][3].x = 5 * ui->Fourth_X_Coordinate_Second_Edge_3->text().toInt();
		if (ui->Fourth_Y_Coordinate_Second_Edge_3->text() == "")
		{
			ui->statusbar->showMessage("Fourth Y Coordinate Second Edge = NULL");
			return;
		}
		info.tensorcurve[1][3].y = 5 * ui->Fourth_Y_Coordinate_Second_Edge_3->text().toInt();
		if (ui->First_X_Coordinate_Third_Edge_3->text() == "")
		{
			ui->statusbar->showMessage("First X Coordinate Third Edge = NULL");
			return;
		}
		info.tensorcurve[2][0].x = 5 * ui->First_X_Coordinate_Third_Edge_3->text().toInt();
		if (ui->First_Y_Coordinate_Third_Edge_3->text() == "")
		{
			ui->statusbar->showMessage("First Y Coordinate Third Edge = NULL");
			return;
		}
		info.tensorcurve[2][0].y = 5 * ui->First_Y_Coordinate_Third_Edge_3->text().toInt();
		if (ui->Second_X_Coordinate_Third_Edge_3->text() == "")
		{
			ui->statusbar->showMessage("Second X Coordinate Third Edge = NULL");
			return;
		}
		info.tensorcurve[2][1].x = 5 * ui->Second_Y_Coordinate_Third_Edge_3->text().toInt();
		if (ui->Second_Y_Coordinate_Third_Edge_3->text() == "")
		{
			ui->statusbar->showMessage("Second Y Coordinate Third Edge = NULL");
			return;
		}
		info.tensorcurve[2][1].y = 5 * ui->Second_Y_Coordinate_Third_Edge_3->text().toInt();
		if (ui->Third_X_Coordinate_Third_Edge_3->text() == "")
		{
			ui->statusbar->showMessage("Third X Coordinate Third Edge = NULL");
			return;
		}
		info.tensorcurve[2][2].x = 5 * ui->Third_X_Coordinate_Third_Edge_3->text().toInt();
		if (ui->Third_Y_Coordinate_Third_Edge_3->text() == "")
		{
			ui->statusbar->showMessage("Third Y Coordinate Third Edge = NULL");
			return;
		}
		info.tensorcurve[2][2].y = 5 * ui->Third_Y_Coordinate_Third_Edge_3->text().toInt();
		if (ui->Fourth_X_Coordinate_Third_Edge_3->text() == "")
		{
			ui->statusbar->showMessage("Fourth X Coordinate Third Edge = NULL");
			return;
		}
		info.tensorcurve[2][3].x = 5 * ui->Fourth_X_Coordinate_Third_Edge_3->text().toInt();
		if (ui->Fourth_Y_Coordinate_Third_Edge_3->text() == "")
		{
			ui->statusbar->showMessage("Fourth Y Coordinate Third Edge = NULL");
			return;
		}
		info.tensorcurve[2][3].y = 5 * ui->Fourth_Y_Coordinate_Third_Edge_3->text().toInt();
		if (ui->First_X_Coordinate_Fourth_Edge_3->text() == "")
		{
			ui->statusbar->showMessage("First X Coordinate Fourth Edge = NULL");
			return;
		}
		info.tensorcurve[3][0].x = 5 * ui->First_X_Coordinate_Fourth_Edge_3->text().toInt();
		if (ui->First_Y_Coordinate_Fourth_Edge_3->text() == "")
		{
			ui->statusbar->showMessage("First Y Coordinate Fourth Edge = NULL");
			return;
		}
		info.tensorcurve[3][0].y = 5 * ui->First_Y_Coordinate_Fourth_Edge_3->text().toInt();
		if (ui->Second_X_Coordinate_Fourth_Edge_3->text() == "")
		{
			ui->statusbar->showMessage("Second X Coordinate Fourth Edge = NULL");
			return;
		}
		info.tensorcurve[3][1].x = 5 * ui->Second_X_Coordinate_Fourth_Edge_3->text().toInt();
		if (ui->Second_Y_Coordinate_Fourth_Edge_3->text() == "")
		{
			ui->statusbar->showMessage("Second Y Coordinate Fourth Edge = NULL");
			return;
		}
		info.tensorcurve[3][1].y = 5 * ui->Second_Y_Coordinate_Fourth_Edge_3->text().toInt();
		if (ui->Third_X_Coordinate_Fourth_Edge_3->text() == "")
		{
			ui->statusbar->showMessage("Third X Coordinate Fourth Edge = NULL");
			return;
		}
		info.tensorcurve[3][2].x = 5 * ui->Third_X_Coordinate_Fourth_Edge_3->text().toInt();
		if (ui->Third_Y_Coordinate_Fourth_Edge_3->text() == "")
		{
			ui->statusbar->showMessage("Third X Coordinate Fourth Edge = NULL");
			return;
		}
		info.tensorcurve[3][2].y = 5 * ui->Third_Y_Coordinate_Fourth_Edge_3->text().toInt();
		if (ui->Fourth_X_Coordinate_Fourth_Edge_3->text() == "")
		{
			ui->statusbar->showMessage("Fourth X Coordinate Fourth Edge = NULL");
			return;
		}
		info.tensorcurve[3][3].x = 5 * ui->Fourth_X_Coordinate_Fourth_Edge_3->text().toInt();
		if (ui->Fourth_Y_Coordinate_Fourth_Edge_3->text() == "")
		{
			ui->statusbar->showMessage("Fourth Y Coordinate Fourth Edge = NULL");
			return;
		}
		info.tensorcurve[3][3].y = 5 * ui->Fourth_Y_Coordinate_Fourth_Edge_3->text().toInt();
		info.ginfo = NSStructures::GInfoConstructor::get_tensor_curve(
			info.tensorcurve,
			{{0, 0.5}, {1, 0.5}},
			{{{0, 0, 255}, {255, 0, 255}}, {{255, 0, 0}, {0, 255, 0}}},
			info.gradient == TensorCoonsPatchParametric
			);
	}

	if (info.colorspace == NoColorspaceType)
	{
		ui->statusbar->showMessage("Colorspace - NULL");
		return;
	}
	else if (info.colorspace == Rainbow)
	{
		info.c = {(LONG)0xFFff0000, (LONG)0xFFffa500, (LONG)0xFFffff00, (LONG)0xFF008000, (LONG)0xFF0000ff, (LONG)0xFFFF00FF};
		info.p = {0.0, 0.2, 0.4, 0.6, 0.8, 1};
		info.n_colors = 6;
		info.ginfo.shading.function.set_linear_interpolation({0xFFff0000, 0xFFffa500, 0xFFffff00, 0xFF008000, 0xFF0000ff, 0xFFFF00FF}, {0.0f, 0.2f, 0.4f, 0.6f, 0.8f, 1.0f});
	}
	else if (info.colorspace == BlackAndWhite)
	{
		info.c = {(LONG)0xFFFFFFFF, (LONG)0xFF000000};
		info.p = {0.0, 1};
		info.n_colors = 2;
		info.ginfo.shading.function.set_linear_interpolation({0xFFFFFFFF, 0xFF000000}, {0.0f, 1.0f});
	}
	else if (info.colorspace == RedAndBlue)
	{
		info.c = {(LONG)0xFFFF0000, (LONG)0xFF0000FF};
		info.p = {0.0, 1};
		info.n_colors = 2;
		info.ginfo.shading.function.set_linear_interpolation({0xFFFF0000, 0xFF0000FF}, {0.0f, 1.0f});
	}
	else if (info.colorspace == Pastel)
	{
		info.c = {(LONG)0xfff39189, (LONG)0xff046582};
		info.p = {0.0, 1};
		info.n_colors = 2;
		info.ginfo.shading.function.set_linear_interpolation({0xfff39189, 0xff046582}, {0.0f, 1.0f});
	}

	QImage pm = GenerateImg(points, info, ui->lable_test->width(), ui->lable_test->height());
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

inline void ClampCoords(QLineEdit* edit)
{
	if (edit->text().toInt() < 0)
		edit->setText("0");
	if (edit->text().toInt() > COORD_SIZE_MM)
		edit->setText(QString::number(COORD_SIZE_MM));
}

void MainWindow::on_First_X_Coordinate_Input_editingFinished()
{
	ClampCoords(ui->First_X_Coordinate_Input);
}

void MainWindow::on_First_Y_Coordinate_Input_editingFinished()
{
	ClampCoords(ui->First_Y_Coordinate_Input);
}

void MainWindow::on_Second_X_Coordinate_Input_editingFinished()
{
	ClampCoords(ui->Second_X_Coordinate_Input);
}

void MainWindow::on_Second_Y_Coordinate_Input_editingFinished()
{
	ClampCoords(ui->Second_Y_Coordinate_Input);
}

void MainWindow::on_First_Center_X_Coordinate_Input_editingFinished()
{
	ClampCoords(ui->First_Center_X_Coordinate_Input);
}

void MainWindow::on_First_Center_Y_Coordinate_Input_editingFinished()
{
	ClampCoords(ui->First_Center_Y_Coordinate_Input);
}

void MainWindow::on_Second_Center_X_Coordinate_Input_editingFinished()
{
	ClampCoords(ui->Second_Center_X_Coordinate_Input);
}

void MainWindow::on_Second_Center_Y_Coordinate_Input_editingFinished()
{
	ClampCoords(ui->Second_Center_Y_Coordinate_Input);
}

void MainWindow::on_First_Radius_Input_editingFinished()
{
	ClampCoords(ui->First_Radius_Input);
}

void MainWindow::on_Second_Radius_Input_editingFinished()
{
	ClampCoords(ui->Second_Radius_Input);
}

void MainWindow::on_First_Vertex_X_Coordinate_Input_editingFinished()
{
	ClampCoords(ui->First_Vertex_X_Coordinate_Input);
}


void MainWindow::on_First_Vertex_Y_Coordinate_Input_editingFinished()
{
	ClampCoords(ui->First_Vertex_Y_Coordinate_Input);
}


void MainWindow::on_Second_Vertex_X_Coordinate_Input_editingFinished()
{
	ClampCoords(ui->Second_Vertex_X_Coordinate_Input);
}


void MainWindow::on_Second_Vertex_Y_Coordinate_Input_editingFinished()
{
	ClampCoords(ui->Second_Vertex_Y_Coordinate_Input);
}


void MainWindow::on_Third_Vertex_X_Coordinate_Input_editingFinished()
{
	ClampCoords(ui->Third_Vertex_X_Coordinate_Input);
}


void MainWindow::on_Third_Vertex_Y_Coordinate_Input_editingFinished()
{
	ClampCoords(ui->Third_Vertex_Y_Coordinate_Input);
}


void MainWindow::on_First_Vertex_X_Coordinate_Input_3_editingFinished()
{
	ClampCoords(ui->First_Vertex_X_Coordinate_Input_3);
}


void MainWindow::on_First_Vertex_Y_Coordinate_Input_3_editingFinished()
{
	ClampCoords(ui->First_Vertex_Y_Coordinate_Input_3);
}


void MainWindow::on_Second_Vertex_X_Coordinate_Input_3_editingFinished()
{
	ClampCoords(ui->Second_Vertex_X_Coordinate_Input_3);
}


void MainWindow::on_Second_Vertex_Y_Coordinate_Input_3_editingFinished()
{
	ClampCoords(ui->Second_Vertex_Y_Coordinate_Input_3);
}


void MainWindow::on_Third_Vertex_X_Coordinate_Input_3_editingFinished()
{
	ClampCoords(ui->Third_Vertex_X_Coordinate_Input_3);
}


void MainWindow::on_Third_Vertex_Y_Coordinate_Input_3_editingFinished()
{
	ClampCoords(ui->Third_Vertex_Y_Coordinate_Input_3);
}


void MainWindow::on_Fourth_Vertex_X_Coordinate_Input_3_editingFinished()
{
	ClampCoords(ui->Fourth_Vertex_X_Coordinate_Input_3);
}


void MainWindow::on_Fourth_Vertex_Y_Coordinate_Input_3_editingFinished()
{
	ClampCoords(ui->Fourth_Vertex_Y_Coordinate_Input_3);
}


void MainWindow::on_First_X_Coordinate_First_Edge_editingFinished()
{
	ClampCoords(ui->First_X_Coordinate_First_Edge);
}


void MainWindow::on_First_Y_Coordinate_First_Edge_editingFinished()
{
	ClampCoords(ui->First_Y_Coordinate_First_Edge);
}


void MainWindow::on_Second_X_Coordinate_First_Edge_editingFinished()
{
	ClampCoords(ui->Second_X_Coordinate_First_Edge);
}


void MainWindow::on_Second_Y_Coordinate_First_Edge_editingFinished()
{
	ClampCoords(ui->Second_Y_Coordinate_First_Edge);
}


void MainWindow::on_First_X_Coordinate_Second_Edge_editingFinished()
{
	ClampCoords(ui->First_X_Coordinate_Second_Edge);
}


void MainWindow::on_First_Y_Coordinate_Second_Edge_editingFinished()
{
	ClampCoords(ui->First_Y_Coordinate_Second_Edge);
}


void MainWindow::on_Second_X_Coordinate_Second_Edge_editingFinished()
{
	ClampCoords(ui->Second_X_Coordinate_Second_Edge);
}


void MainWindow::on_Second_Y_Coordinate_Second_Edge_editingFinished()
{
	ClampCoords(ui->Second_Y_Coordinate_Second_Edge);
}


void MainWindow::on_First_X_Coordinate_Third_Edge_editingFinished()
{
	ClampCoords(ui->First_X_Coordinate_Third_Edge);
}


void MainWindow::on_First_Y_Coordinate_Third_Edge_editingFinished()
{
	ClampCoords(ui->First_Y_Coordinate_Third_Edge);
}


void MainWindow::on_Second_X_Coordinate_Third_Edge_editingFinished()
{
	ClampCoords(ui->Second_X_Coordinate_Third_Edge);
}


void MainWindow::on_Second_Y_Coordinate_Third_Edge_editingFinished()
{
	ClampCoords(ui->Second_Y_Coordinate_Third_Edge);
}


void MainWindow::on_First_X_Coordinate_Fourth_Edge_editingFinished()
{
	ClampCoords(ui->First_X_Coordinate_Fourth_Edge);
}


void MainWindow::on_First_Y_Coordinate_Fourth_Edge_editingFinished()
{
	ClampCoords(ui->First_Y_Coordinate_Fourth_Edge);
}


void MainWindow::on_Second_X_Coordinate_Fourth_Edge_editingFinished()
{
	ClampCoords(ui->Second_X_Coordinate_Fourth_Edge);
}


void MainWindow::on_Second_Y_Coordinate_Fourth_Edge_editingFinished()
{
	ClampCoords(ui->Second_Y_Coordinate_Fourth_Edge);
}


void MainWindow::on_First_X_Coordinate_First_Edge_3_editingFinished()
{
	ClampCoords(ui->First_X_Coordinate_First_Edge_3);
}


void MainWindow::on_First_Y_Coordinate_First_Edge_3_editingFinished()
{
	ClampCoords(ui->First_Y_Coordinate_First_Edge_3);
}


void MainWindow::on_Second_X_Coordinate_First_Edge_3_editingFinished()
{
	ClampCoords(ui->Second_X_Coordinate_First_Edge_3);
}


void MainWindow::on_Second_Y_Coordinate_First_Edge_3_editingFinished()
{
	ClampCoords(ui->Second_Y_Coordinate_First_Edge_3);
}


void MainWindow::on_Third_X_Coordinate_First_Edge_3_editingFinished()
{
	ClampCoords(ui->Third_X_Coordinate_First_Edge_3);
}


void MainWindow::on_Third_Y_Coordinate_First_Edge_3_editingFinished()
{
	ClampCoords(ui->Third_Y_Coordinate_First_Edge_3);
}


void MainWindow::on_Fourth_X_Coordinate_First_Edge_3_editingFinished()
{
	ClampCoords(ui->Fourth_X_Coordinate_First_Edge_3);
}


void MainWindow::on_Fourth_Y_Coordinate_First_Edge_3_editingFinished()
{
	ClampCoords(ui->Fourth_Y_Coordinate_First_Edge_3);
}


void MainWindow::on_First_X_Coordinate_Second_Edge_3_editingFinished()
{
	ClampCoords(ui->First_X_Coordinate_Second_Edge_3);
}


void MainWindow::on_First_Y_Coordinate_Second_Edge_3_editingFinished()
{
	ClampCoords(ui->First_Y_Coordinate_Second_Edge_3);
}


void MainWindow::on_Second_X_Coordinate_Second_Edge_3_editingFinished()
{
	ClampCoords(ui->Second_X_Coordinate_Second_Edge_3);
}


void MainWindow::on_Second_Y_Coordinate_Second_Edge_3_editingFinished()
{
	ClampCoords(ui->Second_Y_Coordinate_Second_Edge_3);
}


void MainWindow::on_Third_X_Coordinate_Second_Edge_3_editingFinished()
{
	ClampCoords(ui->Third_X_Coordinate_Second_Edge_3);
}


void MainWindow::on_Third_Y_Coordinate_Second_Edge_3_editingFinished()
{
	ClampCoords(ui->Third_Y_Coordinate_Second_Edge_3);
}


void MainWindow::on_Fourth_X_Coordinate_Second_Edge_3_editingFinished()
{
	ClampCoords(ui->Fourth_X_Coordinate_Second_Edge_3);
}


void MainWindow::on_Fourth_Y_Coordinate_Second_Edge_3_editingFinished()
{
	ClampCoords(ui->Fourth_Y_Coordinate_Second_Edge_3);
}


void MainWindow::on_First_X_Coordinate_Third_Edge_3_editingFinished()
{
	ClampCoords(ui->First_X_Coordinate_Third_Edge_3);
}


void MainWindow::on_First_Y_Coordinate_Third_Edge_3_editingFinished()
{
	ClampCoords(ui->First_Y_Coordinate_Third_Edge_3);
}


void MainWindow::on_Second_X_Coordinate_Third_Edge_3_editingFinished()
{
	ClampCoords(ui->Second_X_Coordinate_Third_Edge_3);
}


void MainWindow::on_Second_Y_Coordinate_Third_Edge_3_editingFinished()
{
	ClampCoords(ui->Second_Y_Coordinate_Third_Edge_3);
}


void MainWindow::on_Third_X_Coordinate_Third_Edge_3_editingFinished()
{
	ClampCoords(ui->Third_X_Coordinate_Third_Edge_3);
}


void MainWindow::on_Third_Y_Coordinate_Third_Edge_3_editingFinished()
{
	ClampCoords(ui->Third_Y_Coordinate_Third_Edge_3);
}


void MainWindow::on_Fourth_X_Coordinate_Third_Edge_3_editingFinished()
{
	ClampCoords(ui->Fourth_X_Coordinate_Third_Edge_3);
}


void MainWindow::on_Fourth_Y_Coordinate_Third_Edge_3_editingFinished()
{
	ClampCoords(ui->Fourth_Y_Coordinate_Third_Edge_3);
}


void MainWindow::on_First_X_Coordinate_Fourth_Edge_3_editingFinished()
{
	ClampCoords(ui->First_X_Coordinate_Fourth_Edge_3);
}


void MainWindow::on_First_Y_Coordinate_Fourth_Edge_3_editingFinished()
{
	ClampCoords(ui->First_Y_Coordinate_Fourth_Edge_3);
}


void MainWindow::on_Second_X_Coordinate_Fourth_Edge_3_editingFinished()
{
	ClampCoords(ui->Second_X_Coordinate_Fourth_Edge_3);
}


void MainWindow::on_Second_Y_Coordinate_Fourth_Edge_3_editingFinished()
{
	ClampCoords(ui->Second_Y_Coordinate_Fourth_Edge_3);
}


void MainWindow::on_Third_X_Coordinate_Fourth_Edge_3_editingFinished()
{
	ClampCoords(ui->Third_X_Coordinate_Fourth_Edge_3);
}


void MainWindow::on_Third_Y_Coordinate_Fourth_Edge_3_editingFinished()
{
	ClampCoords(ui->Third_Y_Coordinate_Fourth_Edge_3);
}


void MainWindow::on_Fourth_X_Coordinate_Fourth_Edge_3_editingFinished()
{
	ClampCoords(ui->Fourth_X_Coordinate_Fourth_Edge_3);
}


void MainWindow::on_Fourth_Y_Coordinate_Fourth_Edge_3_editingFinished()
{
	ClampCoords(ui->Fourth_Y_Coordinate_Fourth_Edge_3);
}

