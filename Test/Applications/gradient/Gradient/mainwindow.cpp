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
	on_actionLinear_Gradient_triggered();
	ui->Rainbow_Colorspace_Radio_Button->setChecked(true);
	connect(ui->lable_test, SIGNAL(mousePressed()), this, SLOT(on_label_test_clicked()));
}

MainWindow::~MainWindow()
{
	delete ui;
}

void CleanupFunction(void *data)
{
	delete[] data;
}

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

void MainWindow::InitializeColors(bool Triangle)
{
	ui->label_80->SetColor(QColor(Qt::red));
	ui->label_81->SetColor(QColor(Qt::green));
	ui->label_114->SetColor(QColor(Qt::blue));
	if (!Triangle)
	{
		ui->label_115->SetColor(QColor(Qt::yellow));
	}
}

void MainWindow::on_actionLinear_Gradient_triggered()
{
	ui->stackedWidget->setCurrentIndex(0);
	ui->stackedWidget_2->setCurrentIndex(0);
	ui->stackedWidget_3->setCurrentIndex(0);
	ui->statusbar->showMessage("Linear");
	info.gradient = Linear;
	info.gradient_type = c_BrushTypePathNewLinearGradient;
}

void MainWindow::on_actionRadial_Gradient_triggered()
{
	ui->stackedWidget->setCurrentIndex(1);
	ui->stackedWidget_2->setCurrentIndex(1);
	ui->stackedWidget_3->setCurrentIndex(0);
	ui->statusbar->showMessage("Radial");
	info.gradient = Radial;
	info.gradient_type = c_BrushTypePathRadialGradient;
}

void MainWindow::on_actionTriangle_Gradient_triggered()
{
	ui->stackedWidget->setCurrentIndex(2);
	ui->stackedWidget_2->setCurrentIndex(2);
	ui->stackedWidget_3->setCurrentIndex(1);
	InitializeColors(true);
	ui->statusbar->showMessage("Triangle");
	info.gradient = Triangle;
	info.gradient_type = c_BrushTypeTriagnleMeshGradient;
}

void MainWindow::on_actionTriangle_Parametric_Gradient_triggered()
{
	ui->stackedWidget->setCurrentIndex(2);
	ui->stackedWidget_2->setCurrentIndex(3);
	ui->stackedWidget_3->setCurrentIndex(0);
	ui->statusbar->showMessage("Triangle Parametric");
	info.gradient = TriangleParametric;
	info.gradient_type = c_BrushTypeTriagnleMeshGradient;
}

void MainWindow::on_actionCoons_Patch_Gradient_triggered()
{
	ui->stackedWidget->setCurrentIndex(3);
	ui->stackedWidget_2->setCurrentIndex(4);
	ui->stackedWidget_3->setCurrentIndex(1);
	InitializeColors(false);
	ui->statusbar->showMessage("Coons Patch");
	info.gradient = CoonsPatch;
	info.gradient_type = c_BrushTypePathNewLinearGradient;
}

void MainWindow::on_actionCoons_Patch_Parametric_triggered()
{
	ui->stackedWidget->setCurrentIndex(3);
	ui->stackedWidget_2->setCurrentIndex(5);
	ui->stackedWidget_3->setCurrentIndex(0);
	ui->statusbar->showMessage("Coons Patch Parametric");
	info.gradient = CoonsPatchParametric;
	info.gradient_type = c_BrushTypeCurveGradient;
}

void MainWindow::on_actionTensor_Coons_Patch_Gradient_triggered()
{
	ui->stackedWidget->setCurrentIndex(4);
	ui->stackedWidget_2->setCurrentIndex(6);
	ui->stackedWidget_3->setCurrentIndex(1);
	InitializeColors(false);
	ui->statusbar->showMessage("Tensor Coons Patch");
	info.gradient = TensorCoonsPatch;
	info.gradient_type = c_BrushTypePathNewLinearGradient;
}

void MainWindow::on_actionTensor_Coons_Patch_Parametric_triggered()
{
	ui->stackedWidget->setCurrentIndex(4);
	ui->stackedWidget_2->setCurrentIndex(7);
	ui->stackedWidget_3->setCurrentIndex(0);
	ui->statusbar->showMessage("Tensor Coons Patch Parametric");
	info.gradient = TensorCoonsPatchParametric;
	info.gradient_type = c_BrushTypeTensorCurveGradient;
}

std::vector<agg::rgba8> MainWindow::QColor2rgba(bool triangle)
{
	std::vector<agg::rgba8> colors;
	colors.push_back({static_cast<unsigned int>(ui->label_80->GetColor().red()),
					  static_cast<unsigned int>(ui->label_80->GetColor().green()),
					  static_cast<unsigned int>(ui->label_80->GetColor().blue())});
	colors.push_back({static_cast<unsigned int>(ui->label_81->GetColor().red()),
					  static_cast<unsigned int>(ui->label_81->GetColor().green()),
					  static_cast<unsigned int>(ui->label_81->GetColor().blue())});
	colors.push_back({static_cast<unsigned int>(ui->label_114->GetColor().red()),
					  static_cast<unsigned int>(ui->label_114->GetColor().green()),
					  static_cast<unsigned int>(ui->label_114->GetColor().blue())});
	if (!triangle)
	{
		colors.push_back({static_cast<unsigned int>(ui->label_115->GetColor().red()),
						  static_cast<unsigned int>(ui->label_115->GetColor().green()),
						  static_cast<unsigned int>(ui->label_115->GetColor().blue())});
	}

	return colors;
}

std::vector<std::vector<agg::rgba8>> MainWindow::QColor2rgbaMatrix()
{
	std::vector<std::vector<agg::rgba8>> colors;
	std::vector<agg::rgba8> sub_colors;
	sub_colors.push_back({static_cast<unsigned int>(ui->label_80->GetColor().red()),
						  static_cast<unsigned int>(ui->label_80->GetColor().green()),
						  static_cast<unsigned int>(ui->label_80->GetColor().blue())});
	sub_colors.push_back({static_cast<unsigned int>(ui->label_81->GetColor().red()),
						  static_cast<unsigned int>(ui->label_81->GetColor().green()),
						  static_cast<unsigned int>(ui->label_81->GetColor().blue())});
	colors.push_back(sub_colors);
	sub_colors.clear();
	sub_colors.push_back({static_cast<unsigned int>(ui->label_114->GetColor().red()),
						  static_cast<unsigned int>(ui->label_114->GetColor().green()),
						  static_cast<unsigned int>(ui->label_114->GetColor().blue())});
	sub_colors.push_back({static_cast<unsigned int>(ui->label_115->GetColor().red()),
						  static_cast<unsigned int>(ui->label_115->GetColor().green()),
						  static_cast<unsigned int>(ui->label_115->GetColor().blue())});
	colors.push_back(sub_colors);
	sub_colors.clear();

	return colors;
}

inline void setPoint(QImage *image, int x, int y, QRgb color)
{
	image->setPixel(x, y, color);
	image->setPixel(x - 1, y, color);
	image->setPixel(x, y - 1, color);
	image->setPixel(x + 1, y, color);
	image->setPixel(x, y + 1, color);
}

void MainWindow::setPoints(QImage *image)
{
	std::vector<QPoint> points;
	if (info.gradient == Linear)
	{
		setPoint(image,
				 info.p0.x * MM_TO_COORD(ui->lable_test->width()),
				 info.p0.y * MM_TO_COORD(ui->lable_test->height()),
				 qRgb(0, 0, 0));
		setPoint(image,
				 info.p1.x * MM_TO_COORD(ui->lable_test->width()),
				 info.p1.y * MM_TO_COORD(ui->lable_test->height()),
				 qRgb(0, 0, 0));
		points.push_back(QPoint(info.p0.x * MM_TO_COORD(ui->lable_test->width()), info.p0.y * MM_TO_COORD(ui->lable_test->height())));
		points.push_back(QPoint(info.p1.x * MM_TO_COORD(ui->lable_test->width()), info.p1.y * MM_TO_COORD(ui->lable_test->height())));
	}
	else if (info.gradient == Radial)
	{
		setPoint(image,
				 info.c0.x * MM_TO_COORD(ui->lable_test->width()),
				 info.c0.y * MM_TO_COORD(ui->lable_test->height()),
				 qRgb(0, 0, 0));
		setPoint(image,
				 info.c1.x * MM_TO_COORD(ui->lable_test->width()),
				 info.c1.y * MM_TO_COORD(ui->lable_test->height()),
				 qRgb(0, 0, 0));
		points.push_back(QPoint(info.c0.x * MM_TO_COORD(ui->lable_test->width()), info.c0.y * MM_TO_COORD(ui->lable_test->height())));
		points.push_back(QPoint(info.c1.x * MM_TO_COORD(ui->lable_test->width()), info.c1.y * MM_TO_COORD(ui->lable_test->height())));
	}
	else if (info.gradient == Triangle || info.gradient == TriangleParametric)
	{
		for (NSStructures::Point p : info.triangle)
		{
			setPoint(image,
					 p.x * MM_TO_COORD(ui->lable_test->width()),
					 p.y * MM_TO_COORD(ui->lable_test->height()),
					 qRgb(0,0,0));
			points.push_back(QPoint(p.x * MM_TO_COORD(ui->lable_test->width()), p.y * MM_TO_COORD(ui->lable_test->height())));
		}
	}
	else if (info.gradient == CoonsPatch)
	{
		for (NSStructures::Point p : info.curve)
		{
			setPoint(image,
					 p.x,
					 p.y,
					 qRgb(0,0,0));
			points.push_back(QPoint(p.x, p.y));
		}
	}
	else if (info.gradient == CoonsPatchParametric)
	{
		for (NSStructures::Point p : info.curve)
		{
			setPoint(image,
					 p.x * MM_TO_COORD(ui->lable_test->width()),
					 p.y * MM_TO_COORD(ui->lable_test->height()),
					 qRgb(0,0,0));
			points.push_back(QPoint(p.x * MM_TO_COORD(ui->lable_test->width()), p.y * MM_TO_COORD(ui->lable_test->width())));
		}
	}
	else if (info.gradient == TensorCoonsPatch)
	{
		for (std::vector<NSStructures::Point> v : info.tensorcurve)
		{
			for (NSStructures::Point p : v)
			{
				setPoint(image,
						 p.x,
						 p.y,
						 qRgb(0,0,0));
				points.push_back(QPoint(p.x, p.y));
			}
		}
	}
	else if (info.gradient == TensorCoonsPatchParametric)
	{
		for (std::vector<NSStructures::Point> v : info.tensorcurve)
		{
			for (NSStructures::Point p : v)
			{
				setPoint(image,
						 p.x * MM_TO_COORD(ui->lable_test->width()),
						 p.y * MM_TO_COORD(ui->lable_test->height()),
						 qRgb(0,0,0));
				points.push_back(QPoint(p.x * MM_TO_COORD(ui->lable_test->width()), p.y * MM_TO_COORD(ui->lable_test->width())));
			}
		}
	}
	ui->lable_test->SetPoints(points);
}

void MainWindow::on_label_test_clicked()
{
	if (ui->lable_test->Movable())
	{
		ui->lable_test->ResetMovable();
		disconnect(ui->lable_test, SIGNAL(mouseMoved()), this, SLOT(on_label_test_mouse_move()));
	}
	else if (ui->lable_test->CheckPointArea())
	{
		ui->lable_test->ResetMovable();
		connect(ui->lable_test, SIGNAL(mouseMoved()), this, SLOT(on_label_test_mouse_move()));
	}
}

void MainWindow::on_label_test_mouse_move()
{
	if (info.gradient == Linear)
	{
		switch (ui->lable_test->GetIndex())
		{
		case 0:
			ui->First_X_Coordinate_Input->setText(QString::number(ui->lable_test->GetMovePoint().x() / MM_TO_COORD(ui->lable_test->width())));
			ui->First_Y_Coordinate_Input->setText(QString::number(ui->lable_test->GetMovePoint().y() / MM_TO_COORD(ui->lable_test->height())));
			break;
		case 1:
			ui->Second_X_Coordinate_Input->setText(QString::number(ui->lable_test->GetMovePoint().x() / MM_TO_COORD(ui->lable_test->width())));
			ui->Second_Y_Coordinate_Input->setText(QString::number(ui->lable_test->GetMovePoint().y() / MM_TO_COORD(ui->lable_test->height())));
			break;
		default:
			break;
		}
	}
	else if (info.gradient == Radial)
	{
		switch (ui->lable_test->GetIndex())
		{
		case 0:
			ui->First_Center_X_Coordinate_Input->setText(QString::number(ui->lable_test->GetMovePoint().x() / MM_TO_COORD(ui->lable_test->width())));
			ui->First_Center_Y_Coordinate_Input->setText(QString::number(ui->lable_test->GetMovePoint().y() / MM_TO_COORD(ui->lable_test->height())));
			break;
		case 1:
			ui->Second_Center_X_Coordinate_Input->setText(QString::number(ui->lable_test->GetMovePoint().x() / MM_TO_COORD(ui->lable_test->width())));
			ui->Second_Center_Y_Coordinate_Input->setText(QString::number(ui->lable_test->GetMovePoint().y() / MM_TO_COORD(ui->lable_test->height())));
			break;
		default:
			break;
		}
	}
	else if (info.gradient == Triangle || info.gradient == TriangleParametric)
	{
		switch (ui->lable_test->GetIndex())
		{
		case 0:
			ui->First_Vertex_X_Coordinate_Input->setText(QString::number(ui->lable_test->GetMovePoint().x() / MM_TO_COORD(ui->lable_test->width())));
			ui->First_Vertex_Y_Coordinate_Input->setText(QString::number(ui->lable_test->GetMovePoint().y() / MM_TO_COORD(ui->lable_test->height())));
			break;
		case 1:
			ui->Second_Vertex_X_Coordinate_Input->setText(QString::number(ui->lable_test->GetMovePoint().x() / MM_TO_COORD(ui->lable_test->width())));
			ui->Second_Vertex_Y_Coordinate_Input->setText(QString::number(ui->lable_test->GetMovePoint().y() / MM_TO_COORD(ui->lable_test->height())));
			break;
		case 2:
			ui->Third_Vertex_X_Coordinate_Input->setText(QString::number(ui->lable_test->GetMovePoint().x() / MM_TO_COORD(ui->lable_test->width())));
			ui->Third_Vertex_Y_Coordinate_Input->setText(QString::number(ui->lable_test->GetMovePoint().y() / MM_TO_COORD(ui->lable_test->height())));
			break;
		default:
			break;
		}
	}
	else if (info.gradient == CoonsPatch || info.gradient == CoonsPatchParametric)
	{
		switch (ui->lable_test->GetIndex())
		{
		case 0:
			ui->First_Vertex_X_Coordinate_Input_3->setText(QString::number(ui->lable_test->GetMovePoint().x() / MM_TO_COORD(ui->lable_test->width())));
			ui->First_Vertex_Y_Coordinate_Input_3->setText(QString::number(ui->lable_test->GetMovePoint().y() / MM_TO_COORD(ui->lable_test->height())));
			break;
		case 1:
			ui->First_X_Coordinate_First_Edge->setText(QString::number(ui->lable_test->GetMovePoint().x() / MM_TO_COORD(ui->lable_test->width())));
			ui->First_Y_Coordinate_First_Edge->setText(QString::number(ui->lable_test->GetMovePoint().y() / MM_TO_COORD(ui->lable_test->height())));
			break;
		case 2:
			ui->Second_X_Coordinate_First_Edge->setText(QString::number(ui->lable_test->GetMovePoint().x() / MM_TO_COORD(ui->lable_test->width())));
			ui->Second_Y_Coordinate_First_Edge->setText(QString::number(ui->lable_test->GetMovePoint().y() / MM_TO_COORD(ui->lable_test->height())));
			break;
		case 3:
			ui->Second_Vertex_X_Coordinate_Input_3->setText(QString::number(ui->lable_test->GetMovePoint().x() / MM_TO_COORD(ui->lable_test->width())));
			ui->Second_Vertex_Y_Coordinate_Input_3->setText(QString::number(ui->lable_test->GetMovePoint().y() / MM_TO_COORD(ui->lable_test->height())));
			break;
		case 4:
			ui->First_X_Coordinate_Second_Edge->setText(QString::number(ui->lable_test->GetMovePoint().x() / MM_TO_COORD(ui->lable_test->width())));
			ui->First_Y_Coordinate_Second_Edge->setText(QString::number(ui->lable_test->GetMovePoint().y() / MM_TO_COORD(ui->lable_test->height())));
			break;
		case 5:
			ui->Second_X_Coordinate_Second_Edge->setText(QString::number(ui->lable_test->GetMovePoint().x() / MM_TO_COORD(ui->lable_test->width())));
			ui->Second_Y_Coordinate_Second_Edge->setText(QString::number(ui->lable_test->GetMovePoint().y() / MM_TO_COORD(ui->lable_test->height())));
			break;
		case 6:
			ui->Third_Vertex_X_Coordinate_Input_3->setText(QString::number(ui->lable_test->GetMovePoint().x() / MM_TO_COORD(ui->lable_test->width())));
			ui->Third_Vertex_Y_Coordinate_Input_3->setText(QString::number(ui->lable_test->GetMovePoint().y() / MM_TO_COORD(ui->lable_test->height())));
			break;
		case 7:
			ui->First_X_Coordinate_Third_Edge->setText(QString::number(ui->lable_test->GetMovePoint().x() / MM_TO_COORD(ui->lable_test->width())));
			ui->First_Y_Coordinate_Third_Edge->setText(QString::number(ui->lable_test->GetMovePoint().y() / MM_TO_COORD(ui->lable_test->height())));
			break;
		case 8:
			ui->Second_X_Coordinate_Third_Edge->setText(QString::number(ui->lable_test->GetMovePoint().x() / MM_TO_COORD(ui->lable_test->width())));
			ui->Second_Y_Coordinate_Third_Edge->setText(QString::number(ui->lable_test->GetMovePoint().y() / MM_TO_COORD(ui->lable_test->height())));
			break;
		case 9:
			ui->Fourth_Vertex_X_Coordinate_Input_3->setText(QString::number(ui->lable_test->GetMovePoint().x() / MM_TO_COORD(ui->lable_test->width())));
			ui->Fourth_Vertex_Y_Coordinate_Input_3->setText(QString::number(ui->lable_test->GetMovePoint().y() / MM_TO_COORD(ui->lable_test->height())));
			break;
		case 10:
			ui->First_X_Coordinate_Fourth_Edge->setText(QString::number(ui->lable_test->GetMovePoint().x() / MM_TO_COORD(ui->lable_test->width())));
			ui->First_Y_Coordinate_Fourth_Edge->setText(QString::number(ui->lable_test->GetMovePoint().y() / MM_TO_COORD(ui->lable_test->height())));
			break;
		case 11:
			ui->Second_X_Coordinate_Fourth_Edge->setText(QString::number(ui->lable_test->GetMovePoint().x() / MM_TO_COORD(ui->lable_test->width())));
			ui->Second_Y_Coordinate_Fourth_Edge->setText(QString::number(ui->lable_test->GetMovePoint().y() / MM_TO_COORD(ui->lable_test->height())));
			break;
		default:
			break;
		}
	}
	else if (info.gradient == TensorCoonsPatch || info.gradient == TensorCoonsPatchParametric)
	{
		switch (ui->lable_test->GetIndex())
		{
		case 0:
			ui->First_X_Coordinate_First_Edge_3->setText(QString::number(ui->lable_test->GetMovePoint().x() / MM_TO_COORD(ui->lable_test->width())));
			ui->First_Y_Coordinate_First_Edge_3->setText(QString::number(ui->lable_test->GetMovePoint().y() / MM_TO_COORD(ui->lable_test->height())));
			break;
		case 1:
			ui->Second_X_Coordinate_First_Edge_3->setText(QString::number(ui->lable_test->GetMovePoint().x() / MM_TO_COORD(ui->lable_test->width())));
			ui->Second_Y_Coordinate_First_Edge_3->setText(QString::number(ui->lable_test->GetMovePoint().y() / MM_TO_COORD(ui->lable_test->height())));
			break;
		case 2:
			ui->Third_X_Coordinate_First_Edge_3->setText(QString::number(ui->lable_test->GetMovePoint().x()/ MM_TO_COORD(ui->lable_test->width())));
			ui->Third_Y_Coordinate_First_Edge_3->setText(QString::number(ui->lable_test->GetMovePoint().x()/ MM_TO_COORD(ui->lable_test->height())));
			break;
		case 3:
			ui->Fourth_X_Coordinate_First_Edge_3->setText(QString::number(ui->lable_test->GetMovePoint().x()/ MM_TO_COORD(ui->lable_test->width())));
			ui->Fourth_Y_Coordinate_First_Edge_3->setText(QString::number(ui->lable_test->GetMovePoint().x()/ MM_TO_COORD(ui->lable_test->height())));
			break;
		case 4:
			ui->First_X_Coordinate_Second_Edge_3->setText(QString::number(ui->lable_test->GetMovePoint().x() / MM_TO_COORD(ui->lable_test->width())));
			ui->First_Y_Coordinate_Second_Edge_3->setText(QString::number(ui->lable_test->GetMovePoint().y() / MM_TO_COORD(ui->lable_test->height())));
			break;
		case 5:
			ui->Second_X_Coordinate_Second_Edge_3->setText(QString::number(ui->lable_test->GetMovePoint().x() / MM_TO_COORD(ui->lable_test->width())));
			ui->Second_Y_Coordinate_Second_Edge_3->setText(QString::number(ui->lable_test->GetMovePoint().y() / MM_TO_COORD(ui->lable_test->height())));
			break;
		case 6:
			ui->Third_X_Coordinate_Second_Edge_3->setText(QString::number(ui->lable_test->GetMovePoint().x()/ MM_TO_COORD(ui->lable_test->width())));
			ui->Third_Y_Coordinate_Second_Edge_3->setText(QString::number(ui->lable_test->GetMovePoint().x()/ MM_TO_COORD(ui->lable_test->height())));
			break;
		case 7:
			ui->Fourth_X_Coordinate_Second_Edge_3->setText(QString::number(ui->lable_test->GetMovePoint().x()/ MM_TO_COORD(ui->lable_test->width())));
			ui->Fourth_Y_Coordinate_Second_Edge_3->setText(QString::number(ui->lable_test->GetMovePoint().x()/ MM_TO_COORD(ui->lable_test->height())));
			break;
		case 8:
			ui->First_X_Coordinate_Third_Edge_3->setText(QString::number(ui->lable_test->GetMovePoint().x() / MM_TO_COORD(ui->lable_test->width())));
			ui->First_Y_Coordinate_Third_Edge_3->setText(QString::number(ui->lable_test->GetMovePoint().y() / MM_TO_COORD(ui->lable_test->height())));
			break;
		case 9:
			ui->Second_X_Coordinate_Third_Edge_3->setText(QString::number(ui->lable_test->GetMovePoint().x() / MM_TO_COORD(ui->lable_test->width())));
			ui->Second_Y_Coordinate_Third_Edge_3->setText(QString::number(ui->lable_test->GetMovePoint().y() / MM_TO_COORD(ui->lable_test->height())));
			break;
		case 10:
			ui->Third_X_Coordinate_Third_Edge_3->setText(QString::number(ui->lable_test->GetMovePoint().x()/ MM_TO_COORD(ui->lable_test->width())));
			ui->Third_Y_Coordinate_Third_Edge_3->setText(QString::number(ui->lable_test->GetMovePoint().x()/ MM_TO_COORD(ui->lable_test->height())));
			break;
		case 11:
			ui->Fourth_X_Coordinate_Third_Edge_3->setText(QString::number(ui->lable_test->GetMovePoint().x()/ MM_TO_COORD(ui->lable_test->width())));
			ui->Fourth_Y_Coordinate_Third_Edge_3->setText(QString::number(ui->lable_test->GetMovePoint().x()/ MM_TO_COORD(ui->lable_test->height())));
			break;
		case 12:
			ui->First_X_Coordinate_Fourth_Edge_3->setText(QString::number(ui->lable_test->GetMovePoint().x() / MM_TO_COORD(ui->lable_test->width())));
			ui->First_Y_Coordinate_Fourth_Edge_3->setText(QString::number(ui->lable_test->GetMovePoint().y() / MM_TO_COORD(ui->lable_test->height())));
			break;
		case 13:
			ui->Second_X_Coordinate_Fourth_Edge_3->setText(QString::number(ui->lable_test->GetMovePoint().x() / MM_TO_COORD(ui->lable_test->width())));
			ui->Second_Y_Coordinate_Fourth_Edge_3->setText(QString::number(ui->lable_test->GetMovePoint().y() / MM_TO_COORD(ui->lable_test->height())));
			break;
		case 14:
			ui->Third_X_Coordinate_Fourth_Edge_3->setText(QString::number(ui->lable_test->GetMovePoint().x()/ MM_TO_COORD(ui->lable_test->width())));
			ui->Third_Y_Coordinate_Fourth_Edge_3->setText(QString::number(ui->lable_test->GetMovePoint().x()/ MM_TO_COORD(ui->lable_test->height())));
		case 15:
			ui->Fourth_X_Coordinate_Fourth_Edge_3->setText(QString::number(ui->lable_test->GetMovePoint().x()/ MM_TO_COORD(ui->lable_test->width())));
			ui->Fourth_Y_Coordinate_Fourth_Edge_3->setText(QString::number(ui->lable_test->GetMovePoint().x()/ MM_TO_COORD(ui->lable_test->height())));
		default:
			break;
		}
	}

	on_pushButton_clicked();
}

void MainWindow::on_pushButton_clicked()
{
	points = {{0, 0},
			  {static_cast<double>(ui->lable_test->width()), 0},
			  {static_cast<double>(ui->lable_test->width()), static_cast<double>(ui->lable_test->height())},
			  {0, static_cast<double>(ui->lable_test->height())}};

	if (info.gradient == Linear)
	{
		info.p0.x = ui->First_X_Coordinate_Input->text().toInt();
		info.p0.y = ui->First_Y_Coordinate_Input->text().toInt();
		info.p1.x = ui->Second_X_Coordinate_Input->text().toInt();
		info.p1.y = ui->Second_Y_Coordinate_Input->text().toInt();
		info.cont_b = ui->Continue_Shading_Backward->isChecked();
		info.cont_f = ui->Continue_Shading_Forward->isChecked();
		info.ginfo = NSStructures::GInfoConstructor::get_linear(info.p0, info.p1, 0, 1, info.cont_b, info.cont_f);
	}
	else if (info.gradient == Radial)
	{
		info.c0.x = ui->First_Center_X_Coordinate_Input->text().toInt();
		info.c0.y = ui->First_Center_Y_Coordinate_Input->text().toInt();
		info.c1.x = ui->Second_Center_X_Coordinate_Input->text().toInt();
		info.c1.y = ui->Second_Center_Y_Coordinate_Input->text().toInt();
		info.r0 = ui->First_Radius_Input->text().toInt();
		info.r1 = ui->Second_Radius_Input->text().toInt();
		info.cont_b = ui->Continue_Shading_Backward_2->isChecked();
		info.cont_f = ui->Continue_Shading_Forward_2->isChecked();
		info.ginfo = NSStructures::GInfoConstructor::get_radial(info.c0, info.c1, info.r0, info.r1, 0, 1, info.cont_b, info.cont_f);
	}
	else if (info.gradient == Triangle || info.gradient == TriangleParametric)
	{
		info.triangle[0].x = ui->First_Vertex_X_Coordinate_Input->text().toInt();
		info.triangle[0].y = ui->First_Vertex_Y_Coordinate_Input->text().toInt();
		info.triangle[1].x = ui->Second_Vertex_X_Coordinate_Input->text().toInt();
		info.triangle[1].y = ui->Second_Vertex_Y_Coordinate_Input->text().toInt();
		info.triangle[2].x = ui->Third_Vertex_X_Coordinate_Input->text().toInt();
		info.triangle[2].y = ui->Third_Vertex_Y_Coordinate_Input->text().toInt();

		if (info.gradient == TriangleParametric)
		{
			info.triangle_parametrs[0] = ui->First_Vertex_Parametr_Input_4->text().toFloat();
			info.triangle_parametrs[1] = ui->Second_Vertex_Parametr_Input_4->text().toFloat();
			info.triangle_parametrs[2] = ui->Third_Vertex_Parametr_Input_4->text().toFloat();
		}
		info.ginfo = NSStructures::GInfoConstructor::get_triangle(info.triangle,
																  QColor2rgba(true),
																  info.triangle_parametrs,
																  info.gradient == TriangleParametric);
		points = {};
		for (auto p : info.triangle)
		{
			points.push_back({p.x, p.y});
		}
	}
	else if (info.gradient == CoonsPatch || info.gradient == CoonsPatchParametric)
	{
		info.curve[0].x = ui->First_Vertex_X_Coordinate_Input_3->text().toInt();
		info.curve[0].y = ui->First_Vertex_Y_Coordinate_Input_3->text().toInt();
		info.curve[1].x = ui->First_X_Coordinate_First_Edge->text().toInt();
		info.curve[1].y = ui->First_Y_Coordinate_First_Edge->text().toInt();
		info.curve[2].x = ui->Second_X_Coordinate_First_Edge->text().toInt();
		info.curve[2].y = ui->Second_Y_Coordinate_First_Edge->text().toInt();
		info.curve[3].x = ui->Second_Vertex_X_Coordinate_Input_3->text().toInt();
		info.curve[3].y = ui->Second_Vertex_Y_Coordinate_Input_3->text().toInt();
		info.curve[4].x = ui->First_X_Coordinate_Second_Edge->text().toInt();
		info.curve[4].y = ui->First_Y_Coordinate_Second_Edge->text().toInt();
		info.curve[5].x = ui->Second_X_Coordinate_Second_Edge->text().toInt();
		info.curve[5].y = ui->Second_Y_Coordinate_Second_Edge->text().toInt();
		info.curve[6].x = ui->Third_Vertex_X_Coordinate_Input_3->text().toInt();
		info.curve[6].y = ui->Third_Vertex_Y_Coordinate_Input_3->text().toInt();
		info.curve[7].x = ui->First_X_Coordinate_Third_Edge->text().toInt();
		info.curve[7].y = ui->First_Y_Coordinate_Third_Edge->text().toInt();
		info.curve[8].x = ui->Second_X_Coordinate_Third_Edge->text().toInt();
		info.curve[8].y = ui->Second_Y_Coordinate_Third_Edge->text().toInt();
		info.curve[9].x = ui->Fourth_Vertex_X_Coordinate_Input_3->text().toInt();
		info.curve[9].y = ui->Fourth_Vertex_Y_Coordinate_Input_3->text().toInt();
		info.curve[10].x = ui->First_X_Coordinate_Fourth_Edge->text().toInt();
		info.curve[10].y = ui->First_Y_Coordinate_Fourth_Edge->text().toInt();
		info.curve[11].x = ui->Second_X_Coordinate_Fourth_Edge->text().toInt();
		info.curve[11].y = ui->Second_Y_Coordinate_Fourth_Edge->text().toInt();

		if (info.gradient == CoonsPatchParametric)
		{
			info.curve_parametrs[0] = ui->First_Vertex_Parametr_Input_2->text().toFloat();
			info.curve_parametrs[1] = ui->Second_Vertex_Parametr_Input_2->text().toFloat();
			info.curve_parametrs[2] = ui->Third_Vertex_Parametr_Input_2->text().toFloat();
			info.curve_parametrs[3] = ui->Fourth_Vertex_Parametr_Input_2->text().toFloat();
		}

		if (info.gradient == CoonsPatch)
		{
			for(int j = 0; j < 12; j++)
			{
				info.curve[j].x *= MM_TO_COORD(ui->lable_test->width());
				info.curve[j].y *= MM_TO_COORD(ui->lable_test->height());
			}
		}
		info.ginfo = NSStructures::GInfoConstructor::get_curve(
			info.curve,
			info.curve_parametrs,
			QColor2rgba(false),
			info.gradient == CoonsPatchParametric
			);
	}
	else if (info.gradient == TensorCoonsPatch || info.gradient == TensorCoonsPatchParametric)
	{
		info.tensorcurve[0][0].x = ui->First_X_Coordinate_First_Edge_3->text().toInt();
		info.tensorcurve[0][0].y = ui->First_Y_Coordinate_First_Edge_3->text().toInt();
		info.tensorcurve[0][1].x = ui->Second_X_Coordinate_First_Edge_3->text().toInt();
		info.tensorcurve[0][1].y = ui->Second_Y_Coordinate_First_Edge_3->text().toInt();
		info.tensorcurve[0][2].x = ui->Third_X_Coordinate_First_Edge_3->text().toInt();
		info.tensorcurve[0][2].y = ui->Third_Y_Coordinate_First_Edge_3->text().toInt();
		info.tensorcurve[0][3].x = ui->Fourth_X_Coordinate_First_Edge_3->text().toInt();
		info.tensorcurve[0][3].y = ui->Fourth_Y_Coordinate_First_Edge_3->text().toInt();
		info.tensorcurve[1][0].x = ui->First_X_Coordinate_Second_Edge_3->text().toInt();
		info.tensorcurve[1][0].y = ui->First_Y_Coordinate_Second_Edge_3->text().toInt();
		info.tensorcurve[1][1].x = ui->Second_X_Coordinate_Second_Edge_3->text().toInt();
		info.tensorcurve[1][1].y = ui->Second_Y_Coordinate_Second_Edge_3->text().toInt();
		info.tensorcurve[1][2].x = ui->Third_X_Coordinate_Second_Edge_3->text().toInt();
		info.tensorcurve[1][2].y = ui->Third_Y_Coordinate_Second_Edge_3->text().toInt();
		info.tensorcurve[1][3].x = ui->Fourth_X_Coordinate_Second_Edge_3->text().toInt();
		info.tensorcurve[1][3].y = ui->Fourth_Y_Coordinate_Second_Edge_3->text().toInt();
		info.tensorcurve[2][0].x = ui->First_X_Coordinate_Third_Edge_3->text().toInt();
		info.tensorcurve[2][0].y = ui->First_Y_Coordinate_Third_Edge_3->text().toInt();
		info.tensorcurve[2][1].x = ui->Second_Y_Coordinate_Third_Edge_3->text().toInt();
		info.tensorcurve[2][1].y = ui->Second_Y_Coordinate_Third_Edge_3->text().toInt();
		info.tensorcurve[2][2].x = ui->Third_X_Coordinate_Third_Edge_3->text().toInt();
		info.tensorcurve[2][2].y = ui->Third_Y_Coordinate_Third_Edge_3->text().toInt();
		info.tensorcurve[2][3].x = ui->Fourth_X_Coordinate_Third_Edge_3->text().toInt();
		info.tensorcurve[2][3].y = ui->Fourth_Y_Coordinate_Third_Edge_3->text().toInt();
		info.tensorcurve[3][0].x = ui->First_X_Coordinate_Fourth_Edge_3->text().toInt();
		info.tensorcurve[3][0].y = ui->First_Y_Coordinate_Fourth_Edge_3->text().toInt();
		info.tensorcurve[3][1].x = ui->Second_X_Coordinate_Fourth_Edge_3->text().toInt();
		info.tensorcurve[3][1].y = ui->Second_Y_Coordinate_Fourth_Edge_3->text().toInt();
		info.tensorcurve[3][2].x = ui->Third_X_Coordinate_Fourth_Edge_3->text().toInt();
		info.tensorcurve[3][2].y = ui->Third_Y_Coordinate_Fourth_Edge_3->text().toInt();
		info.tensorcurve[3][3].x = ui->Fourth_X_Coordinate_Fourth_Edge_3->text().toInt();
		info.tensorcurve[3][3].y = ui->Fourth_Y_Coordinate_Fourth_Edge_3->text().toInt();

		if (info.gradient == TensorCoonsPatchParametric)
		{
			info.tensor_curve_parametrs[0][0] = ui->First_Vertex_Parametr_Input_3->text().toFloat();
			info.tensor_curve_parametrs[0][1] = ui->Second_Vertex_Parametr_Input_3->text().toFloat();
			info.tensor_curve_parametrs[1][0] = ui->Third_Vertex_Parametr_Input_3->text().toFloat();
			info.tensor_curve_parametrs[1][1] = ui->Fourth_Vertex_Parametr_Input_3->text().toFloat();
		}

		if (info.gradient == TensorCoonsPatch)
		{
			for (int i = 0; i < 4; i++)
			{
				for (int j = 0; j < 4; j++)
				{
					info.tensorcurve[i][j].x *= MM_TO_COORD(ui->lable_test->width());
					info.tensorcurve[i][j].y *= MM_TO_COORD(ui->lable_test->height());
				}
			}
		}

		info.ginfo = NSStructures::GInfoConstructor::get_tensor_curve(
			info.tensorcurve,
			info.tensor_curve_parametrs,
			QColor2rgbaMatrix(),
			info.gradient == TensorCoonsPatchParametric
			);
	}

	if (ui->Rainbow_Colorspace_Radio_Button->isChecked())
	{
		info.c = {(LONG)0xFFff0000, (LONG)0xFFffa500, (LONG)0xFFffff00, (LONG)0xFF008000, (LONG)0xFF0000ff, (LONG)0xFFFF00FF};
		info.p = {0.0, 0.2, 0.4, 0.6, 0.8, 1};
		info.n_colors = 6;
		info.ginfo.shading.function.set_linear_interpolation({0xFFff0000, 0xFFffa500, 0xFFffff00, 0xFF008000, 0xFF0000ff, 0xFFFF00FF}, {0.0f, 0.2f, 0.4f, 0.6f, 0.8f, 1.0f});
	}
	else if (ui->BAW_Colorspace_Radio_Button->isChecked())
	{
		info.c = {(LONG)0xFFFFFFFF, (LONG)0xFF000000};
		info.p = {0.0, 1};
		info.n_colors = 2;
		info.ginfo.shading.function.set_linear_interpolation({0xFFFFFFFF, 0xFF000000}, {0.0f, 1.0f});
	}
	else if (ui->RAB_Colorspace_Radio_Button->isChecked())
	{
		info.c = {(LONG)0xFFFF0000, (LONG)0xFF0000FF};
		info.p = {0.0, 1};
		info.n_colors = 2;
		info.ginfo.shading.function.set_linear_interpolation({0xFFFF0000, 0xFF0000FF}, {0.0f, 1.0f});
	}
	else if (ui->Pastel_Colorspace_Radio_Button->isChecked())
	{
		info.c = {(LONG)0xfff39189, (LONG)0xff046582};
		info.p = {0.0, 1};
		info.n_colors = 2;
		info.ginfo.shading.function.set_linear_interpolation({0xfff39189, 0xff046582}, {0.0f, 1.0f});
	}

	QImage pm = GenerateImg(points, info, ui->lable_test->width(), ui->lable_test->height());
	setPoints(&pm);
	ui->lable_test->setPixmap(QPixmap::fromImage(pm));
	ui->lable_test->setScaledContents(true);
}
