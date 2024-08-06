#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "../../../../DesktopEditor/graphics/pro/Graphics.h"
#include "../../../../PdfFile/PdfFile.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow), img("test.png")
{
	ui->setupUi(this);

	listOfLines				= ui->centralwidget->findChildren<CustomLineEdit*>();
	listOfCheckBox			= ui->centralwidget->findChildren<QCheckBox*>();
	listOfColorLabels		= ui->centralwidget->findChildren<CustomColorLabel*>();
	listOfParametricLines	= ui->centralwidget->findChildren<CustomParametrLineEdit*>();

	connect(ui->label_test, SIGNAL(mousePressed()), this, SLOT(on_label_test_clicked()));

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
	auto a		= info.c;
	auto b		= info.p;
	LONG *c		= a.data();
	double *p	= b.data();
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

void MainWindow::initializeColors(bool Triangle)
{
	listOfColorLabels[0]->setColor(QColor(Qt::red));
	listOfColorLabels[1]->setColor(QColor(Qt::green));
	listOfColorLabels[2]->setColor(QColor(Qt::blue));
	if (!Triangle)
	{
		listOfColorLabels[3]->setColor(QColor(Qt::yellow));
	}
}

void MainWindow::on_actionLinear_Gradient_triggered()
{
	ui->stackedWidget_1->setCurrentIndex(0);
	ui->stackedWidget_2->setCurrentIndex(0);
	ui->stackedWidget_3->setCurrentIndex(0);
	info.gradient		= Linear;
	info.offset			= LinearOffset;
	info.gradient_type	= c_BrushTypePathNewLinearGradient;
	ui->statusbar->showMessage("Linear");
}

void MainWindow::on_actionRadial_Gradient_triggered()
{
	ui->stackedWidget_1->setCurrentIndex(1);
	ui->stackedWidget_2->setCurrentIndex(1);
	ui->stackedWidget_3->setCurrentIndex(0);
	info.gradient		= Radial;
	info.offset			= RadialOffset;
	info.gradient_type	= c_BrushTypePathRadialGradient;
	ui->statusbar->showMessage("Radial");
}

void MainWindow::on_actionTriangle_Gradient_triggered()
{
	ui->stackedWidget_1->setCurrentIndex(2);
	ui->stackedWidget_2->setCurrentIndex(2);
	ui->stackedWidget_3->setCurrentIndex(1);
	info.gradient		= Triangle;
	info.offset			= TriangleOffset;
	info.gradient_type	= c_BrushTypeTriagnleMeshGradient;
	initializeColors(true);
	ui->statusbar->showMessage("Triangle");
}

void MainWindow::on_actionTriangle_Parametric_Gradient_triggered()
{
	ui->stackedWidget_1->setCurrentIndex(2);
	ui->stackedWidget_2->setCurrentIndex(3);
	ui->stackedWidget_3->setCurrentIndex(0);
	info.gradient		= TriangleParametric;
	info.offset			= TriangleOffset;
	info.gradient_type	= c_BrushTypeTriagnleMeshGradient;
	ui->statusbar->showMessage("Triangle Parametric");
}

void MainWindow::on_actionCoons_Patch_Gradient_triggered()
{
	ui->stackedWidget_1->setCurrentIndex(3);
	ui->stackedWidget_2->setCurrentIndex(4);
	ui->stackedWidget_3->setCurrentIndex(1);
	info.gradient		= CoonsPatch;
	info.offset			= CoonsPatchOffset;
	info.gradient_type	= c_BrushTypeCurveGradient;
	initializeColors(false);
	ui->statusbar->showMessage("Coons Patch");
}

void MainWindow::on_actionCoons_Patch_Parametric_triggered()
{
	ui->stackedWidget_1->setCurrentIndex(3);
	ui->stackedWidget_2->setCurrentIndex(5);
	ui->stackedWidget_3->setCurrentIndex(0);
	info.gradient		= CoonsPatchParametric;
	info.offset			= CoonsPatchOffset;
	info.gradient_type	= c_BrushTypeCurveGradient;
	ui->statusbar->showMessage("Coons Patch Parametric");
}

void MainWindow::on_actionTensor_Coons_Patch_Gradient_triggered()
{
	ui->stackedWidget_1->setCurrentIndex(4);
	ui->stackedWidget_2->setCurrentIndex(6);
	ui->stackedWidget_3->setCurrentIndex(1);
	info.gradient		= TensorCoonsPatch;
	info.offset			= TensorCoonsPatchOffset;
	info.gradient_type	= c_BrushTypeTensorCurveGradient;
	initializeColors(false);
	ui->statusbar->showMessage("Tensor Coons Patch");
}

void MainWindow::on_actionTensor_Coons_Patch_Parametric_triggered()
{
	ui->stackedWidget_1->setCurrentIndex(4);
	ui->stackedWidget_2->setCurrentIndex(7);
	ui->stackedWidget_3->setCurrentIndex(0);
	info.gradient		= TensorCoonsPatchParametric;
	info.offset			= TensorCoonsPatchOffset;
	info.gradient_type	= c_BrushTypeTensorCurveGradient;
	ui->statusbar->showMessage("Tensor Coons Patch Parametric");
}

inline agg::rgba8 getRGB(CustomColorLabel *label)
{
	return {static_cast<unsigned int>(label->getColor().red()),
			static_cast<unsigned int>(label->getColor().green()),
			static_cast<unsigned int>(label->getColor().blue())};
}

std::vector<agg::rgba8> MainWindow::qColor2rgba(bool triangle)
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

std::vector<std::vector<agg::rgba8>> MainWindow::qColor2rgbaMatrix()
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

NSStructures::Point MainWindow::scaleCoord(NSStructures::Point p)
{
	return {p.x * MM_TO_COORD(ui->label_test->width()), p.y * MM_TO_COORD(ui->label_test->height())};
}

inline void setPoint(QImage *image, NSStructures::Point p)
{
	image->setPixel(p.x, p.y, qRgb(0, 0, 0));
	image->setPixel(p.x - 1, p.y, qRgb(0, 0, 0));
	image->setPixel(p.x, p.y - 1, qRgb(0, 0, 0));
	image->setPixel(p.x + 1, p.y, qRgb(0, 0, 0));
	image->setPixel(p.x, p.y + 1, qRgb(0, 0, 0));
}

void MainWindow::setPoints(QImage *image)
{
	std::vector<NSStructures::Point> points;
	switch (info.gradient)
	{
	case Linear:
		setPoint(image, scaleCoord(info.p0));
		setPoint(image, scaleCoord(info.p1));
		points.push_back(scaleCoord(info.p0));
		points.push_back(scaleCoord(info.p1));
		break;
	case Radial:
		setPoint(image, scaleCoord(info.c0));
		setPoint(image, scaleCoord(info.c1));
		points.push_back(scaleCoord(info.c0));
		points.push_back(scaleCoord(info.c1));
		break;
	case Triangle:
	case TriangleParametric:
		for (NSStructures::Point p : info.triangle)
		{
			setPoint(image, scaleCoord(p));
			points.push_back(scaleCoord(p));
		}
		break;
	case CoonsPatch:
	case CoonsPatchParametric:
		for (int i = 0; i < info.curve.size(); i++)
		{
			setPoint(image, scaleCoord(info.curve[i]));
			points.push_back(scaleCoord(info.curve[i]));
		}
		break;
	case TensorCoonsPatch:
	case TensorCoonsPatchParametric:
		for (std::vector<NSStructures::Point> v : info.tensorcurve)
		{
			for (NSStructures::Point p : v)
			{
				setPoint(image, scaleCoord(p));
				points.push_back(scaleCoord(p));
			}
		}
		break;
	default:
		break;
	}
	ui->label_test->setPoints(points);
}

void MainWindow::on_label_test_clicked()
{
	if (ui->label_test->getMovable())
	{
		ui->label_test->resetMovable();
		disconnect(ui->label_test, SIGNAL(mouseMoved()), this, SLOT(on_label_test_mouse_move()));
	}
	else if (ui->label_test->checkPointArea())
	{
		ui->label_test->resetMovable();
		connect(ui->label_test, SIGNAL(mouseMoved()), this, SLOT(on_label_test_mouse_move()));
	}
}

void MainWindow::on_label_test_mouse_move()
{
	listOfLines[info.offset + 2 * ui->label_test->getIndex() + 0]->setText(QString::number(static_cast<int>(ui->label_test->getMovePoint().x) / MM_TO_COORD(ui->label_test->width())));
	listOfLines[info.offset + 2 * ui->label_test->getIndex() + 1]->setText(QString::number(static_cast<int>(ui->label_test->getMovePoint().y) / MM_TO_COORD(ui->label_test->height())));
	on_pushButton_clicked();
}

void MainWindow::lineEdits2Points()
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

void MainWindow::lineEdits2Parametrs()
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

void MainWindow::checkBox2Continue()
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
	lineEdits2Points();
	lineEdits2Parametrs();
	checkBox2Continue();
	points = {{0, 0},
			  {ui->label_test->width(), 0},
			  {ui->label_test->width(), ui->label_test->height()},
			  {0, ui->label_test->height()}};

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
		info.ginfo = NSStructures::GInfoConstructor::get_triangle(info.triangle, qColor2rgba(true),
																  info.triangle_parametrs, info.gradient == TriangleParametric);
		points = {};
		for (auto p : info.triangle)
		{
			points.push_back({p.x, p.y});
		}
		break;
	case CoonsPatch:
	case CoonsPatchParametric:
		info.ginfo = NSStructures::GInfoConstructor::get_curve(info.curve, info.curve_parametrs,
															   qColor2rgba(false), info.gradient == CoonsPatchParametric);
		break;
	case TensorCoonsPatch:
	case TensorCoonsPatchParametric:
		info.ginfo = NSStructures::GInfoConstructor::get_tensor_curve(info.tensorcurve, info.tensor_curve_parametrs,
																	  qColor2rgbaMatrix(), info.gradient == TensorCoonsPatchParametric);
		break;
	default:
		break;
	}

	if (ui->Rainbow_Colorspace_Radio_Button->isChecked())
	{
		info.c = {(LONG)0xFFff0000, (LONG)0xFFffa500, (LONG)0xFFffff00, (LONG)0xFF008000, (LONG)0xFF0000ff, (LONG)0xFFFF00FF};
		info.p = {0.0, 0.2, 0.4, 0.6, 0.8, 1};
		info.n_colors = 6;
		info.ginfo.shading.function.set_linear_interpolation({0xFFff0000, 0xFFffa500, 0xFFffff00, 0xFF008000, 0xFF0000ff, 0xFFFF00FF},
															 {0.0f, 0.2f, 0.4f, 0.6f, 0.8f, 1.0f});
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

	QImage pm = GenerateImg(points, info, ui->label_test->width(), ui->label_test->height());
	setPoints(&pm);
	ui->label_test->setPixmap(QPixmap::fromImage(pm));
	ui->label_test->setScaledContents(true);
}
