#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QLabel>
#include <QList>
#include <QListWidgetItem>
#include <QMainWindow>
#include <QLineEdit>
#include <QCheckBox>
#include <QPoint>
#include <QPixmap>
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
		movable = false;
		setMouseTracking(true);
	}

	~CustomLabel() {}

	void SetPoints(const std::vector<QPoint>& points)
	{
		m_points = points;
	}

	QPoint GetCheckPoint() const
	{
		return checkPoint;
	}

	QPoint GetMovePoint() const
	{
		return movePoint;
	}

	size_t GetIndex() const
	{
		return index;
	}

	bool Movable() const
	{
		return movable;
	}

	void ResetMovable()
	{
		if (movable)
		{
			movable = false;
		}
		else
		{
			movable = true;
		}
	}

	void setIndex(size_t _index)
	{
		index = _index;
	}

	void Clear()
	{
		m_points.clear();
	}

	bool CheckPointArea()
	{
		for (int i = 0; i < m_points.size(); i++)
		{
			QRect rect(m_points[i].x() - 5, m_points[i].y() - 5, m_points[i].x() + 5, m_points[i].y() + 5);
			if (rect.contains(checkPoint))
			{
				index = i;
				return true;
			}
		}
		return false;
	}

signals:
	void mousePressed();
	void mouseMoved();

protected:
	void mousePressEvent(QMouseEvent *event) override
	{
		checkPoint = event->pos();
		emit mousePressed();
	}

	void mouseMoveEvent(QMouseEvent *event) override
	{
		movePoint = event->pos();
		emit mouseMoved();
	}

private:
	bool movable;
	size_t index;
	QPoint movePoint;
	QPoint checkPoint;
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
	TensorCoonsPatchParametric
} GradientType;

typedef enum
{
	LinearOffset = 68,
	RadialOffset = 62,
	TriangleOffset = 56,
	CoonsPatchOffset = 32,
	TensorCoonsPatchOffset = 0
} GradientOffse;

struct Info
{
	GradientType gradient;
	GradientOffse offset;

	float r0, r1;
	NSStructures::Point c0, c1;
	NSStructures::Point p0, p1;
	bool cont_b, cont_f;
	std::vector<NSStructures::Point> triangle;
	std::vector<NSStructures::Point> curve;
	std::vector<std::vector<NSStructures::Point>> tensorcurve;
	std::vector<float> triangle_parametrs;
	std::vector<float> curve_parametrs;
	std::vector<std::vector<float>> tensor_curve_parametrs;

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
		triangle.resize(3);
		curve.resize(12);
		tensorcurve.resize(4);
		for (int i = 0; i < tensorcurve.size(); i++)
		{
			tensorcurve[i].resize(4);
		}
		triangle_parametrs.resize(3);
		curve_parametrs.resize(4);
		tensor_curve_parametrs.resize(2);
		for (int i = 0; i < tensor_curve_parametrs.size(); i++)
		{
			tensor_curve_parametrs[i].resize(2);
		}

		cont_b = cont_f = false;
	}
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
	void setPoints(QImage *image);
	QImage img;
	QLabel *lable;
	std::vector<NSStructures::Point> points;
	Info info;

private slots:

	void on_label_test_clicked();

	void on_label_test_mouse_move();

	void on_point_set();

	void on_parametrs_set();

	void on_continue_set();

	void on_actionLinear_Gradient_triggered();

	void on_actionRadial_Gradient_triggered();

	void on_pushButton_clicked();

	void on_actionTriangle_Gradient_triggered();

	void on_actionTriangle_Parametric_Gradient_triggered();

	void on_actionCoons_Patch_Gradient_triggered();

	void on_actionCoons_Patch_Parametric_triggered();

	void on_actionTensor_Coons_Patch_Gradient_triggered();

	void on_actionTensor_Coons_Patch_Parametric_triggered();

private:
	QList<CustomLineEdit *> listOfLines;
	QList<CustomParametrLineEdit *> listOfParametricLines;
	QList<QCheckBox *> listOfCheckBox;
	QList<CustomColorLabel *>listOfColorLabels;
	Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
