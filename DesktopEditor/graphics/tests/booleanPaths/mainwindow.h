#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "../../pro/Graphics.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = nullptr);
	~MainWindow();

	void PrepareClip();
	void Reset(Aggplus::CGraphicsPath *path);
	void AddPath(NSGraphics::IGraphicsRenderer* pathRenderer, Aggplus::CGraphicsPath* path);
	void Draw(Aggplus::CGraphicsPath *path = nullptr);
	void AddCommand(Aggplus::CGraphicsPath* path, agg::path_commands_e, float x0,
					float y0, float x1 = 0.0, float y1 = 0.0, float x2 = 0.0, float y2 = 0.0);

private slots:
	void on_pushButton_clicked();

	void on_pushButton_2_clicked();

	void on_pushButton_3_clicked();

	void on_pushButton_4_clicked();

	void on_pushButton_5_clicked();

	void on_pushButton_6_clicked();

	void on_pushButton_10_clicked();

	void on_pushButton_7_clicked();

	void on_pushButton_8_clicked();

	void on_pushButton_11_clicked();

	void on_pushButton_14_clicked();

	void on_pushButton_9_clicked();

	void on_pushButton_12_clicked();

private:
	Aggplus::CGraphicsPath* Path1;
	Aggplus::CGraphicsPath* Path2;

	Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
