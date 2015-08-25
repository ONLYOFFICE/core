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
    void downloadEvent( NSEditorApi::CAscDownloadFileInfo* );
    void setModified(int, bool);
    void signalPrint(int, int);
    void dialogSave();

public:
    QAscMainPanel( QWidget *parent, CAscApplicationManager* pManager );

    void resizeEvent(QResizeEvent* event);

public slots:
    void pushButtonSaveClicked();
    void pushButtonMainClicked();
    void downloadEventSlot( NSEditorApi::CAscDownloadFileInfo* );
    void onSetModified(int, bool);
    void slotPrint(int, int);
    void onDialogSave();

private:
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

public:
    WId GetHwndForKeyboard()
    {
        return ((QWidget*)m_pTabs->parent())->winId();
    }

public:
    virtual void OnEvent(NSEditorApi::CAscMenuEvent* pEvent);
    virtual bool IsSupportEvent(int nEventType)
    {
        return true;
    }

    void sendDialogSave(std::wstring sName);

private:
    void RecalculatePlaces();
};

#endif // QMAINPANEL_H
