#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QLabel>
#include <QList>
#include <QMainWindow>
#include <QLineEdit>
#include <QCheckBox>
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

class CustomAlphaLineEdit : public QLineEdit
{
	Q_OBJECT
public:
	CustomAlphaLineEdit(QWidget *parent = nullptr) : QLineEdit(parent)
	{
		connect(this, &QLineEdit::editingFinished, this, &CustomAlphaLineEdit::onEditingFinished);
	}
	~CustomAlphaLineEdit() {}
public slots:
	void onEditingFinished()
	{
		if (this->text() == "")
			this->setText(this->placeholderText());
		if (this->text().toUInt() > 255)
			this->setText("255");
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

	void setPoints(const std::vector<NSStructures::Point>& points)
	{
		m_points = points;
	}

	NSStructures::Point getMovePoint() const
	{
		return movePoint;
	}

	size_t getIndex() const
	{
		return index;
	}

	bool getMovable() const
	{
		return movable;
	}

	void resetMovable()
	{
		movable = !movable;
	}

	void setIndex(size_t _index)
	{
		index = _index;
	}

	bool checkPointArea()
	{
		for (int i = 0; i < m_points.size(); i++)
		{
			QRect rect(m_points[i].x - 5, m_points[i].y - 5, 10, 10);
			if (rect.contains(checkPoint.x, checkPoint.y))
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
		checkPoint = {event->pos().x(), event->pos().y()};
		emit mousePressed();
	}

	void mouseMoveEvent(QMouseEvent *event) override
	{
		movePoint = {event->pos().x(), event->pos().y()};
		emit mouseMoved();
	}

private:
	bool	movable;
	size_t  index;

	NSStructures::Point				 movePoint;
	NSStructures::Point 			 checkPoint;
	std::vector<NSStructures::Point> m_points;
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

	void setColor(QColor color)
	{
		m_color = color;
		this->setStyleSheet("QLabel { background-color : " + m_color.name() + "; border: 1px solid black; padding 10px;}");
	}

	QColor getColor() const
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
			setColor(color);
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
	Luminosity
} GradientType;

typedef enum
{
	LinearOffset			= 68,
	RadialOffset			= 62,
	TriangleOffset			= 56,
	CoonsPatchOffset		= 32,
	TensorCoonsPatchOffset  = 0
} GradientOffse;

struct Info
{
	GradientType	gradient;
	GradientOffse	offset;

	NSStructures::Point p0, p1;
	NSStructures::Point c0, c1;
	float				r0, r1;
	std::vector<NSStructures::Point>				triangle	= std::vector<NSStructures::Point>(3);
	std::vector<NSStructures::Point>				curve		= std::vector<NSStructures::Point>(12);
	std::vector<std::vector<NSStructures::Point>>	tensorcurve = std::vector<std::vector<NSStructures::Point>>(4, std::vector<NSStructures::Point>(4));

	std::vector<float>				triangle_parametrs		= std::vector<float>(3);
	std::vector<float>				curve_parametrs			= std::vector<float>(4);
	std::vector<std::vector<float>> tensor_curve_parametrs	= std::vector<std::vector<float>>(2, std::vector<float>(2));

	bool cont_b, cont_f;

	NSStructures::GradientInfo ginfo;
	std::vector<LONG>	c;
	std::vector<double>	p;
	int gradient_type;
	int n_colors;
};

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = nullptr);
	~MainWindow();

	void initializeColors(bool triangle);
	std::vector<agg::rgba8> qColor2rgba(bool triangle);
	std::vector<std::vector<agg::rgba8>> qColor2rgbaMatrix();
	std::vector<std::vector<agg::rgba8>> setAlphas();

	void setPoints(QImage *image);
	NSStructures::Point scaleCoord(NSStructures::Point p);

	void lineEdits2Points();
	void lineEdits2Parametrs();
	void checkBox2Continue();

private slots:

	void on_label_test_clicked();

	void on_label_test_mouse_move();

	void on_actionLinear_Gradient_triggered();

	void on_actionRadial_Gradient_triggered();

	void on_pushButton_clicked();

	void on_actionTriangle_Gradient_triggered();

	void on_actionTriangle_Parametric_Gradient_triggered();

	void on_actionCoons_Patch_Gradient_triggered();

	void on_actionCoons_Patch_Parametric_triggered();

	void on_actionTensor_Coons_Patch_Gradient_triggered();

	void on_actionTensor_Coons_Patch_Parametric_triggered();

	void on_actionactionLuminocity_Gradient_triggered();

private:
	QImage	img;
	Info	info;
	std::vector<NSStructures::Point> points;

	QList<QCheckBox *>				listOfCheckBox;
	QList<CustomLineEdit *>			listOfLines;
	QList<CustomColorLabel *>		listOfColorLabels;
	QList<CustomParametrLineEdit *> listOfParametricLines;
	QList<CustomAlphaLineEdit *>				listOfAlphas;

	Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
