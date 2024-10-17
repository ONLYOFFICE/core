#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QPushButton>
#include <QPoint>
#include <QMouseEvent>
#include "../../pro/Graphics.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

const double NEGATIVE_OFFSET= 400.0;
const double RECTANGLE[4]	= {-400.0, -400.0, 200.0, 200.0};
const double TRIANGLE[6]	= {-300.0, -400.0, 100.0, 200.0, -100.0, 100.0};
const double CROSS[24]		= {-375.0, -325.0, 50.0, 0.0, 50.0, -50.0,
							   100.0, -50.0, 100.0, 0.0, 150.0, 0.0,
							   150.0, 50.0, 100.0, 50.0, 100.0, 100.0,
							   50.0, 100.0, 50.0, 50.0, 0.0, 50.0};

class CustomLabel : public QLabel
{
	Q_OBJECT

public:
	CustomLabel(QWidget *parent = nullptr);

	QPointF GetStartPoint() const noexcept;
	double GetDifferenceX() const noexcept;
	double GetDifferenceY() const noexcept;
	bool GetMovable()		const noexcept;

signals:
	void mousePress();
	void mouseMove();

protected:
	void mousePressEvent(QMouseEvent *event)	override;
	void mouseMoveEvent(QMouseEvent *event)		override;
	void mouseReleaseEvent(QMouseEvent *event)	override;

private:
	bool Movable	= false;
	QPointF StartP	= {0.0, 0.0};
	QPointF CurrP	= {0.0, 0.0};
};

class CustomButton : public QPushButton
{
	Q_OBJECT

public:
	CustomButton(QWidget *parent = nullptr);
};

class BooleanButton : public QPushButton
{
	Q_OBJECT

public:
	BooleanButton(QWidget *parent = nullptr);
};

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = nullptr);
	~MainWindow();

	Aggplus::CGraphicsPath SetPath(double scale, double offsetX, double offsetY, QString Figure);
	void AddPath(NSGraphics::IGraphicsRenderer* pathRenderer, const Aggplus::CGraphicsPath& path, bool isResult = false);
	void Draw(bool drawResult = false);
	void SetCoords(QLabel* label, const Aggplus::CGraphicsPath& path);

	void DrawPath1();
	void DrawPath2();
	void BooleanOp();

private slots:	
	void CheckMousePress();
	void Move();
	void SetCommand();
	void SetFigure();
	void on_horizontalSlider_sliderMoved(int position);
	void on_horizontalSlider_2_sliderMoved(int position);

public:
	QString Figure1;
	QString Figure2;
	Aggplus::BooleanOpType Op;

	double Scale[2] = {1.0, 1.0};
	double Offsets[4] = {100.0, 100.0, 200.0, 200.0};
	double OldOffsets[4];

	bool Move1	 = false;
	bool Move2	 = false;

	Aggplus::CGraphicsPath Path1;
	Aggplus::CGraphicsPath Path2;
	Aggplus::CGraphicsPath Result;

	Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
