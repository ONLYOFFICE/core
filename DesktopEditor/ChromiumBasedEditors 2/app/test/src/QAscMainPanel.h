#ifndef QMAINPANEL_H
#define QMAINPANEL_H

#include <QMouseEvent>
#include <QResizeEvent>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QPushButton>
#include <QLabel>

#include "../../../lib/include/applicationmanager.h"

class CMainButtonsController
{
public:
    virtual void pushButtonMinimizeClicked() {}
    virtual void pushButtonMaximizeClicked() {}
    virtual void pushButtonCloseClicked() {}
};

class QAscMainPanel : public QWidget, public NSEditorApi::CAscMenuEventListener
{
    Q_OBJECT

signals:
    void signal_Download( NSEditorApi::CAscDownloadFileInfo* );
    void signal_Modified(int, bool);
    void signal_Print(int, int);
    void signal_DialogSave();
    void signal_CheckKeyboard();

public:
    QAscMainPanel( QWidget *parent, CAscApplicationManager* pManager, bool bIsCustomWindow );

public slots:
    void pushButtonSaveClicked();
    void pushButtonMainClicked();

    void pushButtonMinimizeClicked();
    void pushButtonMaximizeClicked();
    void pushButtonCloseClicked();

    void slot_Download( NSEditorApi::CAscDownloadFileInfo* );
    void slot_Modified(int, bool);
    void slot_Print(int, int);
    void slot_DialogSave();
    void slot_CheckKeyboard();

public:
    WId GetHwndForKeyboard()
    {
        return ((QWidget*)m_pTabs->parent())->winId();
    }
    void SetMainButtonsController(CMainButtonsController* pController)
    {
        m_pMainButtonsController = pController;
    }

public:
    void resizeEvent(QResizeEvent* event);
    virtual void OnEvent(NSEditorApi::CAscMenuEvent* pEvent);
    virtual bool IsSupportEvent(int nEventType) { return true; }

    void OpenDialogSave(std::wstring sName);
    void CheckKeyboard();

    CAscApplicationManager* GetAppManager() { return m_pManager; }

protected:
    void RecalculatePlaces();

protected:
    QPushButton*    m_pButtonMain;
    QWidget*        m_pMainWidget;

    QPushButton*    m_pButtonMinimize;
    QPushButton*    m_pButtonMaximize;
    QPushButton*    m_pButtonClose;

    QPushButton*    m_pSaveModifiedButton;

    QTabWidget*     m_pTabs;

    QWidget*        m_pSeparator;

    std::wstring    m_sDownloadName;

    CAscApplicationManager* m_pManager;

    std::map<int, NSEditorApi::CAscDownloadFileInfo*> m_mapDownloads;
    QWidget*        m_pDownloadBackground;
    QLabel*         m_pDownloadLable;

    bool            m_bIsCustomWindow;

    CMainButtonsController* m_pMainButtonsController;
};

#endif // QMAINPANEL_H
