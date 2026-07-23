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
#include <QPushButton>
#include <QFileDialog>
#include <QProcess>
#include <QResizeEvent>
#include <QCloseEvent>

enum OpenFileType
{
    oftEdit = 1,
    oftView = 2,
    oftReview = 3
};

class CProcessEditor : public QProcess
{
    Q_OBJECT

public:
    QString m_sPath;
    QString m_sParams;
    OpenFileType m_eType;
    int m_nId;

public:
    CProcessEditor(const QString& sPath, const OpenFileType& type, const int& nId);
    void StartEditor();

signals:
    void signalFinish(int id);

public slots:
    void slotFinish(int nCode, QProcess::ExitStatus exitStatus);
};

class CSubProcesses : public QProcess
{
    Q_OBJECT

private:
    QMap<int, CProcessEditor*> m_mapProcesses;
    int m_nCounterId;

public:
    CSubProcesses();
    CProcessEditor* Create(const QString& sPath, const OpenFileType& type);
    void CloseAll();

public slots:
    void slotFinish(int nId);
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    QPushButton* m_pButtonOpenEdit;
    QPushButton* m_pButtonOpenView;
    CSubProcesses m_oProcesses;

public slots:
    void pushButtonOpenEdit();
    void pushButtonOpenView();

public:
    void calculatePlaces();
    void resizeEvent(QResizeEvent*);
    void closeEvent(QCloseEvent*);
};

#endif // MAINWINDOW_H
