#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QLabel>
#include <QListWidgetItem>
#include <QMainWindow>
#include "../../../../DesktopEditor/graphics/structures.h"

QT_BEGIN_NAMESPACE
namespace Ui
{
	class MainWindow;
}
QT_END_NAMESPACE

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

typedef enum
{
	NoColorspaceType,
	Rainbow,
	BlackAndWhite,
	RedAndBlue,
	Pastel
} ColorspaceType;

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
	ColorspaceType colorspace = NoColorspaceType;
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
	QImage img;
	QLabel *lable;
	std::vector<Point> points;
	Info info;

private slots:

	void on_actionLinear_Gradient_triggered();

	void on_actionRadial_Gradient_triggered();

	void on_BAW_Colorspace_Radio_Button_clicked();

	void on_RAB_Colorspace_Radio_Button_clicked();

	void on_Pastel_Colorspace_Radio_Button_clicked();

	void on_Rainbow_Colorspace_Radio_Button_clicked();

	void on_pushButton_clicked();

	void on_Continue_Shading_Forward_clicked(bool checked);

	void on_checkBox_2_clicked(bool checked);

	void on_Continue_Shading_Forward_2_clicked(bool checked);

	void on_Continue_Shading_Backward_2_clicked(bool checked);

	void on_First_X_Coordinate_Input_editingFinished();

	void on_First_Y_Coordinate_Input_editingFinished();

	void on_Second_X_Coordinate_Input_editingFinished();

	void on_Second_Y_Coordinate_Input_editingFinished();

	void on_First_Center_X_Coordinate_Input_editingFinished();

	void on_First_Center_Y_Coordinate_Input_editingFinished();

	void on_Second_Center_X_Coordinate_Input_editingFinished();

	void on_Second_Center_Y_Coordinate_Input_editingFinished();

	void on_First_Radius_Input_editingFinished();

	void on_Second_Radius_Input_editingFinished();

	void on_actionTriangle_Gradient_triggered();

	void on_First_Vertex_X_Coordinate_Input_editingFinished();

	void on_First_Vertex_Y_Coordinate_Input_editingFinished();

	void on_Second_Vertex_X_Coordinate_Input_editingFinished();

	void on_Second_Vertex_Y_Coordinate_Input_editingFinished();

	void on_Third_Vertex_X_Coordinate_Input_editingFinished();

	void on_Third_Vertex_Y_Coordinate_Input_editingFinished();

	void on_actionTriangle_Parametric_Gradient_triggered();

	void on_actionCoons_Patch_Gradient_triggered();

	void on_actionCoons_Patch_Parametric_triggered();

	void on_actionTensor_Coons_Patch_Gradient_triggered();

	void on_actionTensor_Coons_Patch_Parametric_triggered();

	void on_First_Vertex_X_Coordinate_Input_3_editingFinished();

	void on_First_Vertex_Y_Coordinate_Input_3_editingFinished();

	void on_Second_Vertex_X_Coordinate_Input_3_editingFinished();

	void on_Second_Vertex_Y_Coordinate_Input_3_editingFinished();

	void on_Third_Vertex_X_Coordinate_Input_3_editingFinished();

	void on_Third_Vertex_Y_Coordinate_Input_3_editingFinished();

	void on_Fourth_Vertex_X_Coordinate_Input_3_editingFinished();

	void on_Fourth_Vertex_Y_Coordinate_Input_3_editingFinished();

	void on_First_X_Coordinate_First_Edge_editingFinished();

	void on_First_Y_Coordinate_First_Edge_editingFinished();

	void on_Second_X_Coordinate_First_Edge_editingFinished();

	void on_Second_Y_Coordinate_First_Edge_editingFinished();

	void on_First_X_Coordinate_Second_Edge_editingFinished();

	void on_First_Y_Coordinate_Second_Edge_editingFinished();

	void on_Second_X_Coordinate_Second_Edge_editingFinished();

	void on_Second_Y_Coordinate_Second_Edge_editingFinished();

	void on_First_X_Coordinate_Third_Edge_editingFinished();

	void on_First_Y_Coordinate_Third_Edge_editingFinished();

	void on_Second_X_Coordinate_Third_Edge_editingFinished();

	void on_Second_Y_Coordinate_Third_Edge_editingFinished();

	void on_First_X_Coordinate_Fourth_Edge_editingFinished();

	void on_First_Y_Coordinate_Fourth_Edge_editingFinished();

	void on_Second_X_Coordinate_Fourth_Edge_editingFinished();

	void on_Second_Y_Coordinate_Fourth_Edge_editingFinished();

	void on_First_X_Coordinate_First_Edge_3_editingFinished();

	void on_First_Y_Coordinate_First_Edge_3_editingFinished();

	void on_Second_X_Coordinate_First_Edge_3_editingFinished();

	void on_Second_Y_Coordinate_First_Edge_3_editingFinished();

	void on_Third_X_Coordinate_First_Edge_3_editingFinished();

	void on_Third_Y_Coordinate_First_Edge_3_editingFinished();

	void on_Fourth_X_Coordinate_First_Edge_3_editingFinished();

	void on_Fourth_Y_Coordinate_First_Edge_3_editingFinished();

	void on_First_X_Coordinate_Second_Edge_3_editingFinished();

	void on_First_Y_Coordinate_Second_Edge_3_editingFinished();

	void on_Second_X_Coordinate_Second_Edge_3_editingFinished();

	void on_Second_Y_Coordinate_Second_Edge_3_editingFinished();

	void on_Third_X_Coordinate_Second_Edge_3_editingFinished();

	void on_Third_Y_Coordinate_Second_Edge_3_editingFinished();

	void on_Fourth_X_Coordinate_Second_Edge_3_editingFinished();

	void on_Fourth_Y_Coordinate_Second_Edge_3_editingFinished();

	void on_First_X_Coordinate_Third_Edge_3_editingFinished();

	void on_First_Y_Coordinate_Third_Edge_3_editingFinished();

	void on_Second_X_Coordinate_Third_Edge_3_editingFinished();

	void on_Second_Y_Coordinate_Third_Edge_3_editingFinished();

	void on_Third_X_Coordinate_Third_Edge_3_editingFinished();

	void on_Third_Y_Coordinate_Third_Edge_3_editingFinished();

	void on_Fourth_X_Coordinate_Third_Edge_3_editingFinished();

	void on_Fourth_Y_Coordinate_Third_Edge_3_editingFinished();

	void on_First_X_Coordinate_Fourth_Edge_3_editingFinished();

	void on_First_Y_Coordinate_Fourth_Edge_3_editingFinished();

	void on_Second_X_Coordinate_Fourth_Edge_3_editingFinished();

	void on_Second_Y_Coordinate_Fourth_Edge_3_editingFinished();

	void on_Third_X_Coordinate_Fourth_Edge_3_editingFinished();

	void on_Third_Y_Coordinate_Fourth_Edge_3_editingFinished();

	void on_Fourth_X_Coordinate_Fourth_Edge_3_editingFinished();

	void on_Fourth_Y_Coordinate_Fourth_Edge_3_editingFinished();

private:
	Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
