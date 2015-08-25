#include <QFile>
#include "QAscApplicationManager.h"
#include "mainwindow.h"
#include <QScreen>

#include <shlwapi.h>
#include <shlobj.h>

int main( int argc, char *argv[] )
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
    FILE* f = fopen("E:\\12345.txt", "a+");
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

        WCHAR szPath[MAX_PATH];
        // Get path for each computer, non-user specific and non-roaming data.
        if ( SUCCEEDED( SHGetFolderPathW( NULL, CSIDL_COMMON_APPDATA, NULL, 0, szPath ) ) )
        {
            // Append product-specific path
            std::wstring sAppData(szPath);
            sAppData += L"/ONLYOFFICE";
            oManager.m_oSettings.SetUserDataPath(sAppData);
        }

        oCef.Init_CEF(&oManager);
        return aa.exec();
    }

    QApplication a(argc, argv);
    CApplicationCEF* application_cef = new CApplicationCEF();

    CAscApplicationManager* pApplicationManager = new QAscApplicationManager();

    WCHAR szPath[MAX_PATH];
    // Get path for each computer, non-user specific and non-roaming data.
    if ( SUCCEEDED( SHGetFolderPathW( NULL, CSIDL_COMMON_APPDATA, NULL, 0, szPath ) ) )
    {
        // Append product-specific path
        std::wstring sAppData(szPath);
        sAppData += L"/ONLYOFFICE";
        pApplicationManager->m_oSettings.SetUserDataPath(sAppData);
    }

    qreal ww = a.primaryScreen()->logicalDotsPerInch();
    qreal hh = a.primaryScreen()->physicalDotsPerInch();

    application_cef->Init_CEF(pApplicationManager);

    pApplicationManager->CheckFonts();

    a.setStyleSheet("#mainPanel { margin: 0; padding: 0; }\
#systemPanel { margin: 0; padding: 0; } \
#centralWidget { background: #fff; }");

    // Font
    QFont mainFont = a.font();
    mainFont.setStyleStrategy( QFont::PreferAntialias );
    a.setFont( mainFont );

    // Background color
    HBRUSH windowBackground = CreateSolidBrush( RGB( 49, 52, 55 ) );

    // Create window
    CMainWindow window( &a, windowBackground, 100, 100, 1024 * 2, 600 * 2, pApplicationManager );
    window.setMinimumSize( 100, 100 );

    ((QAscApplicationManager*)pApplicationManager)->m_pPanel = window.mainPanel;

    // Launch
    a.exec();

    // release all subprocesses
    pApplicationManager->CloseApplication();

    delete application_cef;
    delete pApplicationManager;
}
