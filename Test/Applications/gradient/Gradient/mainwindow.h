#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QLabel>
#include <QListWidgetItem>
#include <QMainWindow>
#include <QLineEdit>
#include <QPoint>
#include <QMouseEvent>
#include <QColorDialog>
#include <QPainter>
#include "../../../../DesktopEditor/graphics/structures.h"

#define COORD_SIZE_MM 100
#define MM_TO_COORD(size) (size / COORD_SIZE_MM)

QT_BEGIN_NAMESPACE
namespace Ui
{
	class MainWindow;
}
QT_END_NAMESPACE

class CustomLineEdit : public QLineEdit
{
	Q_OBJECT
public:
	CustomLineEdit(QWidget *parent = nullptr) : QLineEdit(parent)
	{
		connect(this, &QLineEdit::editingFinished, this, &CustomLineEdit::onEditingFinished);
	}
	~CustomLineEdit() {}
public slots:
	void onEditingFinished()
	{
		if (this->text() == "")
			this->setText(this->placeholderText());
		if (this->text().toInt() < 0)
			this->setText("0");
		if (this->text().toInt() > COORD_SIZE_MM)
			this->setText(QString::number(COORD_SIZE_MM));
	}
};

class CustomParametrLineEdit : public QLineEdit
{
	Q_OBJECT
public:
	CustomParametrLineEdit(QWidget *parent = nullptr) : QLineEdit(parent)
	{
		connect(this, &QLineEdit::editingFinished, this, &CustomParametrLineEdit::onEditingFinished);
	}
	~CustomParametrLineEdit() {}
public slots:
	void onEditingFinished()
	{
		if (this->text() == "")
			this->setText(this->placeholderText());
		if (this->text().toDouble() < 0)
			this->setText("0");
		if (this->text().toDouble() > 1)
			this->setText("1");
	}
};

class CustomLabel : public QLabel
{
	Q_OBJECT
public:
	CustomLabel(QWidget *parent = nullptr) : QLabel(parent)
	{
		connect(this, &CustomLabel::mousePressed, this, &CustomLabel::onCheckPosition);
	}

	~CustomLabel() {}

	std::vector<QPoint> GetPoints() const
	{
		return m_points;
	}

	void Clear()
	{
		m_points.clear();
	}
signals:
	void mousePressed(QPoint point);
protected:
	void mousePressEvent(QMouseEvent *event) override
	{
		QPoint point = event->pos();
		emit mousePressed(point);
	}
public slots:
	void onCheckPosition(QPoint point)
	{
		m_points.push_back(point);
	}
private:
	std::vector<QPoint> m_points;
};

class CustomColorLabel : public QLabel
{
	Q_OBJECT
public:
	CustomColorLabel(QWidget *parent = nullptr) : QLabel(parent)
	{
		connect(this, &CustomColorLabel::mousePressed, this, &CustomColorLabel::onMousePressed);
	}

	~CustomColorLabel() {}

	void SetColor(QColor color)
	{
		m_color = color;
		this->setStyleSheet("QLabel { background-color : " + m_color.name() + "; border: 1px solid black; padding 10px;}");
	}

	QColor GetColor() const
	{
		return m_color;
	}
signals:
	void mousePressed();
protected:
	void mousePressEvent(QMouseEvent *event) override
	{
		emit mousePressed();
	}
public slots:
	void onMousePressed()
	{
		QColorDialog colorDialog;
		QColor color = colorDialog.getColor();

		if (color.isValid())
		{
			SetColor(color);
		}
	}
private:
	QColor m_color;
};

typedef enum
{
	Linear,
	Radial,
	Triangle,
	TriangleParametric,
	CoonsPatch,
	CoonsPatchParametric,
	TensorCoonsPatch,
	TensorCoonsPatchParametric,
	Functional
} GradientType;

struct Point
{
	Point(double _x = 0, double _y = 0) : x(_x), y(_y)
	{
	}
	double x, y;
};

struct Color
{
	uint r, g, b;
	Color()
	{
		r = b = g = 0;
	}
	Color(uint rgb)
	{
		b = rgb % 0x100;
		g = (rgb / 0x100) % 0x100;
		r = rgb / 0x100 / 0x100;
	}
	uint get_color()
	{
		return b + g * 0x100 + r * 0x10000;
	}
};

struct Info
{
	GradientType gradient = Linear;

	float r0, r1;
	NSStructures::Point c0, c1;
	NSStructures::Point p0, p1;
	bool cont_b, cont_f;
	std::vector<NSStructures::Point> triangle = {{100, 100}, {300, 200}, {200, 350}};
	std::vector<NSStructures::Point> curve = {{100, 300}, {50, 250}, {150, 150}, {100, 100},
											  {150, 50}, {250, 150}, {300, 100}, {250, 150},
											  {350, 250}, {300, 300}, {250, 350}, {150,250}};
	std::vector<std::vector<NSStructures::Point>> tensorcurve = {{{100, 300}, {150, 250}, {50, 150}, {100,100}},
																 {{150, 250}, {170, 230}, {170, 170}, {50, 150}},
																 {{350, 250}, {230, 230}, {230, 170}, {150, 250}},
																 {{300, 300}, {250, 250}, {350, 150}, {300, 100}}};
	std::vector<float> triangle_parametrs = {0.f, 0.5f, 1.f};
	std::vector<float> curve_parametrs = {0.f, 0.3f, 1.f, 0.6f};
	std::vector<std::vector<float>> tensor_curve_parametrs = {{0.f, 0.3f}, {1.f, 0.6f}};

	NSStructures::GradientInfo ginfo;
	int gradient_type;
	std::vector<LONG> c;
	std::vector<double> p;
	int n_colors;
	Info() : gradient_type(c_BrushTypePathNewLinearGradient)
	{
		c = {(LONG)0xFFff0000, (LONG)0xFFffa500, (LONG)0xFFffff00, (LONG)0xFF008000, (LONG)0xFF0000ff, (LONG)0xFFFF00FF};
		p = {0.0, 0.2, 0.4, 0.6, 0.8, 1};
		n_colors = 6;
		ginfo.shading.shading_type = NSStructures::ShadingInfo::Parametric;

		r0 = 0;
		r1 = 100;
		c0 = {200, 200};
		c1 = {200, 200};
		p0 = {0, 0};
		p1 = {400, 400};
		cont_b = cont_f = false;
	};
	~Info()
	{
	}
};

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = nullptr);
	~MainWindow();
	void InitializeColors(bool triangle);
	std::vector<agg::rgba8> QColor2rgba(bool triangle);
	std::vector<std::vector<agg::rgba8>> QColor2rgbaMatrix();
	QImage img;
	QLabel *lable;
	std::vector<Point> points;
	Info info;

private slots:

	void on_actionLinear_Gradient_triggered();

	void on_actionRadial_Gradient_triggered();

	void on_pushButton_clicked();

	void on_actionTriangle_Gradient_triggered();

	void on_actionTriangle_Parametric_Gradient_triggered();

	void on_actionCoons_Patch_Gradient_triggered();

	void on_actionCoons_Patch_Parametric_triggered();

	void on_actionTensor_Coons_Patch_Gradient_triggered();

	void on_actionTensor_Coons_Patch_Parametric_triggered();

	void on_pushButton_2_clicked();

private:
	Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
