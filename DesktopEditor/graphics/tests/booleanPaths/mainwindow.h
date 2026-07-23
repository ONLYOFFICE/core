/*
 * Copyright (C) Ascensio System SIA, 2009-2026
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation, together with the
 * additional terms provided in the LICENSE file.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For
 * details, see the GNU AGPL at: https://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA by email at info@onlyoffice.com
 * or by postal mail at 20A-6 Ernesta Birznieka-Upisha Street, Riga,
 * LV-1050, Latvia, European Union.
 *
 * The interactive user interfaces in modified versions of the Program
 * are required to display Appropriate Legal Notices in accordance with
 * Section 5 of the GNU AGPL version 3.
 *
 * No trademark rights are granted under this License.
 *
 * All non-code elements of the Product, including illustrations,
 * icon sets, and technical writing content, are licensed under the
 * Creative Commons Attribution-ShareAlike 4.0 International License:
 * https://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 * This license applies only to such non-code elements and does not
 * modify or replace the licensing terms applicable to the Program's
 * source code, which remains licensed under the GNU Affero General
 * Public License v3.
 *
 * SPDX-License-Identifier: AGPL-3.0-only
 */

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
