#include <windows.h>

#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QPushButton>
#include <QScrollArea>
#include <QFileDialog>

#include "QAscMainPanel.h"
#include "QAscTabWidget.h"

#include <QStandardPaths>

#include "../../../lib/include/qascprinter.h"

QAscMainPanel::QAscMainPanel( QWidget* parent, CAscApplicationManager* pManager, bool bIsCustomWindow ) : QWidget( parent )
{
    m_bIsCustomWindow = bIsCustomWindow;
    m_pMainButtonsController = NULL;

    QObject::connect(this, SIGNAL( signal_Download(NSEditorApi::CAscDownloadFileInfo*)), this, SLOT( slot_Download(NSEditorApi::CAscDownloadFileInfo*)), Qt::QueuedConnection );
    QObject::connect(this, SIGNAL( signal_Modified(int,bool)) , this, SLOT( slot_Modified(int,bool)), Qt::QueuedConnection );
    QObject::connect(this, SIGNAL( signal_Print(int,int)) , this, SLOT( slot_Print(int,int)), Qt::QueuedConnection );
    QObject::connect(this, SIGNAL( signal_DialogSave()), this, SLOT( slot_DialogSave()), Qt::QueuedConnection);
    QObject::connect(this, SIGNAL( signal_CheckKeyboard()), this, SLOT( slot_CheckKeyboard()), Qt::QueuedConnection);

    m_pManager = pManager;

    setObjectName( "mainPanel" );

    QGridLayout *mainGridLayout = new QGridLayout();
    mainGridLayout->setSpacing( 0 );
    mainGridLayout->setMargin( 0 );
    setLayout( mainGridLayout );

    // Central widget
    QWidget *centralWidget = new QWidget( this );
    centralWidget->setObjectName( "centralWidget" );
    centralWidget->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding );
    //centralWidget->setStyleSheet("background-color:#313437");

    QAscTabWidget* pTabs = new QAscTabWidget(centralWidget);
    pTabs->setGeometry(0, 0, centralWidget->width(), centralWidget->height());
    pTabs->m_pManager = m_pManager;

    m_pTabs = (QTabWidget*)pTabs;

    m_pSaveModifiedButton = new QPushButton("", centralWidget);

    m_pSaveModifiedButton->setObjectName( "pushButtoneModified" );
    QObject::connect( m_pSaveModifiedButton, SIGNAL( clicked() ), this, SLOT( pushButtonSaveClicked()) );

    m_pSaveModifiedButton->setStyleSheet("background-color:#FFFFFF;border:none;margin:0;padding:0;");

    if (m_bIsCustomWindow)
    {
        // Minimize
        m_pButtonMinimize = new QPushButton( "", centralWidget );
        m_pButtonMinimize->setObjectName( "pushButtonMinimize" );
        QObject::connect( m_pButtonMinimize, SIGNAL( clicked() ), this, SLOT( pushButtonMinimizeClicked() ) );

        m_pButtonMinimize->setStyleSheet("QPushButton {background-image:url(:/Icons/new_collapse_normal.png);border:none;margin:0;padding:0;}\
    QPushButton::hover {background-image:url(:/Icons/new_collapse_hover.png);border:none;margin:0;padding:0;}\
    QPushButton::pressed {background-image:url(:/Icons/new_collapse_hover.png);border:none;margin:0;padding:0;}");

        // Maximize
        m_pButtonMaximize = new QPushButton( "", centralWidget );
        m_pButtonMaximize->setObjectName( "pushButtonMaximize" );
        QObject::connect( m_pButtonMaximize, SIGNAL( clicked() ), this, SLOT( pushButtonMaximizeClicked() ) );

        m_pButtonMaximize->setStyleSheet("QPushButton {background-image:url(:/Icons/new_deploy_normal.png);border:none;margin:0;padding:0;}\
    QPushButton::hover {background-image:url(:/Icons/new_deploy_hover.png);border:none;margin:0;padding:0;}\
    QPushButton::pressed {background-image:url(:/Icons/new_deploy_hover.png);border:none;margin:0;padding:0;}");

        // Close
        m_pButtonClose = new QPushButton( "", centralWidget );
        m_pButtonClose->setObjectName( "pushButtonClose" );
        QObject::connect( m_pButtonClose, SIGNAL( clicked() ), this, SLOT( pushButtonCloseClicked() ) );

        m_pButtonClose->setStyleSheet("QPushButton {background-image:url(:/Icons/new_exit_normal.png);border:none;margin:0;padding:0;}\
    QPushButton::hover {background-image:url(:/Icons/new_exit_hover.png);border:none;margin:0;padding:0;}\
    QPushButton::pressed {background-image:url(:/Icons/new_exit_hover.png);border:none;margin:0;padding:0;}");
    }
    else
    {
        m_pButtonMinimize = NULL;
        m_pButtonMaximize = NULL;
        m_pButtonClose = NULL;
    }

    // Main
    m_pButtonMain = new QPushButton( "", centralWidget );
    m_pButtonMain->setObjectName( "pushButtonMain" );
    QObject::connect( m_pButtonMain, SIGNAL( clicked() ), this, SLOT( pushButtonMainClicked() ) );

    m_pButtonMain->setStyleSheet("QPushButton {background-image:url(:/Icons/Documents_active_normal.png);border:none;margin:0;padding:0;}\
QPushButton::hover {background-image:url(:/Icons/Documents_active_normal.png);border:none;margin:0;padding:0;}\
QPushButton::pressed {background-image:url(:/Icons/Documents_active_hover.png);border:none;margin:0;padding:0;}");

    QCefView* pMainWidget = new QCefView(centralWidget);
    pMainWidget->Create(m_pManager, cvwtSimple);
    pMainWidget->setObjectName( "mainPanel" );
    pMainWidget->setHidden(false);

    //pMainWidget->GetCefView()->load(L"https://ascdesktop.teamlab.info/products/files/?desktop=true");
    //pMainWidget->GetCefView()->load(L"https://testinfo.teamlab.info/products/files/?desktop=true");
    //pMainWidget->GetCefView()->load(L"https://denisdenis.teamlab.info/products/files/?desktop=true");
    //pMainWidget->GetCefView()->load(L"https://denismy.teamlab.info/products/files/?desktop=true");
    //pMainWidget->GetCefView()->load(L"file:///C:/Users/oleg.korshul/Desktop/desktop/index.html");
    //pMainWidget->GetCefView()->load(L"file:///C:/Users/oleg.korshul/Desktop/desktop2/index.html");
    pMainWidget->GetCefView()->load(L"https://koroleg.teamlab.info/products/files/?desktop=true");
    //pMainWidget->GetCefView()->load(L"http://isa2:8086");

    m_pMainWidget = (QWidget*)pMainWidget;
    pTabs->m_pMainWidget = m_pMainWidget;
    pTabs->m_pMainButton = m_pButtonMain;

    m_pSeparator = new QWidget(centralWidget);
    m_pSeparator->setObjectName("separator");
    m_pSeparator->setStyleSheet("background-color:#D6D6D7");
    m_pSeparator->setGeometry(0, 28, this->width(), 1);

    mainGridLayout->addWidget( centralWidget );

    m_pDownloadBackground = new QWidget(centralWidget);
    m_pDownloadBackground->setObjectName("downloadBackground");
    m_pDownloadBackground->setStyleSheet("background-color:#646464;");

    m_pDownloadLable = new QLabel(m_pDownloadBackground);
    m_pDownloadLable->setObjectName("downloadLable");

#if 1
    QPalette palette;

    //white text
    QBrush brush(QColor(0, 0, 0, 255));
    brush.setStyle(Qt::SolidPattern);

    //black background
    QBrush brush1(QColor(0x64, 0x64, 0x64, 255));
    brush1.setStyle(Qt::SolidPattern);

    //set white text
    palette.setBrush(QPalette::Active, QPalette::WindowText, brush);
    palette.setBrush(QPalette::Inactive, QPalette::WindowText, brush);

    //set black background
    palette.setBrush(QPalette::Active, QPalette::Window, brush1);
    palette.setBrush(QPalette::Inactive, QPalette::Window, brush1);

    //set palette
    m_pDownloadLable->setPalette(palette);

    //set font
    QFont font;
    font.setPixelSize(32);
    font.setBold(true);

    m_pDownloadLable->setFont(font);
    m_pDownloadLable->setAutoFillBackground(true);

    m_pDownloadLable->setText("Oleg");
#endif

    m_pDownloadBackground->setHidden(true);

    RecalculatePlaces();

    m_pManager->SetEventListener(this);
}

void QAscMainPanel::RecalculatePlaces()
{
    int nWindowW = this->width();
    int nWindowH = this->height();
    int nCaptionH = 29;

    m_pTabs->setGeometry(0, 0, nWindowW, nWindowH);
    m_pButtonMain->setGeometry(0, 0, 108, nCaptionH);

    m_pSeparator->setGeometry(0, 28, this->width(), 1);

    int nStartOffset = 12;
    int nBetweenApp = 12;
    int nButtonW = 12;
    int nY = (nCaptionH - nButtonW) >> 1;
    nY = 5;

    if (m_bIsCustomWindow)
    {
        m_pButtonClose->setGeometry(nWindowW - nStartOffset - nButtonW, nY, nButtonW, nButtonW);
        m_pButtonMaximize->setGeometry(nWindowW - nStartOffset - 2 * nButtonW - nBetweenApp, nY, nButtonW, nButtonW);
        m_pButtonMinimize->setGeometry(nWindowW - nStartOffset - 3 * nButtonW - 2 * nBetweenApp, nY, nButtonW, nButtonW);

        m_pSaveModifiedButton->setGeometry(nWindowW - nStartOffset - 4 * nButtonW - 3 * nBetweenApp, nY, nButtonW, nButtonW);
    }
    else
    {
        m_pSaveModifiedButton->setGeometry(nWindowW - nStartOffset - nButtonW, nY, nButtonW, nButtonW);
    }

    m_pMainWidget->setGeometry(0, nCaptionH, nWindowW, nWindowH - nCaptionH);

    m_pDownloadBackground->setGeometry(0, nWindowH - 100, nWindowW, 100);
    m_pDownloadLable->setGeometry(0, 0, nWindowW, 100);
}

void QAscMainPanel::pushButtonSaveClicked()
{
#if 0
    int nCount = m_pTabs->tabBar()->count();
    for (int i = 0; i < nCount; ++i)
    {
        if (i != m_pTabs->tabBar()->currentIndex())
        {
            CCefView* pCefView = ((QCefView*)m_pTabs->widget(m_pTabs->tabBar()->currentIndex()))->GetCefView();
            pCefView->focus(false);
        }
    }
    return;
#endif

#if 0
    if (-1 != m_pTabs->tabBar()->currentIndex())
    {
        CCefView* pCefView = ((QCefView*)m_pTabs->widget(m_pTabs->tabBar()->currentIndex()))->GetCefView();
        NSEditorApi::CAscMenuEvent* pEvent = new NSEditorApi::CAscMenuEvent();
        pEvent->m_nType = ASC_MENU_EVENT_TYPE_CEF_PRINT_START;
        pCefView->Apply(pEvent);
    }
#endif

    if (m_pSaveModifiedButton->styleSheet() != "background-color:#00FF00;border:none;margin:0;padding:0;")
        return;

    bool bIsMain = !m_pMainWidget->isHidden();
    if (bIsMain)
        return;

    if (-1 == m_pTabs->tabBar()->currentIndex())
        return;

    CCefView* pCefView = ((QCefView*)m_pTabs->widget(m_pTabs->tabBar()->currentIndex()))->GetCefView();
    NSEditorApi::CAscMenuEvent* pEvent = new NSEditorApi::CAscMenuEvent();
    pEvent->m_nType = ASC_MENU_EVENT_TYPE_CEF_SAVE;
    pCefView->Apply(pEvent);
}

void QAscMainPanel::pushButtonMainClicked()
{
    // test logout
    //m_pManager->Logout(L"ascdesktop.teamlab.info");

    m_pMainWidget->setHidden(!m_pMainWidget->isHidden());

    if (m_pMainWidget->isHidden())
    {
        m_pButtonMain->setStyleSheet("QPushButton {background-image:url(:/Icons/Documents_disabled_normal.png);border:none;margin:0;padding:0;}\
    QPushButton::hover {background-image:url(:/Icons/Documents_disabled_normal.png);border:none;margin:0;padding:0;}\
    QPushButton::pressed {background-image:url(:/Icons/Documents_disabled_hover.png);border:none;margin:0;padding:0;}");

        ((QCefView*)m_pMainWidget)->GetCefView()->focus();
    }
    else
    {
        m_pButtonMain->setStyleSheet("QPushButton {background-image:url(:/Icons/Documents_active_normal.png);border:none;margin:0;padding:0;}\
    QPushButton::hover {background-image:url(:/Icons/Documents_active_normal.png);border:none;margin:0;padding:0;}\
    QPushButton::pressed {background-image:url(:/Icons/Documents_active_hover.png);border:none;margin:0;padding:0;}");

        ((QAscTabWidget*)m_pTabs)->FireFocus();
        m_pTabs->setCurrentIndex(-1);
    }
}

void QAscMainPanel::pushButtonMinimizeClicked()
{
    if (m_pMainButtonsController)
        m_pMainButtonsController->pushButtonMinimizeClicked();
}
void QAscMainPanel::pushButtonMaximizeClicked()
{
    if (m_pMainButtonsController)
        m_pMainButtonsController->pushButtonMaximizeClicked();
}
void QAscMainPanel::pushButtonCloseClicked()
{
    if (m_pMainButtonsController)
        m_pMainButtonsController->pushButtonCloseClicked();
}

void QAscMainPanel::slot_Download(NSEditorApi::CAscDownloadFileInfo* pInfo)
{
    if (pInfo->get_IsComplete())
    {
        // файл докачался, нужно удалить его из списка
        std::map<int, NSEditorApi::CAscDownloadFileInfo*>::iterator i = m_mapDownloads.find(pInfo->get_Id());

        if (i != m_mapDownloads.end())
        {
            RELEASEINTERFACE((i->second));
            m_mapDownloads.erase(i);
        }
    }
    else
    {
        std::map<int, NSEditorApi::CAscDownloadFileInfo*>::iterator i = m_mapDownloads.find(pInfo->get_Id());

        if (i != m_mapDownloads.end())
        {
            NSEditorApi::CAscDownloadFileInfo* pCurrent = i->second;
            pCurrent->put_Percent(pInfo->get_Percent());
            pCurrent->put_Speed(pInfo->get_Speed());
            pCurrent->put_IsComplete(pInfo->get_IsComplete());
        }
        else
        {
            if (!pInfo->get_FilePath().empty())
            {
                ADDREFINTERFACE(pInfo);
                m_mapDownloads.insert(std::pair<int, NSEditorApi::CAscDownloadFileInfo*>(pInfo->get_Id(), pInfo));
            }
        }
    }

    RELEASEINTERFACE(pInfo);

    // теперь смотрим, нужно ли показывать загрузки
    if (0 == m_mapDownloads.size())
    {
        m_pDownloadBackground->setHidden(true);
        return;
    }

    m_pDownloadBackground->setHidden(false);
    QString sText = "";

    for (std::map<int, NSEditorApi::CAscDownloadFileInfo*>::iterator i = m_mapDownloads.begin(); i != m_mapDownloads.end(); i++)
    {
        NSEditorApi::CAscDownloadFileInfo* pInfo = i->second;

        std::wstring sName = pInfo->get_FilePath();
        int n1 = sName.find_last_of((wchar_t)'\\');
        int n2 = sName.find_last_of((wchar_t)'/');

        int n = -1;
        if (n1 != std::wstring::npos)
            n = n1;
        if (n2 != std::wstring::npos && n2 > n1)
            n = n2;

        if (-1 == n)
            continue;

        sName = sName.substr(n + 1);

        sName += L" : %1; ";

        QString ss = QString::fromStdWString(sName);
        int nPercent = pInfo->get_Percent();
        if (nPercent < 0)
            nPercent = 0;

        sText += ss.arg(nPercent);
    }

    m_pDownloadLable->setText(sText);
}

void QAscMainPanel::slot_Modified(int id, bool value)
{
    bool bIsMain = !m_pMainWidget->isHidden();
    if (bIsMain)
        return;

    int nTabIndex = ((QAscTabWidget*)m_pTabs)->GetTabIndexById(id);
    if (nTabIndex == m_pTabs->tabBar()->currentIndex())
    {
        if (value == true)
            m_pSaveModifiedButton->setStyleSheet("background-color:#00FF00;border:none;margin:0;padding:0;");
        else
            m_pSaveModifiedButton->setStyleSheet("background-color:#FFFFFF;border:none;margin:0;padding:0;");
    }
}

void QAscMainPanel::slot_Print(int id, int pagesCount)
{
    QAscPrinterContext* pContext = new QAscPrinterContext();

    //pagesCount = 1;

    pContext->getPrinter()->setFromTo(1, pagesCount);

    QPrintDialog *dialog = new QPrintDialog(pContext->getPrinter(), NULL);
    dialog->setWindowTitle(tr("Print Document"));
    dialog->show();

    CCefView* pView = m_pManager->GetViewById(id);

    pContext->BeginPaint();
    for (int i = 0; i < pagesCount; ++i)
    {
        pContext->AddRef();

        NSEditorApi::CAscPrintPage* pData = new NSEditorApi::CAscPrintPage();
        pData->put_Context(pContext);
        pData->put_Page(i);

        NSEditorApi::CAscMenuEvent* pEvent = new NSEditorApi::CAscMenuEvent();
        pEvent->m_nType = ASC_MENU_EVENT_TYPE_CEF_PRINT_PAGE;
        pEvent->m_pData = pData;

        pView->Apply(pEvent);

        if (i != (pagesCount - 1))
            pContext->getPrinter()->newPage();
    }
    pContext->EndPaint();

    NSEditorApi::CAscMenuEvent* pEventEnd = new NSEditorApi::CAscMenuEvent();
    pEventEnd->m_nType = ASC_MENU_EVENT_TYPE_CEF_PRINT_END;

    pView->Apply(pEventEnd);

    pContext->Release();
}

void QAscMainPanel::slot_DialogSave()
{
    std::wstring sName = m_sDownloadName;

    QStringList arDocumentsPath = QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation);
    QString sDocumentsPath = (arDocumentsPath.size() == 0) ? QString() : arDocumentsPath[0];

    sDocumentsPath += QString::fromStdWString(L"/" + sName);
    QString sFileQT = QFileDialog::getSaveFileName(NULL, QString(), sDocumentsPath);
    std::wstring sPathQT = sFileQT.toStdWString();
    m_pManager->EndSaveDialog(sPathQT);
    m_sDownloadName = L"";
}

void QAscMainPanel::slot_CheckKeyboard()
{
    if (m_pManager)
        m_pManager->CheckKeyboard();
}

void QAscMainPanel::OpenDialogSave(std::wstring sName)
{
    m_sDownloadName = sName;
    emit signal_DialogSave();
}
void QAscMainPanel::CheckKeyboard()
{
    emit signal_CheckKeyboard();
}

void QAscMainPanel::resizeEvent(QResizeEvent* event)
{
    this->RecalculatePlaces();
}

void QAscMainPanel::OnEvent(NSEditorApi::CAscMenuEvent* pEvent)
{
    if (NULL == pEvent)
        return;

    switch (pEvent->m_nType)
    {
    case ASC_MENU_EVENT_TYPE_CEF_CREATETAB:
    {
        NSEditorApi::CAscCreateTab* pData = (NSEditorApi::CAscCreateTab*)pEvent->m_pData;
        QString sUrl = QString::fromStdWString(pData->get_Url());
        ((QAscTabWidget*)m_pTabs)->OpenPopup(sUrl);
        break;
    }
    case ASC_MENU_EVENT_TYPE_CEF_TABEDITORTYPE:
    {
        NSEditorApi::CAscTabEditorType* pData = (NSEditorApi::CAscTabEditorType*)pEvent->m_pData;
        CCefView* pView = m_pManager->GetViewById(pData->get_Id());
        if (NULL != pView && pView->GetType() == cvwtEditor)
        {
            CCefViewEditor* pEditor = (CCefViewEditor*)pView;
            pEditor->SetEditorType((AscEditorType)pData->get_Type());
        }
        ((QAscTabWidget*)m_pTabs)->UpdateIcons();
        break;
    }
    case ASC_MENU_EVENT_TYPE_CEF_ONCLOSE:
    {
        NSEditorApi::CAscTypeId* pId = (NSEditorApi::CAscTypeId*)pEvent->m_pData;
        int nId = pId->get_Id();

        int nIndex = ((QAscTabWidget*)m_pTabs)->GetTabIndexById(nId);

        m_pManager->DestroyCefView(nId);

        if (-1 != nIndex)
            m_pTabs->removeTab(nIndex);

        break;
    }
    case ASC_MENU_EVENT_TYPE_CEF_DOWNLOAD_START:
    {
        NSEditorApi::CAscDownloadFileInfo* pData = (NSEditorApi::CAscDownloadFileInfo*)pEvent->m_pData;
        QString sUrl = QString::fromStdWString(pData->get_Url());
        ((QAscTabWidget*)m_pTabs)->OpenDownload(sUrl, pData->get_Id());
        break;
    }
    case ASC_MENU_EVENT_TYPE_CEF_DOWNLOAD:
    {        
        NSEditorApi::CAscDownloadFileInfo* pData = (NSEditorApi::CAscDownloadFileInfo*)pEvent->m_pData;

        if (pData->get_IsComplete())
        {
            ((QAscTabWidget*)m_pTabs)->EndDownload(pData->get_Id());
        }

        ADDREFINTERFACE(pData);
        emit signal_Download(pData);

        break;
    }
    case ASC_MENU_EVENT_TYPE_CEF_DOCUMENT_NAME:
    {
        NSEditorApi::CAscDocumentName* pData = (NSEditorApi::CAscDocumentName*)pEvent->m_pData;

        QString sUrl = QString::fromStdWString(pData->get_Name());
        ((QAscTabWidget*)m_pTabs)->onSendNameAsync(sUrl, pData->get_Id());

        break;
    }
    case ASC_MENU_EVENT_TYPE_CEF_MODIFY_CHANGED:
    {
        NSEditorApi::CAscDocumentModifyChanged* pData = (NSEditorApi::CAscDocumentModifyChanged*)pEvent->m_pData;
        emit signal_Modified(pData->get_Id(), pData->get_Changed());

        break;
    }
    case ASC_MENU_EVENT_TYPE_CEF_ONSAVE:
    {
        //OutputDebugStringA("onsave");

        break;
    }
    case ASC_MENU_EVENT_TYPE_CEF_ONBEFORE_PRINT_END:
    {
        NSEditorApi::CAscPrintEnd* pData = (NSEditorApi::CAscPrintEnd*)pEvent->m_pData;
        emit signal_Print(pData->get_Id(), pData->get_PagesCount());
        break;
    }
    case ASC_MENU_EVENT_TYPE_CEF_ONKEYBOARDDOWN:
    {
        NSEditorApi::CAscKeyboardDown* pData = (NSEditorApi::CAscKeyboardDown*)pEvent->m_pData;

        QString sFormat = QString("code: %1, ctrl: %2, shift: %3, alt: %4").arg(
                    QString::number(pData->get_KeyCode()),
                    QString::number((int)pData->get_IsCtrl()),
                    QString::number((int)pData->get_IsShift()),
                    QString::number((int)pData->get_IsAlt()));

#if 0
        if (pData->get_KeyCode() == 32 && pData->get_IsAlt() == true)
        {
            RECT winrect;

            HWND windowHandle = (HWND)this->parentWidget()->winId();
            GetWindowRect( windowHandle, &winrect );
            TrackPopupMenu( GetSystemMenu( windowHandle, false ), TPM_TOPALIGN | TPM_LEFTALIGN, winrect.left + 5, winrect.top + 5, 0, windowHandle, NULL);
        }
#endif
        break;
    }
    }

    RELEASEINTERFACE(pEvent);
}
