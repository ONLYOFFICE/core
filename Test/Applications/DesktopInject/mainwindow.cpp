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
