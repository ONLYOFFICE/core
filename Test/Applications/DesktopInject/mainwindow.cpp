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

#include "mainwindow.h"
#include <QDebug>
#include <QtGlobal>

#ifdef Q_OS_WIN
#include "windows.h"
#endif

CProcessEditor::CProcessEditor(const QString& sPath, const OpenFileType& type, const int& nId) : QProcess(nullptr)
{
    m_nId = nId;
    m_sPath = sPath;
    m_eType = type;
}

void CProcessEditor::StartEditor()
{
    switch (m_eType)
    {
    case oftEdit:
        m_sParams = "--edit=";
        break;
    case oftView:
        m_sParams = "--view=";
        break;
    case oftReview:
        m_sParams = "--review=";
        break;
    default:
        break;
    }

    QObject::connect(this, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(slotFinish(int,QProcess::ExitStatus)));
    QStringList args;
    args.push_back("--single-window-app");
    args.push_back(m_sParams + m_sPath);

    QString sApplicationPath;

#ifdef Q_OS_WIN
    wchar_t sBuffer[2048];
    DWORD dwBufferSize = sizeof(sBuffer);
    QString sApplicationDir;

    HKEY hKey = 0;
    HKEY hRoot = HKEY_LOCAL_MACHINE;
    if (ERROR_SUCCESS == RegOpenKeyExW(hRoot, L"SOFTWARE\\ONLYOFFICE\\DesktopEditors", 0, KEY_READ, &hKey))
    {
        if (ERROR_SUCCESS == RegQueryValueExW(hKey, L"AppPath", 0, NULL, (LPBYTE)sBuffer, &dwBufferSize))
        {
            std::wstring sValue = sBuffer;
            sApplicationDir = QString::fromStdWString(sValue);
            sApplicationPath = sApplicationDir + "\\editors.exe";
        }
    }

    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    env.insert("PATH", sApplicationDir + "\\converter;" + env.value("PATH"));
    setProcessEnvironment(env);
#endif

#ifdef Q_OS_LINUX
    sApplicationPath = "onlyoffice-desktopeditors";
#endif

    start(sApplicationPath, args);
}

void CProcessEditor::slotFinish(int nCode, QProcess::ExitStatus exitStatus)
{
    qDebug() << "File: " << m_sPath << ";; ExitCode: " << nCode;
    emit signalFinish(m_nId);
}

CSubProcesses::CSubProcesses()
{
    m_nCounterId = 0;
}
CProcessEditor* CSubProcesses::Create(const QString& sPath, const OpenFileType& type)
{
    ++m_nCounterId;
    CProcessEditor* pProcess = new CProcessEditor(sPath, type, m_nCounterId);
    QObject::connect(pProcess, &CProcessEditor::signalFinish, this, &CSubProcesses::slotFinish);
    m_mapProcesses.insert(m_nCounterId, pProcess);
    pProcess->StartEditor();
    return pProcess;
}
void CSubProcesses::CloseAll()
{
    QList<CProcessEditor*> arProcesses = m_mapProcesses.values();
    int nCount = arProcesses.count();
    for (int nI = 0; nI < nCount; ++nI)
    {
        CProcessEditor* pProcess = arProcesses.at(nI);
        pProcess->close();
    }
    m_mapProcesses.clear();
}
void CSubProcesses::slotFinish(int nId)
{
    CProcessEditor* pProcess = m_mapProcesses.value(nId);
    if (pProcess)
    {
        m_mapProcesses.remove(nId);
        pProcess->deleteLater();
    }
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    m_pButtonOpenEdit = new QPushButton(this);
    m_pButtonOpenEdit->setText("Open in editor");
    m_pButtonOpenView = new QPushButton(this);
    m_pButtonOpenView->setText("Open in viewer");

    QObject::connect(m_pButtonOpenEdit, &QPushButton::clicked, this, &MainWindow::pushButtonOpenEdit);
    QObject::connect(m_pButtonOpenView, &QPushButton::clicked, this, &MainWindow::pushButtonOpenView);

    setGeometry(200, 200, 800, 600);
    calculatePlaces();
}

MainWindow::~MainWindow()
{

}

void MainWindow::pushButtonOpenEdit()
{
    QString sPath = QFileDialog::getOpenFileName();
    if (sPath.isEmpty())
        return;
    m_oProcesses.Create(sPath, oftEdit);
}

void MainWindow::pushButtonOpenView()
{
    QString sPath = QFileDialog::getOpenFileName();
    if (sPath.isEmpty())
        return;
    m_oProcesses.Create(sPath, oftView);
}

void MainWindow::calculatePlaces()
{
    int nButtonW = 100;
    int nButtonH = 40;
    int nBetween = 40;

    int nX = (width() - nButtonW) >> 1;
    int nY = (height() - (nButtonH + nBetween + nButtonH)) >> 1;

    m_pButtonOpenEdit->setGeometry(nX, nY, nButtonW, nButtonH);
    m_pButtonOpenView->setGeometry(nX, nY + nButtonH + nBetween, nButtonW, nButtonH);
}
void MainWindow::resizeEvent(QResizeEvent*)
{
    calculatePlaces();
}
void MainWindow::closeEvent(QCloseEvent*)
{
    m_oProcesses.CloseAll();
}
