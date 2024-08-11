#include "mainwindow.h"
#include "ui_mainwindow.h"

CustomLabel::CustomLabel(QWidget *parent) : QLabel(parent)
{
	setMouseTracking(true);
}

QPointF CustomLabel::GetStartPoint() const noexcept
{
	return StartP;
}

double CustomLabel::GetDifferenceX() const noexcept
{
	return CurrP.x() - StartP.x();
}

double CustomLabel::GetDifferenceY() const noexcept
{
	return CurrP.y() - StartP.y();
}

bool CustomLabel::GetMovable() const noexcept
{
	return Movable;
}

void CustomLabel::ResetMovable() noexcept
{
	Movable = !Movable;
}

void CustomLabel::mousePressEvent(QMouseEvent *event)
{
	StartP = event->pos();
	ResetMovable();
	emit mousePress();
}

void CustomLabel::mouseMoveEvent(QMouseEvent *event)
{
	CurrP = event->pos();
	emit mouseMove();
}

CustomButton::CustomButton(QWidget *parent) : QPushButton(parent)
{
	connect(this, &QPushButton::clicked, this, &CustomButton::SetFigure);
}

void CustomButton::SetFigure() noexcept
{
	MainWindow::Figure = this->text();
}

BooleanButton::BooleanButton(QWidget *parent) : QPushButton(parent)
{
	connect(this, &QPushButton::clicked, this, &BooleanButton::SetCommand);
}

void BooleanButton::SetCommand() noexcept
{
	if (this->text() == "Unite")
		MainWindow::Op = Aggplus::Union;
	else if (this->text() == "Intersect")
		MainWindow::Op = Aggplus::Intersection;
	else
		MainWindow::Op = Aggplus::Subtraction;
}

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
	, Path1(new Aggplus::CGraphicsPath)
	, Path2(new Aggplus::CGraphicsPath)
	, ui(new Ui::MainWindow)
{
	ui->setupUi(this);
}

MainWindow::~MainWindow()
{
	delete Path1;
	delete Path2;
	delete ui;
}

Aggplus::CGraphicsPath* MainWindow::SetPath(double offsetX, double offsetY)
{
	Aggplus::CGraphicsPath *path = new Aggplus::CGraphicsPath;

	path->StartFigure();
	if (Figure == "Rectangle")
	{
		path->MoveTo(RECTANGLE[0] + offsetX,
					 RECTANGLE[1] + offsetY);
		path->LineTo(RECTANGLE[0] + RECTANGLE[2] + offsetX,
					 RECTANGLE[1] + offsetY);
		path->LineTo(RECTANGLE[0] + RECTANGLE[2] + offsetX,
					 RECTANGLE[1] + RECTANGLE[3] + offsetY);
		path->LineTo(RECTANGLE[0] + offsetX,
					 RECTANGLE[1] + RECTANGLE[3] + offsetY);
		path->LineTo(RECTANGLE[0] + offsetX,
					 RECTANGLE[1] + offsetY);
	}
	else if (Figure == "Ellipse")
	{
		path->AddEllipse(RECTANGLE[0] + offsetX,
						 RECTANGLE[1] + offsetY,
						 RECTANGLE[2],
						 RECTANGLE[3]);
	}
	else
	{
		path->MoveTo(TRIANGLE[0] + offsetX,
					 TRIANGLE[1] + offsetY);
		path->LineTo(TRIANGLE[2] + offsetX,
					 TRIANGLE[3] + offsetY);
		path->LineTo(TRIANGLE[4] + offsetX,
					 TRIANGLE[5] + offsetY);
		path->LineTo(TRIANGLE[0] + offsetX,
					 TRIANGLE[1] + offsetY);
	}
	path->CloseFigure();

	return path;
}

void MainWindow::AddPath(NSGraphics::IGraphicsRenderer* pathRenderer, Aggplus::CGraphicsPath* path)
{
	if (path->GetPointCount() == 0)
		return;

	pathRenderer->PathCommandStart();
	pathRenderer->BeginCommand(c_nPathType);

	size_t length = path->GetPointCount();
	std::vector<Aggplus::PointD> points = path->GetPoints(0, length);
	pathRenderer->PathCommandMoveTo(points[0].X, points[0].Y);

	for (size_t i = 1; i < length; i++)
	{
		if (path->IsCurvePoint(i))
		{
			pathRenderer->PathCommandCurveTo(points[i].X, points[i].Y, points[i + 1].X,
											 points[i + 1].Y,points[i + 2].X, points[i + 2].Y);
			i += 2;
		}
		else
			pathRenderer->PathCommandLineTo(points[i].X, points[i].Y);
	}

	pathRenderer->DrawPath(c_nStroke);

	pathRenderer->EndCommand(c_nPathType);
	pathRenderer->PathCommandEnd();
}

void MainWindow::Draw(Aggplus::CGraphicsPath *path)
{
	ui->label->clear();
	QImage img(ui->label->width(), ui->label->height(), QImage::Format_RGB888);

	NSGraphics::IGraphicsRenderer* pathRenderer = NSGraphics::Create();
	NSFonts::IFontManager* fmp = NSFonts::NSFontManager::Create();
	pathRenderer->SetFontManager(fmp);

	BYTE* pData = new BYTE[4 * ui->label->width() * ui->label->height()];

	CBgraFrame oFrame;
	oFrame.put_Data(pData);
	oFrame.put_Width(ui->label->width());
	oFrame.put_Height(ui->label->height());
	oFrame.put_Stride(4 * ui->label->width());

	pathRenderer->CreateFromBgraFrame(&oFrame);
	pathRenderer->SetSwapRGB(true);

	pathRenderer->put_Width(ui->label->width());
	pathRenderer->put_Height(ui->label->height());

	if (path != nullptr)
	{
		AddPath(pathRenderer, path);
	}
	else
	{
		AddPath(pathRenderer, Path1);
		AddPath(pathRenderer, Path2);
	}

	unsigned int* pData32 = (unsigned int*)pData;
	for (size_t i = 0; i < img.size().width(); i++)
	{
		for (size_t j = 0; j < img.size().height(); j++)
		{
			img.setPixelColor(j, i, pData32[j + i * ui->label->width()]);
		}
	}

	ui->label->setPixmap(QPixmap::fromImage(img));
}

void MainWindow::SetCoords(QLabel *label, Aggplus::CGraphicsPath *path)
{
	size_t length = path->GetPointCount();
	std::vector<Aggplus::PointD> points = path->GetPoints(0, length);
	QString text = "";

	for (size_t i = 0; i < length; i++)
		text += "(" + QString::number(points[i].X) +
				"; " + QString::number(points[i].Y) + "); ";

	label->setText(text);
}

void MainWindow::DrawPath1()
{
	Path1 = SetPath(Offsets[0], Offsets[1]);
	Draw();
	SetCoords(ui->label_4, Path1);
}

void MainWindow::DrawPath2()
{
	Path2 = SetPath(Offsets[2], Offsets[3]);
	Draw();
	SetCoords(ui->label_5, Path2);
}

void MainWindow::BooleanOp()
{
	if (Path1->GetPointCount() == 0 || Path2->GetPointCount() == 0)
		return;

	Aggplus::CGraphicsPath *result = Aggplus::BooleanOperation(Path1, Path2, Op);
	Draw(result);
	SetCoords(ui->label_7, result);
	Path1->Reset();
	Path2->Reset();
}

void MainWindow::CheckMousePress()
{
	if (!ui->label->GetMovable())
	{
		Move1 = false;
		Move2 = false;
		disconnect(ui->label, SIGNAL(mouseMove()), this, SLOT(Move()));
		return;
	}

	QRectF rect1(RECTANGLE[0] + Offsets[0],
				 RECTANGLE[1] + Offsets[1],
				 RECTANGLE[2],
				 RECTANGLE[3]),
		   rect2(RECTANGLE[0] + Offsets[2],
				 RECTANGLE[1] + Offsets[3],
				 RECTANGLE[2],
				 RECTANGLE[3]);

	Move1 = rect1.contains(ui->label->GetStartPoint()) && Path1->GetPointCount() != 0;
	Move2 = rect2.contains(ui->label->GetStartPoint()) && Path2->GetPointCount() != 0;
	if (Move1 || Move2)
	{
		for (size_t i = 0; i < 4; i++)
			OldOffsets[i] = Offsets[i];
		connect(ui->label, SIGNAL(mouseMove()), this, SLOT(Move()));
	}
}

inline double CheckOffset(double offset, double min, double max)
{
	return std::max(min, std::min(max, offset));
}

void MainWindow::Move()
{
	if (Move1)
	{
		Offsets[0] = CheckOffset(OldOffsets[0] + ui->label->GetDifferenceX(),
								 static_cast<double>(ui->label->x()),
								 static_cast<double>(ui->label->width()));
		Offsets[1] = CheckOffset(OldOffsets[1] + ui->label->GetDifferenceY(),
								 static_cast<double>(ui->label->y()),
								 static_cast<double>(ui->label->height()));
		DrawPath1();
	}
	if (Move2)
	{
		Offsets[2] = CheckOffset(OldOffsets[2] + ui->label->GetDifferenceX(),
								 static_cast<double>(ui->label->x()),
								 static_cast<double>(ui->label->width()));
		Offsets[3] = CheckOffset(OldOffsets[3] + ui->label->GetDifferenceY(),
								 static_cast<double>(ui->label->y()),
								 static_cast<double>(ui->label->height()));
		DrawPath2();
	}
}
