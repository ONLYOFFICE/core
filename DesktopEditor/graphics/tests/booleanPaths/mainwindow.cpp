#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "../../GraphicsPath.h"

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
	, ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	Path1 = new Aggplus::CGraphicsPath;
	Path1->StartFigure();
	Path2 = new Aggplus::CGraphicsPath;
	Path2->StartFigure();
}

MainWindow::~MainWindow()
{
	delete ui;
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

void MainWindow::AddCommand(Aggplus::CGraphicsPath *path, agg::path_commands_e cmd,
							float x0, float y0, float x1, float y1, float x2, float y2)
{
	switch (cmd)
	{
	case agg::path_cmd_move_to:
		path->MoveTo(x0, y0);
		break;
	case agg::path_cmd_line_to:
		if (path->GetPointCount() == 0)
			return;
		path->LineTo(x0, y0);
		break;
	case agg::path_cmd_curve4:
		if (path->GetPointCount() == 0)
			return;
		path->CurveTo(x1, y1, x2, y2, x0, y0);
		break;
	default:
		break;
	}
	Draw();
}

void MainWindow::PrepareClip()
{
	if (Path1->GetPointCount() == 0 || Path2->GetPointCount() == 0)
		return;

	Path1->CloseFigure();
	Path2->CloseFigure();
}

void MainWindow::Reset(Aggplus::CGraphicsPath *path)
{
	path->Reset();
	path->StartFigure();
	Draw();
}

void MainWindow::on_pushButton_clicked()
{
	AddCommand(Path1, agg::path_cmd_move_to, ui->lineEdit->text().toFloat(), ui->lineEdit_2->text().toFloat());
}

void MainWindow::on_pushButton_2_clicked()
{
	AddCommand(Path1, agg::path_cmd_line_to, ui->lineEdit_3->text().toFloat(), ui->lineEdit_4->text().toFloat());
}


void MainWindow::on_pushButton_3_clicked()
{
	AddCommand(Path1, agg::path_cmd_curve4, ui->lineEdit_5->text().toFloat(), ui->lineEdit_6->text().toFloat(),
			   ui->lineEdit_7->text().toFloat(), ui->lineEdit_8->text().toFloat(),ui->lineEdit_9->text().toFloat(),
			   ui->lineEdit_10->text().toFloat());
}


void MainWindow::on_pushButton_4_clicked()
{
	AddCommand(Path2, agg::path_cmd_move_to, ui->lineEdit_11->text().toFloat(), ui->lineEdit_12->text().toFloat());
}


void MainWindow::on_pushButton_5_clicked()
{
	AddCommand(Path2, agg::path_cmd_line_to, ui->lineEdit_13->text().toFloat(), ui->lineEdit_14->text().toFloat());
}


void MainWindow::on_pushButton_6_clicked()
{
	AddCommand(Path2, agg::path_cmd_curve4, ui->lineEdit_15->text().toFloat(), ui->lineEdit_16->text().toFloat(),
			   ui->lineEdit_17->text().toFloat(), ui->lineEdit_18->text().toFloat(),ui->lineEdit_19->text().toFloat(),
			   ui->lineEdit_20->text().toFloat());
}


void MainWindow::on_pushButton_10_clicked()
{
	PrepareClip();
	Aggplus::CGraphicsPath* result = Aggplus::BooleanOperation(Path1, Path2, Aggplus::Union);
	Draw(result);
}


void MainWindow::on_pushButton_7_clicked()
{
	PrepareClip();
	Aggplus::CGraphicsPath* result = Aggplus::BooleanOperation(Path1, Path2, Aggplus::Intersection);
	Draw(result);
}


void MainWindow::on_pushButton_8_clicked()
{
	PrepareClip();
	Aggplus::CGraphicsPath* result = Aggplus::BooleanOperation(Path1, Path2, Aggplus::Subtraction);
	Draw(result);
}


void MainWindow::on_pushButton_11_clicked()
{
	PrepareClip();
	Aggplus::CGraphicsPath* result = Aggplus::BooleanOperation(Path1, Path2, Aggplus::Division);
	Draw(result);
}


void MainWindow::on_pushButton_14_clicked()
{
	PrepareClip();
	Aggplus::CGraphicsPath* result = Aggplus::BooleanOperation(Path1, Path2, Aggplus::Exclusion);
	Draw(result);
}


void MainWindow::on_pushButton_9_clicked()
{
	Reset(Path1);
}


void MainWindow::on_pushButton_12_clicked()
{
	Reset(Path2);
}

