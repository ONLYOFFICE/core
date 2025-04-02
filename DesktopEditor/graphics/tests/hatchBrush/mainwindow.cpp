#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QImage>
#include <QPixmap>
#include <QSpinBox>
#include <QComboBox>
#include <QStringList>

#include "../../pro/Graphics.h"

static constexpr double rectangle[4] = {
	10.0, 10.0, 80.0, 80.0,
};

static const QStringList patterns = {
	"cross",
	"dashDnDiag",
	"dashHorz",
	"dashUpDiag",
	"dashVert",
	"diagBrick",
	"diagCross",
	"divot",
	"dkDnDiag",
	"dkHorz",
	"dkUpDiag",
	"dkVert",
	"dnDiag",
	"dotDmnd",
	"dotGrid",
	"horz",
	"horzBrick",
	"lgCheck",
	"lgConfetti",
	"lgGrid",
	"ltDnDiag",
	"ltHorz",
	"ltUpDiag",
	"ltVert",
	"narHorz",
	"narVert",
	"openDmnd",
	"pct10",
	"pct20",
	"pct25",
	"pct30",
	"pct40",
	"pct5",
	"pct50",
	"pct60",
	"pct70",
	"pct75",
	"pct80",
	"pct90",
	"plaid",
	"shingle",
	"smCheck",
	"smConfetti",
	"smGrid",
	"solidDmnd",
	"sphere",
	"trellis",
	"upDiag",
	"vert",
	"wave",
	"wdDnDiag",
	"wdUpDiag",
	"weave",
	"zigZag"
};

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
	, ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	ui->pushButton->SetColor(Qt::red);
	ui->pushButton_2->SetColor(Qt::blue);

	ui->comboBox->addItems(patterns);

	connect(ui->pushButton,		&CColorButton::ColorChanged,	this, &MainWindow::Draw);
	connect(ui->pushButton_2,	&CColorButton::ColorChanged,	this, &MainWindow::Draw);
	connect(ui->spinBox,		&QSpinBox::valueChanged,		this, &MainWindow::Draw);
	connect(ui->spinBox_2,		&QSpinBox::valueChanged,		this, &MainWindow::Draw);
	connect(ui->comboBox,		&QComboBox::currentTextChanged, this, &MainWindow::Draw);
	connect(ui->radioButton,	&QRadioButton::clicked,			this, &MainWindow::Draw);
	connect(ui->radioButton_2,	&QRadioButton::clicked,			this, &MainWindow::Draw);

	m_oPathRectangle.StartFigure();
	m_oPathRectangle.MoveTo(rectangle[0], rectangle[1]);
	m_oPathRectangle.LineTo(rectangle[0] + rectangle[2], rectangle[1]);
	m_oPathRectangle.LineTo(rectangle[0] + rectangle[2], rectangle[1] + rectangle[3]);
	m_oPathRectangle.LineTo(rectangle[0], rectangle[1] + rectangle[3]);
	m_oPathRectangle.LineTo(rectangle[0], rectangle[1]);
	m_oPathRectangle.CloseFigure();

	m_oPathEllisps.StartFigure();
	m_oPathEllisps.AddEllipse(rectangle[0], rectangle[1], rectangle[2], rectangle[3]);
	m_oPathEllisps.CloseFigure();

	Draw();
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::Draw()
{
	NSGraphics::IGraphicsRenderer *renderer = NSGraphics::Create();

	int width = ui->label->width(),
		height = ui->label->height();

	BYTE* data = new  BYTE[4 * width * height];

	CBgraFrame frame;
	frame.put_Data(data);
	frame.put_Width(width);
	frame.put_Height(height);
	frame.put_Stride(4 * width);

	renderer->put_Width(100);
	renderer->put_Height(100);

	renderer->CreateFromBgraFrame(&frame);
	renderer->SetSwapRGB(false);

	renderer->put_BrushType(c_BrushTypeHatch1);
	renderer->put_BrushTexturePath(ui->comboBox->currentText().toStdWString());
	renderer->put_BrushColor1(ui->pushButton->GetColor().rgb());
	renderer->put_BrushAlpha1(ui->spinBox->value());
	renderer->put_BrushColor2(ui->pushButton_2->GetColor().rgb());
	renderer->put_BrushAlpha2(ui->spinBox->value());

	std::vector<Aggplus::PointD> points;
	unsigned length;
	bool is_rectangle = ui->radioButton->isChecked();
	if (is_rectangle)
	{
		length = m_oPathRectangle.GetPointCount();
		points = m_oPathRectangle.GetPoints(0, length);
	}
	else
	{
		length = m_oPathEllisps.GetPointCount();
		points = m_oPathEllisps.GetPoints(0, length);
	}

	renderer->BeginCommand(c_nPathType);
	renderer->PathCommandStart();

	for (unsigned i = 0; i < length; i++)
	{
		if (is_rectangle ? m_oPathRectangle.IsCurvePoint(i) : m_oPathEllisps.IsCurvePoint(i))
		{
			renderer->PathCommandCurveTo(points[i].X, points[i].Y,
										 points[i + 1].X, points[i + 1].Y,
										 points[i + 2].X, points[i + 2].Y);
			i += 2;
		}
		else if (is_rectangle ? m_oPathRectangle.IsMovePoint(i) : m_oPathEllisps.IsMovePoint(i))
			renderer->PathCommandMoveTo(points[i].X, points[i].Y);
		else if (is_rectangle ? m_oPathRectangle.IsLinePoint(i) : m_oPathEllisps.IsLinePoint(i))
			renderer->PathCommandLineTo(points[i].X, points[i].Y);
	}

	renderer->PathCommandClose();
	renderer->Fill();
	renderer->EndCommand(c_nPathType);
	renderer->PathCommandEnd();

	auto img = QImage(data, width, height, QImage::Format_RGBA8888, [](void *data){
		delete[] (BYTE*)data;
	});
	ui->label->setPixmap(QPixmap::fromImage(img));

	RELEASEOBJECT(renderer);
}
