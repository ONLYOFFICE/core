#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <vector>

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

void CustomLabel::mousePressEvent(QMouseEvent *event)
{
	StartP = event->pos();
	Movable = true;
	emit mousePress();
}

void CustomLabel::mouseReleaseEvent(QMouseEvent *event)
{
	CurrP = event->pos();
	Movable = false;
	emit mousePress();
}

void CustomLabel::mouseMoveEvent(QMouseEvent *event)
{
	CurrP = event->pos();
	emit mouseMove();
}

CustomButton::CustomButton(QWidget *parent) : QPushButton(parent)
{
}

BooleanButton::BooleanButton(QWidget *parent) : QPushButton(parent)
{
}

std::vector<QObject*> GetChildsByClassName(QObject* parent, const QString& name)
{
	std::vector<QObject*> res;
	foreach (QObject* child, parent->children())
	{
		if (QString(child->metaObject()->className()) == name)
			res.push_back(child);
		else
		{
			if (!child->children().empty())
			{
				std::vector<QObject*> resChilds = GetChildsByClassName(child, name);
				res.insert(res.end(), resChilds.begin(), resChilds.end());
			}
		}
	}
	return res;
}

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
	, Path1(new Aggplus::CGraphicsPath)
	, Path2(new Aggplus::CGraphicsPath)
	, ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	Figure1 = "";
	Figure2 = "";
	Op = Aggplus::Intersection;

	std::vector<QObject*> arrPathButtons = GetChildsByClassName(this, "CustomButton");
	std::vector<QObject*> arrBooleanButtons = GetChildsByClassName(this, "BooleanButton");

	for (std::vector<QObject*>::iterator i = arrPathButtons.begin(); i != arrPathButtons.end(); i++)
		connect((QPushButton*)(*i), &QPushButton::clicked, this, &MainWindow::SetFigure);

	for (std::vector<QObject*>::iterator i = arrBooleanButtons.begin(); i != arrBooleanButtons.end(); i++)
		connect((QPushButton*)(*i), &QPushButton::clicked, this, &MainWindow::SetCommand);
}

void MainWindow::SetFigure()
{
	QPushButton* sender = (QPushButton*)this->sender();

	if (((QGroupBox*)sender->parentWidget())->title() == "Path1")
	{
		Figure1 = sender->text();
		DrawPath1();
	}

	if (((QGroupBox*)sender->parentWidget())->title() == "Path2")
	{
		Figure2 = sender->text();
		DrawPath2();
	}
}

void MainWindow::SetCommand()
{
	QString text = ((QPushButton*)sender())->text();
	if (text == "Unite")
		Op = Aggplus::Union;
	else if (text == "Intersect")
		Op = Aggplus::Intersection;
	else
		Op = Aggplus::Subtraction;

	BooleanOp();
}

MainWindow::~MainWindow()
{
	delete Path1;
	delete Path2;
	delete ui;
}

Aggplus::CGraphicsPath* MainWindow::SetPath(double offsetX, double offsetY, QString Figure)
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

void MainWindow::AddPath(NSGraphics::IGraphicsRenderer* pathRenderer, Aggplus::CGraphicsPath* path, bool isResult)
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

	if (isResult)
	{
		pathRenderer->put_BrushColor1(0xFF0000);
		pathRenderer->Fill();
	}

	pathRenderer->put_PenColor(!isResult ? 0x000000 : 0x0000FF);
	pathRenderer->DrawPath(c_nStroke);

	pathRenderer->EndCommand(c_nPathType);
	pathRenderer->PathCommandEnd();
}

void MainWindow::Draw(Aggplus::CGraphicsPath *path)
{
	ui->label->clear();

	NSGraphics::IGraphicsRenderer* pathRenderer = NSGraphics::Create();
	NSFonts::IFontManager* fmp = NSFonts::NSFontManager::Create();
	pathRenderer->SetFontManager(fmp);

	int nW = ui->label->width();
	int nH = ui->label->height();

	BYTE* pData = new BYTE[4 * nW * nH];

	CBgraFrame oFrame;
	oFrame.put_Data(pData);
	oFrame.put_Width(nW);
	oFrame.put_Height(nH);
	oFrame.put_Stride(4 * nW);

	pathRenderer->CreateFromBgraFrame(&oFrame);
	pathRenderer->SetSwapRGB(true);

	pathRenderer->put_Width(nW);
	pathRenderer->put_Height(nH);

	AddPath(pathRenderer, Path1);
	AddPath(pathRenderer, Path2);

	if (path != nullptr)
	{
		AddPath(pathRenderer, path, true);
	}

	QImage img = QImage(pData, nW, nH, QImage::Format_RGBA8888, [](void *data){
		delete [] (BYTE*)data;
	});
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
	if (Path1) delete Path1;
	Path1 = SetPath(Offsets[0], Offsets[1], Figure1);
	Draw();
	SetCoords(ui->label_4, Path1);
}

void MainWindow::DrawPath2()
{
	if (Path2) delete Path2;
	Path2 = SetPath(Offsets[2], Offsets[3], Figure2);
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
	if (Move2)
		Move1 = false;
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
