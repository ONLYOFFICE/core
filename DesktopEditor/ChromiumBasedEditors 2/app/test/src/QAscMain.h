#pragma once

#include <QFile>
#include <QScreen>
#include <QDir>

#include "QAscApplicationManager.h"

#ifdef WIN32
#include <shlwapi.h>
#include <shlobj.h>
#endif

#include <QStandardPaths>
#include <QApplication>

static std::wstring GetAppDataPath()
{
#ifdef WIN32
    WCHAR szPath[MAX_PATH];
    // Get path for each computer, non-user specific and non-roaming data.
    if ( SUCCEEDED( SHGetFolderPathW( NULL, CSIDL_COMMON_APPDATA, NULL, 0, szPath ) ) )
    {
        // TODO: [0]
        // QStringList arr = QStandardPaths::standardLocations(QStandardPaths::AppDataLocation);

        // Append product-specific path
        std::wstring sAppData(szPath);
        sAppData += L"/ONLYOFFICE";
        return sAppData;
    }
#else
    std::wstring sAppData = QDir::homePath().toStdWString();
    sAppData += L"/ONLYOFFICE";
    return sAppData;
#endif

    return L"";
}

#include <QMainWindow>
#include <QApplication>
class QAscMainWindow : public QMainWindow
{
    Q_OBJECT
    QWidget* m_pMainPanel;

public:
    explicit QAscMainWindow(QWidget *parent, CAscApplicationManager* pAppManager) : QMainWindow(parent)
    {
#if 0
        this->resize(2000, 1000);
#else
        this->resize(1200, 700);
#endif
        m_pMainPanel = new QAscMainPanel(this, pAppManager, false);
        this->setCentralWidget(m_pMainPanel);

        ((QAscApplicationManager*)pAppManager)->m_pPanel = (QAscMainPanel*)m_pMainPanel;

        QMetaObject::connectSlotsByName(this);

        pAppManager->StartSpellChecker();
        pAppManager->StartKeyboardChecker();
    }

    void closeEvent(QCloseEvent*)
    {
        QAscMainPanel* pPanel = (QAscMainPanel*)m_pMainPanel;
        pPanel->GetAppManager()->GetApplication()->ExitMessageLoop();
    }

    ~QAscMainWindow()
    {

    }
};


static int AscEditor_Main( int argc, char *argv[] )
{
    bool bIsChromiumSubprocess = false;
    for (int i = 0; i < argc; ++i)
    {
        if ((0 == strcmp("--type=gpu-process", argv[i])) ||
            (0 == strcmp("--type=renderer", argv[i])) ||
            (0 == strcmp("--type=zygote", argv[i])))
        {
            bIsChromiumSubprocess = true;
            break;
        }
    }

#if 0
    FILE* f = fopen("/home/oleg/activex/1/12345.txt", "a+");
    fprintf(f, "-----------------------------------------------\n");
    for (int i = 0; i < argc; ++i)
    {
        fprintf(f, argv[i]);
        fprintf(f, "\n");
    }
    fprintf(f, "-----------------------------------------------\n");
    fclose(f);
#endif

    if (bIsChromiumSubprocess)
    {
        QApplication aa(argc, argv);
        CApplicationCEF oCef;
        CAscApplicationManager oManager;

        std::wstring sUserPath = GetAppDataPath();
        oManager.m_oSettings.SetUserDataPath(sUserPath);

        oCef.Init_CEF(&oManager, argc, argv);

        bool bIsOwnMessageLoop = false;
        int nResult = oCef.RunMessageLoop(bIsOwnMessageLoop);

        if (bIsOwnMessageLoop)
            return nResult;

        return aa.exec();
    }

    QApplication a(argc, argv);
    CApplicationCEF* application_cef = new CApplicationCEF();

    CAscApplicationManager* pApplicationManager = new QAscApplicationManager();

    std::wstring sUserPath = GetAppDataPath();
    pApplicationManager->m_oSettings.SetUserDataPath(sUserPath);

    qreal ww = a.primaryScreen()->logicalDotsPerInch();
    qreal hh = a.primaryScreen()->physicalDotsPerInch();

    application_cef->Init_CEF(pApplicationManager, argc, argv);

    pApplicationManager->CheckFonts();

    a.setStyleSheet("#mainPanel { margin: 0; padding: 0; }\
#systemPanel { margin: 0; padding: 0; } \
#centralWidget { background: #313437; } \
QPushButton:focus{border:none;outline:none;}");

    // Font
    QFont mainFont = a.font();
    mainFont.setStyleStrategy( QFont::PreferAntialias );
    a.setFont( mainFont );

    // Create window
    QAscMainWindow w(NULL, pApplicationManager);
    w.show();

    bool bIsOwnMessageLoop = false;
    application_cef->RunMessageLoop(bIsOwnMessageLoop);

    if (!bIsOwnMessageLoop)
    {
        // Launch
        a.exec();
    }

    // release all subprocesses
    pApplicationManager->CloseApplication();

    delete application_cef;
    delete pApplicationManager;

    return 0;
}
