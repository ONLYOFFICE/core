#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "../../pro/Graphics.h"

#include <QMainWindow>
#include <QPushButton>
#include <QColor>
#include <QColorDialog>
#include <QPalette>

class CColorButton final : public QPushButton
{
	Q_OBJECT
public:
	CColorButton(QWidget *parent = nullptr) : QPushButton(parent)
	{
		connect(this, &QPushButton::clicked, this, &CColorButton::onClicked);
	}

	~CColorButton() {}

	QColor GetColor() const
	{
		return m_oColor;
	}

	void SetColor(const QColor& color)
	{
		if (color == m_oColor)
			return;

		m_oColor = color;
		setStyleSheet("QPushButton { background-color : " + m_oColor.name()
					  + "; border: 1px solid black; padding 10px;}");

		emit ColorChanged();
	}

signals:
	void ColorChanged();

public slots:
	void onClicked()
	{
		QColorDialog color_dialog;
		auto color = color_dialog.getColor();

		if (color.isValid())
			SetColor(color);
	}

private:
	QColor m_oColor;
};

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

public slots:
	void Draw();

private:
	Ui::MainWindow *ui;

	NSFonts::IApplicationFonts* m_oAppFonts;
	NSFonts::IFontManager*		m_oFontManager;
};
#endif // MAINWINDOW_H
