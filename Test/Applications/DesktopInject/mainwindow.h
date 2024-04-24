/*
 * (c) Copyright Ascensio System SIA 2010-2024
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at 20A-6 Ernesta Birznieka-Upish
 * street, Riga, Latvia, EU, LV-1050.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
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
