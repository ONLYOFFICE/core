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
#include <QTextCharFormat>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


#include <QStandardItem>
#include "../../../../xml/include/xmlutils.h"

class MainWindow : public QMainWindow
{
        Q_OBJECT

    public:
        MainWindow(QWidget *parent = nullptr);
        ~MainWindow();

        bool ConvertToRaster(const std::wstring& wsPathToFile, bool bWithXmlFile = true);
        bool ConvertToEmf(const std::wstring& wsPathToXmlFile, const std::wstring& wsPathToNewEmfFile = L"");

        bool SaveInXmlFile(const std::wstring& wsPathToFile);

        void DisplayingFile(const std::wstring& wsPathToFile, bool bWithXmlFile = true);

    private slots:
        void on_expandButton_clicked();
        void on_ModButton_clicked();
        void on_actionExit_triggered();
        void on_actionChange_File_triggered();
        void on_actionStatistics_triggered();
        void on_actionSave_XML_as_triggered();

        void on_actionSave_EMF_as_triggered();

private:
        Ui::MainWindow      *ui;

        bool ReadXmlFile(const std::wstring& wsPathToXmlFile);
        void Clear();
    protected:
        virtual void resizeEvent(QResizeEvent *);

};
#endif // MAINWINDOW_H
