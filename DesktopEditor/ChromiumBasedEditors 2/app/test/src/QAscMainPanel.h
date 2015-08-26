#ifndef QMAINPANEL_H
#define QMAINPANEL_H

#include <QMouseEvent>
#include <QResizeEvent>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QPushButton>
#include <QLabel>

#include "../../../lib/include/applicationmanager.h"

class QAscMainPanel : public QWidget, public NSEditorApi::CAscMenuEventListener
{
    Q_OBJECT

signals:
    void signal_Download( NSEditorApi::CAscDownloadFileInfo* );
    void signal_Modified(int, bool);
    void signal_Print(int, int);
    void signal_DialogSave();

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

public:
    WId GetHwndForKeyboard()
    {
        return ((QWidget*)m_pTabs->parent())->winId();
    }

public:
    void resizeEvent(QResizeEvent* event);
    virtual void OnEvent(NSEditorApi::CAscMenuEvent* pEvent);
    virtual bool IsSupportEvent(int nEventType) { return true; }

    void OpenDialogSave(std::wstring sName);

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
};

#endif // QMAINPANEL_H
