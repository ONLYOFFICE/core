#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "../../../../DesktopEditor/graphics/pro/Graphics.h"
#include "../../../../PdfFile/PdfFile.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow), img("test.png"), lable(new QLabel)
{
	ui->setupUi(this);

	listOfLines           = ui->centralwidget->findChildren<CustomLineEdit*>();
	listOfParametricLines = ui->centralwidget->findChildren<CustomParametrLineEdit*>();
	listOfCheckBox        = ui->centralwidget->findChildren<QCheckBox*>();
	listOfColorLabels     = ui->centralwidget->findChildren<CustomColorLabel*>();

	connect(ui->lable_test, SIGNAL(mousePressed()), this, SLOT(on_label_test_clicked()));

	on_actionLinear_Gradient_triggered();
}

MainWindow::~MainWindow()
{
	delete ui;
}

void CleanupFunction(void *data)
{
	delete[] data;
}

QImage GenerateImg(std::vector<NSStructures::Point> &points, Info &info, const int& w, const int& h)
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
	info.gradient      = Linear;
	info.offset        = LinearOffset;
	info.gradient_type = c_BrushTypePathNewLinearGradient;
}

void MainWindow::on_actionRadial_Gradient_triggered()
{
	ui->stackedWidget->setCurrentIndex(1);
	ui->stackedWidget_2->setCurrentIndex(1);
	ui->stackedWidget_3->setCurrentIndex(0);
	ui->statusbar->showMessage("Radial");
	info.gradient      = Radial;
	info.offset        = RadialOffset;
	info.gradient_type = c_BrushTypePathRadialGradient;
}

void MainWindow::on_actionTriangle_Gradient_triggered()
{
	ui->stackedWidget->setCurrentIndex(2);
	ui->stackedWidget_2->setCurrentIndex(2);
	ui->stackedWidget_3->setCurrentIndex(1);
	InitializeColors(true);
	ui->statusbar->showMessage("Triangle");
	info.gradient      = Triangle;
	info.offset        = TriangleOffset;
	info.gradient_type = c_BrushTypeTriagnleMeshGradient;
}

void MainWindow::on_actionTriangle_Parametric_Gradient_triggered()
{
	ui->stackedWidget->setCurrentIndex(2);
	ui->stackedWidget_2->setCurrentIndex(3);
	ui->stackedWidget_3->setCurrentIndex(0);
	ui->statusbar->showMessage("Triangle Parametric");
	info.gradient      = TriangleParametric;
	info.offset        = TriangleOffset;
	info.gradient_type = c_BrushTypeTriagnleMeshGradient;
}

void MainWindow::on_actionCoons_Patch_Gradient_triggered()
{
	ui->stackedWidget->setCurrentIndex(3);
	ui->stackedWidget_2->setCurrentIndex(4);
	ui->stackedWidget_3->setCurrentIndex(1);
	InitializeColors(false);
	ui->statusbar->showMessage("Coons Patch");
	info.gradient      = CoonsPatch;
	info.offset        = CoonsPatchOffset;
	info.gradient_type = c_BrushTypePathNewLinearGradient;
}

void MainWindow::on_actionCoons_Patch_Parametric_triggered()
{
	ui->stackedWidget->setCurrentIndex(3);
	ui->stackedWidget_2->setCurrentIndex(5);
	ui->stackedWidget_3->setCurrentIndex(0);
	ui->statusbar->showMessage("Coons Patch Parametric");
	info.gradient      = CoonsPatchParametric;
	info.offset        = CoonsPatchOffset;
	info.gradient_type = c_BrushTypeCurveGradient;
}

void MainWindow::on_actionTensor_Coons_Patch_Gradient_triggered()
{
	ui->stackedWidget->setCurrentIndex(4);
	ui->stackedWidget_2->setCurrentIndex(6);
	ui->stackedWidget_3->setCurrentIndex(1);
	InitializeColors(false);
	ui->statusbar->showMessage("Tensor Coons Patch");
	info.gradient      = TensorCoonsPatch;
	info.offset        = TensorCoonsPatchOffset;
	info.gradient_type = c_BrushTypePathNewLinearGradient;
}

void MainWindow::on_actionTensor_Coons_Patch_Parametric_triggered()
{
	ui->stackedWidget->setCurrentIndex(4);
	ui->stackedWidget_2->setCurrentIndex(7);
	ui->stackedWidget_3->setCurrentIndex(0);
	ui->statusbar->showMessage("Tensor Coons Patch Parametric");
	info.gradient      = TensorCoonsPatchParametric;
	info.offset        = TensorCoonsPatchOffset;
	info.gradient_type = c_BrushTypeTensorCurveGradient;
}

inline agg::rgba8 getRGB(CustomColorLabel *label)
{
	return {static_cast<unsigned int>(label->GetColor().red()),
			static_cast<unsigned int>(label->GetColor().green()),
			static_cast<unsigned int>(label->GetColor().blue())};
}

std::vector<agg::rgba8> MainWindow::QColor2rgba(bool triangle)
{
	std::vector<agg::rgba8> colors;

	size_t size = listOfColorLabels.size();
	if (triangle)
		size--;

	for (int i = 0; i < size; i++)
	{
		colors.push_back(getRGB(listOfColorLabels[i]));
	}

	return colors;
}

std::vector<std::vector<agg::rgba8>> MainWindow::QColor2rgbaMatrix()
{
	std::vector<std::vector<agg::rgba8>> colors;
	size_t size = listOfColorLabels.size() / 2;

	for (int i = 0; i < size; i++)
	{
		std::vector<agg::rgba8> sub_colors;
		for (int j = 0; j < size; j++)
		{
			sub_colors.push_back(getRGB(listOfColorLabels[2 * i + j]));
		}
		colors.push_back(sub_colors);
	}

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
	switch (info.gradient)
	{
	case Linear:
		setPoint(image, info.p0.x * MM_TO_COORD(ui->lable_test->width()), info.p0.y * MM_TO_COORD(ui->lable_test->height()), qRgb(0, 0, 0));
		setPoint(image, info.p1.x * MM_TO_COORD(ui->lable_test->width()), info.p1.y * MM_TO_COORD(ui->lable_test->height()), qRgb(0, 0, 0));
		points.push_back(QPoint(info.p0.x * MM_TO_COORD(ui->lable_test->width()), info.p0.y * MM_TO_COORD(ui->lable_test->height())));
		points.push_back(QPoint(info.p1.x * MM_TO_COORD(ui->lable_test->width()), info.p1.y * MM_TO_COORD(ui->lable_test->height())));
		break;
	case Radial:
		setPoint(image, info.c0.x * MM_TO_COORD(ui->lable_test->width()), info.c0.y * MM_TO_COORD(ui->lable_test->height()), qRgb(0, 0, 0));
		setPoint(image, info.c1.x * MM_TO_COORD(ui->lable_test->width()), info.c1.y * MM_TO_COORD(ui->lable_test->height()),qRgb(0, 0, 0));
		points.push_back(QPoint(info.c0.x * MM_TO_COORD(ui->lable_test->width()), info.c0.y * MM_TO_COORD(ui->lable_test->height())));
		points.push_back(QPoint(info.c1.x * MM_TO_COORD(ui->lable_test->width()), info.c1.y * MM_TO_COORD(ui->lable_test->height())));
		break;
	case Triangle:
	case TriangleParametric:
		for (NSStructures::Point p : info.triangle)
		{
			setPoint(image, p.x * MM_TO_COORD(ui->lable_test->width()), p.y * MM_TO_COORD(ui->lable_test->height()), qRgb(0,0,0));
			points.push_back(QPoint(p.x * MM_TO_COORD(ui->lable_test->width()), p.y * MM_TO_COORD(ui->lable_test->height())));
		}
		break;
	case CoonsPatch:
		for (NSStructures::Point p : info.curve)
		{
			setPoint(image, p.x, p.y, qRgb(0,0,0));
			points.push_back(QPoint(p.x, p.y));
		}
		break;
	case CoonsPatchParametric:
		for (NSStructures::Point p : info.curve)
		{
			setPoint(image, p.x * MM_TO_COORD(ui->lable_test->width()), p.y * MM_TO_COORD(ui->lable_test->height()), qRgb(0,0,0));
			points.push_back(QPoint(p.x * MM_TO_COORD(ui->lable_test->width()), p.y * MM_TO_COORD(ui->lable_test->width())));
		}
		break;
	case TensorCoonsPatch:
		for (std::vector<NSStructures::Point> v : info.tensorcurve)
		{
			for (NSStructures::Point p : v)
			{
				setPoint(image, p.x, p.y, qRgb(0,0,0));
				points.push_back(QPoint(p.x, p.y));
			}
		}
		break;
	case TensorCoonsPatchParametric:
		for (std::vector<NSStructures::Point> v : info.tensorcurve)
		{
			for (NSStructures::Point p : v)
			{
				setPoint(image, p.x * MM_TO_COORD(ui->lable_test->width()), p.y * MM_TO_COORD(ui->lable_test->height()), qRgb(0,0,0));
				points.push_back(QPoint(p.x * MM_TO_COORD(ui->lable_test->width()), p.y * MM_TO_COORD(ui->lable_test->width())));
			}
		}
		break;
	default:
		break;
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
	listOfLines[info.offset + 2 * ui->lable_test->GetIndex() + 0]->setText(QString::number(ui->lable_test->GetMovePoint().x() / MM_TO_COORD(ui->lable_test->width())));
	listOfLines[info.offset + 2 * ui->lable_test->GetIndex() + 1]->setText(QString::number(ui->lable_test->GetMovePoint().y() / MM_TO_COORD(ui->lable_test->height())));
	on_pushButton_clicked();
}

void MainWindow::on_point_set()
{
	//Linear
	info.p0.x = listOfLines[LinearOffset + 0]->text().toInt();
	info.p0.y = listOfLines[LinearOffset + 1]->text().toInt();
	info.p1.x = listOfLines[LinearOffset + 2]->text().toInt();
	info.p1.y = listOfLines[LinearOffset + 3]->text().toInt();

	//Radial
	info.c0.x = listOfLines[RadialOffset + 0]->text().toInt();
	info.c0.y = listOfLines[RadialOffset + 1]->text().toInt();
	info.c1.x = listOfLines[RadialOffset + 2]->text().toInt();
	info.c1.y = listOfLines[RadialOffset + 3]->text().toInt();
	info.r0   = listOfLines[RadialOffset + 4]->text().toInt();
	info.r1   = listOfLines[RadialOffset + 5]->text().toInt();

	//Triangle and TriangleParametric
	for (size_t i = 0; i < info.triangle.size(); i++)
	{
		info.triangle[i].x = listOfLines[TriangleOffset + 2 * i + 0]->text().toInt();
		info.triangle[i].y = listOfLines[TriangleOffset + 2 * i + 1]->text().toInt();
	}

	//CoonsPatch and CoonsPatchParametric
	for (size_t i = 0; i < info.curve.size(); i++)
	{
		info.curve[i].x = listOfLines[CoonsPatchOffset + 2 * i + 0]->text().toInt();
		info.curve[i].y = listOfLines[CoonsPatchOffset + 2 * i + 1]->text().toInt();
	}

	//TensorCoonsPatch and TensorCoonsPatchParametric
	for (size_t i = 0; i < info.tensorcurve.size(); i++)
	{
		for (size_t j = 0; j < info.tensorcurve[i].size(); j++)
		{
			info.tensorcurve[i][j].x = listOfLines[TensorCoonsPatchOffset + 2 * (i * info.tensorcurve[i].size() + j) + 0]->text().toInt();
			info.tensorcurve[i][j].y = listOfLines[TensorCoonsPatchOffset + 2 * (i * info.tensorcurve[i].size() + j) + 1]->text().toInt();
		}
	}
}

void MainWindow::on_parametrs_set()
{
	size_t offset = 0;

	//CoonsPatchParametric
	for (size_t i = 0; i < info.curve_parametrs.size(); i++)
	{
		info.curve_parametrs[i] = listOfParametricLines[offset + i]->text().toFloat();
	}
	offset += info.curve_parametrs.size();

	//TriangleParametric
	for (size_t i = 0; i < info.triangle_parametrs.size(); i++)
	{
		info.triangle_parametrs[i] = listOfParametricLines[offset + i]->text().toFloat();
	}
	offset += info.triangle_parametrs.size();

	//TensorCoonsPatchParametric
	for (size_t i = 0; i < info.tensor_curve_parametrs.size(); i++)
	{
		offset += i;
		for (size_t j = 0; j < info.tensor_curve_parametrs[i].size(); j++)
		{
			info.tensor_curve_parametrs[i][j] = listOfParametricLines[offset + i]->text().toFloat();
		}
	}
}

void MainWindow::on_continue_set()
{
	switch (info.gradient)
	{
	case Linear:
		info.cont_f = listOfCheckBox[0]->isChecked();
		info.cont_b = listOfCheckBox[1]->isChecked();
		break;
	case Radial:
		info.cont_f = listOfCheckBox[2]->isChecked();
		info.cont_b = listOfCheckBox[3]->isChecked();
		break;
	default:
		break;
	}
}

void MainWindow::on_pushButton_clicked()
{
	points = {NSStructures::Point(),
			  NSStructures::Point(ui->lable_test->width(), 0),
			  NSStructures::Point(ui->lable_test->width(), ui->lable_test->height()),
			  NSStructures::Point(0, ui->lable_test->height())};
	on_point_set();
	on_parametrs_set();
	on_continue_set();

	switch (info.gradient)
	{
	case Linear:
		info.ginfo = NSStructures::GInfoConstructor::get_linear(info.p0, info.p1, 0, 1, info.cont_b, info.cont_f);
		break;
	case Radial:
		info.ginfo = NSStructures::GInfoConstructor::get_radial(info.c0, info.c1, info.r0, info.r1, 0, 1, info.cont_b, info.cont_f);
		break;
	case Triangle:
	case TriangleParametric:
		info.ginfo = NSStructures::GInfoConstructor::get_triangle(info.triangle, QColor2rgba(true), info.triangle_parametrs, info.gradient == TriangleParametric);
		points = {};
		for (auto p : info.triangle)
		{
			points.push_back({p.x, p.y});
		}
		break;
	case CoonsPatch:
		for(int i = 0; i < info.curve.size(); i++)
		{
			info.curve[i].x *= MM_TO_COORD(ui->lable_test->width());
			info.curve[i].y *= MM_TO_COORD(ui->lable_test->height());
		}
		info.ginfo = NSStructures::GInfoConstructor::get_curve(info.curve, info.curve_parametrs, QColor2rgba(false), false);
		break;
	case CoonsPatchParametric:
		info.ginfo = NSStructures::GInfoConstructor::get_curve(info.curve, info.curve_parametrs, QColor2rgba(false),true);
		break;
	case TensorCoonsPatch:
		for (int i = 0; i < info.tensorcurve.size(); i++)
		{
			for (int j = 0; j < info.tensorcurve[i].size(); j++)
			{
				info.tensorcurve[i][j].x *= MM_TO_COORD(ui->lable_test->width());
				info.tensorcurve[i][j].y *= MM_TO_COORD(ui->lable_test->height());
			}
		}
		info.ginfo = NSStructures::GInfoConstructor::get_tensor_curve(info.tensorcurve, info.tensor_curve_parametrs, QColor2rgbaMatrix(), false);
		break;
	case TensorCoonsPatchParametric:
		info.ginfo = NSStructures::GInfoConstructor::get_tensor_curve(info.tensorcurve, info.tensor_curve_parametrs, QColor2rgbaMatrix(), true);
		break;
	default:
		break;
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
