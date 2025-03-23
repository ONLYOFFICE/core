#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QStringList>
#include <QMap>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = nullptr);
	~MainWindow();

private slots:
	void on_pushButton_clicked();

	void on_pushButton_2_clicked();

private:
	Ui::MainWindow *ui;

	QStringList		m_arFileNames;

	QStringList m_arExtensions = {
		"bmp",
		"gif",
		"jpg",
		"png",
		"ico",
		"tif",
		"tga",
		"pcx",
		"wbmp",
		"jp2",
		"jpc",
		"pgx",
		"pnm",
		"ras",
		"mng",
		"ska",
		"raw",
		"psd",
		"pic",
	};
	QMap<QString, int> m_mapExtensionCodes = {
		{"bmp", 1},
		{"gif", 2},
		{"jpg", 3},
		{"png", 4},
		{"ico", 5},
		{"tif", 6},
		{"tga", 7},
		{"pcx", 8},
		{"wbmp", 9},
		{"jp2", 11},
		{"jpc", 12},
		{"pgx", 13},
		{"pnm", 14},
		{"ras", 15},
		{"mng", 17},
		{"ska", 18},
		{"raw", 19},
		{"psd", 20},
		{"pic", 25},
	};
};

#endif // MAINWINDOW_H
