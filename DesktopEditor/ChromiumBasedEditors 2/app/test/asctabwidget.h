#ifndef ASCTABWIDGET
#define ASCTABWIDGET

#include <QResizeEvent>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTabBar>
#include <QtWidgets/QPushButton>
#include <QStylePainter>
#include <QStyleOption>

#include "../../lib/include/applicationmanager.h"
#include "../../lib/qcefview/qcefview.h"

class QPushButtonWrapper : public QPushButton
{
    Q_OBJECT

public:
    QCefView* m_pView;

public:
    QPushButtonWrapper(QWidget *parent = 0) : QPushButton(parent)
    {
        m_pView = NULL;
    }

public:

    virtual void paintEvent(QPaintEvent *)
     {
         QStyleOption opt;
         opt.init(this);
         QPainter p(this);
         style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
     }
};

class CAscTabWidget : public QTabWidget
{
    Q_OBJECT

public:
    QWidget* m_pMainWidget;
    QPushButton* m_pMainButton;
    CAscApplicationManager* m_pManager;

    std::map<int, QCefView*> m_mapDownloads;

    int m_nAreaWidth;
    int m_nAreaHeight;

signals:
    void sendPopup( QString );
    void sendDownload( QString, int );
    void sendEndDownload( int );
    void sendName( QString, int );

public:
    CAscTabWidget(QWidget *parent = 0) : QTabWidget(parent)
    {
        m_nAreaWidth = -1;
        m_nAreaHeight = -1;

        QObject::connect(this, SIGNAL( sendPopup( QString ) ), this, SLOT( openPopup(QString) ), Qt::QueuedConnection );
        QObject::connect(this, SIGNAL( sendDownload( QString , int) ), this, SLOT( openDownload(QString, int) ), Qt::QueuedConnection );
        QObject::connect(this, SIGNAL( sendEndDownload( int ) ), this, SLOT( endDownload(int) ), Qt::QueuedConnection );
        QObject::connect(this, SIGNAL( sendName(QString,int)), this, SLOT( onSendName(QString, int) ), Qt::QueuedConnection );

        this->tabBar()->setMovable(true);

        //QString strPaneColor = "#FFFFFF";
        QString strPaneColor = "#313437";

        QString strStyleQSS = "";
        //strStyleQSS += "QTabWidget QWidget { background-color:#FFFFFF;border:none;padding:0;margin:0; }";
        strStyleQSS += ("QTabWidget::pane { background-color:" + strPaneColor + "; border:none; }");
        strStyleQSS += "QTabWidget::tab-bar { height: 29px; left: 111px; }";

        strStyleQSS += "QTabBar::tab { \
                background-color: #CFCFCF; border: 1px solid #CFCFCF; border-radius: 1px;   \
                min-width: 41px; max-width: 135px;                                          \
                width:135px; height: 27px;                                                  \
                margin-left: 0px; margin-right: 1px; margin-top: 0px; margin-bottom: 0px; }";

        strStyleQSS += "QTabBar::tab:hover { \
                background-color: #DDDDDD; border: 1px solid #DDDDDD; border-radius: 1px; }";

        strStyleQSS += "QTabBar::tab:selected { \
                background-color: #F1F1F1; border: 1px solid #F1F1F1; border-radius: 1px;}";

        strStyleQSS += "QTabBar::tab:first { margin-left: 0px; }";
        strStyleQSS += "QTabBar::tab:last { }";

        strStyleQSS += "QTabBar::tear { width: 0px; }";

        strStyleQSS += "QTabBar QToolButton::left-arrow {margin:-2;background-color:#FFFFFF;border: 1px solid #FFFFFF; }";

        strStyleQSS += "QTabBar QToolButton::left-arrow { image: url(:/Icons/changetab_left_normal.png) }";
        strStyleQSS += "QTabBar QToolButton::left-arrow:hover { image: url(:/Icons/changetab_left_hover.png) }";
        strStyleQSS += "QTabBar QToolButton::left-arrow:disabled { image: url(:/Icons/changetab_left_disabled.png) }";
        strStyleQSS += "QTabBar QToolButton::left-arrow:pressed { image: url(:/Icons/changetab_left_active.png) }";

        strStyleQSS += "QTabBar QToolButton::right-arrow {margin:-2;background-color:#FFFFFF;border: 1px solid #FFFFFF;}";

        strStyleQSS += "QTabBar QToolButton::right-arrow { image: url(:/Icons/changetab_right_normal.png) }";
        strStyleQSS += "QTabBar QToolButton::right-arrow:hover { image: url(:/Icons/changetab_right_hover.png) }";
        strStyleQSS += "QTabBar QToolButton::right-arrow:disabled { image: url(:/Icons/changetab_right_disabled.png) }";
        strStyleQSS += "QTabBar QToolButton::right-arrow:pressed { image: url(:/Icons/changetab_right_active.png) }";

        strStyleQSS += "QTabBar::scroller { padding: 0px; margin: 0px;background-color:#FFFFFF;border: 1px solid #FFFFFF; width:10; }";

        setStyleSheet(strStyleQSS);

        QSize iconSize;
        iconSize.setWidth(18);
        iconSize.setHeight(10);
        this->setIconSize(iconSize);

        connect(this, SIGNAL(currentChanged(int)), this, SLOT(currentChangedSlot(int)));
    }

    void addEditor(QString strName, AscEditorType etType = etDocument, std::wstring strUrl = L"")
    {
        if (!m_pManager)
            return;

        QCefView* pView = new QCefView(this);
        pView->SetBackgroundCefColor(244, 244, 244);
        pView->Create(m_pManager, cvwtEditor);

        this->addTab(pView, strName);
        this->resizeEvent(NULL);

        pView->GetCefView()->load(strUrl);

        QPushButtonWrapper *close = new QPushButtonWrapper(pView);
        close->m_pView = pView;

        QObject::connect(close, SIGNAL(clicked()), this, SLOT(clickedSlotClose()));

        QString sStyleClose = "QPushButton {background:transparent;background-repeat: no-repeat;background-image:url(:/Icons/output_tab_normal.png);border:none;width:8px;height:11px;margin-right:5px;} ";
        sStyleClose += "QPushButton:hover {background-image:url(:/Icons/output_tab_hover.png);border:none;width:8px;height:11px;margin-right:5px;}";
        sStyleClose += "QPushButton:pressed {background-image:url(:/Icons/output_tab_active.png);border:none;width:8px;height:11px;margin-right:5px;}";

        close->setStyleSheet(sStyleClose);                        
        this->tabBar()->setTabButton(this->tabBar()->count() - 1, QTabBar::RightSide, close);

        this->resizeEvent(NULL);
    }

protected:
    void resizeEvent(QResizeEvent* e)
    {
        Q_UNUSED(e);

        int nMin = 41;
        int nMax = 135;

        int nFirst = 44;
        int nStartOffset = 5;
        int nBetweenApp = 5;
        int nButtonW = 16;
        int nEndOffset = 40;

        int nControlWidth = this->width();
        int nTabBarWidth = nControlWidth - nFirst - nStartOffset - nEndOffset - 3 * nButtonW - 2 * nBetweenApp;

        int nCountTabs = this->tabBar()->count();
        if (nCountTabs == 0)
            nCountTabs = 1;

        int nTabWidth = (nTabBarWidth - (2 + 2) * nCountTabs) / nCountTabs;
        if (nTabWidth > nMax)
            nTabWidth = nMax;
        if (nTabWidth < nMin)
            nTabWidth = nMin;

        int nMinTabBarWidth = (nTabWidth + 2 + 2) * nCountTabs;
        if (nTabBarWidth > nMinTabBarWidth)
            nTabBarWidth = nMinTabBarWidth;

        //QString strPaneColor = "#FFFFFF";
        QString strPaneColor = "#313437";

        QString strStyleQSS = "";
        //strStyleQSS += "QTabWidget QWidget { background-color:#FFFFFF;border:none;padding:0;margin:0; }";
        strStyleQSS += ("QTabWidget::pane { background-color:" + strPaneColor + "; border: none; }");
        strStyleQSS += QString("QTabWidget::tab-bar { width:%1px; height: 29px; left: 111px; }").arg(nTabBarWidth);

        strStyleQSS += QString("QTabBar::tab { \
                background-color: #CFCFCF; border: 1px solid #CFCFCF; border-radius: 1px;   \
                min-width: 41px; max-width: 135px;                                          \
                width:%1px; height: 27px;                                                  \
                margin-right: 1px; margin-left: 0px; margin-top: 0px; margin-bottom: 0px; }").arg(nTabWidth);

        strStyleQSS += "QTabBar::tab:hover { \
                background-color: #DDDDDD; border: 1px solid #DDDDDD; border-radius: 1px; }";

        strStyleQSS += "QTabBar::tab:selected { \
                background-color: #F1F1F1; border: 1px solid #F1F1F1; border-radius: 1px;}";

        strStyleQSS += "QTabBar::tab:first { margin-left: 0px; }";
        strStyleQSS += "QTabBar::tab:last { }";

        strStyleQSS += "QTabBar::tear { width: 0px; }";

        strStyleQSS += "QTabBar QToolButton::left-arrow {margin:-2;background-color:#FFFFFF;border: 1px solid #FFFFFF; }";

        strStyleQSS += "QTabBar QToolButton::left-arrow { image: url(:/Icons/changetab_left_normal.png) }";
        strStyleQSS += "QTabBar QToolButton::left-arrow:hover { image: url(:/Icons/changetab_left_hover.png) }";
        strStyleQSS += "QTabBar QToolButton::left-arrow:disabled { image: url(:/Icons/changetab_left_disabled.png) }";
        strStyleQSS += "QTabBar QToolButton::left-arrow:pressed { image: url(:/Icons/changetab_left_active.png) }";

        strStyleQSS += "QTabBar QToolButton::right-arrow {margin:-2;background-color:#FFFFFF;border: 1px solid #FFFFFF;}";

        strStyleQSS += "QTabBar QToolButton::right-arrow { image: url(:/Icons/changetab_right_normal.png) }";
        strStyleQSS += "QTabBar QToolButton::right-arrow:hover { image: url(:/Icons/changetab_right_hover.png) }";
        strStyleQSS += "QTabBar QToolButton::right-arrow:disabled { image: url(:/Icons/changetab_right_disabled.png) }";
        strStyleQSS += "QTabBar QToolButton::right-arrow:pressed { image: url(:/Icons/changetab_right_active.png) }";

        strStyleQSS += "QTabBar::scroller { padding: 0px; margin: 0px;background-color:#FFFFFF;border: 1px solid #FFFFFF; width:10; }";

        strStyleQSS += "QTabBar QIcon { offset: 10px; }";
        strStyleQSS += "QTabBar::tab:text{ font:Arial;font-weight:normal;font-size:8pt;text-align:left;}";

        setStyleSheet(strStyleQSS);

        int nNewAreaWidth = width();
        int nNewAreaHeight = height() - this->tabBar()->height();

        if (nNewAreaWidth != m_nAreaWidth || nNewAreaHeight != m_nAreaHeight)
        {
            m_nAreaWidth = nNewAreaWidth;
            m_nAreaHeight = nNewAreaHeight;
            int nCountNatural = this->count();
            for (int i = 0; i < nCountNatural; ++i)
            {
                if (i == this->currentIndex())
                    continue;

                ((QCefView*)widget(i))->GetCefView()->resizeEvent(m_nAreaWidth, m_nAreaHeight);
            }
        }
    }

public:
    void OpenPopup(QString strUrl)
    {
        if (strUrl.length() == 0)
            return;
        emit sendPopup(strUrl);
    }

    void OpenDownload(QString strUrl, int nParentId)
    {
        if (strUrl.length() == 0)
            return;
        emit sendDownload(strUrl, nParentId);
    }

    void EndDownload(int nId)
    {
        emit sendEndDownload(nId);
    }

public slots:
    void currentChangedSlot(int index)
    {
        resizeEvent(NULL);
        if (!m_pMainWidget->isHidden())
        {
            m_pMainWidget->setHidden(true);
            m_pMainButton->setStyleSheet("QPushButton {background-image:url(:/Icons/Documents_disabled_normal.png);border:none;margin:0;padding:0;}\
        QPushButton::hover {background-image:url(:/Icons/Documents_disabled_normal.png);border:none;margin:0;padding:0;}\
        QPushButton::pressed {background-image:url(:/Icons/Documents_disabled_hover.png);border:none;margin:0;padding:0;}");
        }

        FireFocus(index);
        UpdateIcons();
    }

    void openPopup(QString sValue)
    {
        this->addEditor("Editor", etDocument, sValue.toStdWString());
    }

    void openDownload(QString sUrl, int nParentId)
    {
        std::wstring strUrl = sUrl.toStdWString();
        if (!m_pManager)
            return;

        QCefView* pView = new QCefView(NULL);
        pView->Create(m_pManager, cvwtEditor);
        pView->GetCefView()->SetParentCef(nParentId);
        pView->GetCefView()->load(strUrl);

        m_mapDownloads.insert(std::pair<int, QCefView*>(pView->GetCefView()->GetId(), pView));
    }
    void endDownload(int nId)
    {
        std::map<int, QCefView*>::iterator i = m_mapDownloads.find(nId);
        if (i != m_mapDownloads.end())
        {
            QCefView* pView = i->second;
            RELEASEOBJECT(pView);
            m_mapDownloads.erase(i);
        }
    }

    void onSendNameAsync(QString sUrl, int nId)
    {
        emit sendName(sUrl, nId);
    }
    void onSendName(QString sUrl, int nId)
    {
        int nIndex = this->GetTabIndexById(nId);
        this->tabBar()->setTabText(nIndex, sUrl);
    }

    void FireFocus(int index = -1)
    {
        int nIndex = (index >= 0) ? index : this->currentIndex();
        if (nIndex >= 0)
            ((QCefView*)this->widget(nIndex))->GetCefView()->focus();
    }

    void UpdateIcons()
    {
        QTabBar* pBar = this->tabBar();
        int nCount = pBar->count();

        int nSelectedIndex = pBar->currentIndex();
        for (int i = 0; i < nCount; ++i)
        {
            CCefViewEditor* pEditor = (CCefViewEditor*)(((QCefView*)(this->widget(i)))->GetCefView());

            bool bIsActive = (i == nSelectedIndex) ? true : false;

            switch (pEditor->GetEditorType())
            {
                case etPresentation:
                {
                    pBar->setTabIcon(i, bIsActive ? QIcon(":/Icons/tabicon_PE_active.png") : QIcon(":/Icons/tabicon_PE_normal.png"));
                    break;
                }
                case etSpreadsheet:
                {
                    pBar->setTabIcon(i, bIsActive ? QIcon(":/Icons/tabicon_SE_active.png") : QIcon(":/Icons/tabicon_SE_normal.png"));
                    break;
                }
                case etDocument:
                default:
                {
                    pBar->setTabIcon(i, bIsActive ? QIcon(":/Icons/tabicon_DE_active.png") : QIcon(":/Icons/tabicon_DE_normal.png"));
                    break;
                }
            }

            //pBar->setTabTextColor(nIndex, bIsActive ? QColor(255, 255, 255) : QColor(51, 51, 51));
            pBar->setTabTextColor(i, QColor(51, 51, 51));
        }
    }

public slots:
    void clickedSlotClose()
    {
        QPushButtonWrapper* pButton = (QPushButtonWrapper*)sender();
        QCefView* pViewCef = pButton->m_pView;
        int nId = pViewCef->GetCefView()->GetId();

#if 1
        std::string sId = std::to_string(nId);sId += "\n";
        OutputDebugStringA(sId.c_str());
#endif

        NSEditorApi::CAscMenuEvent* pEvent = new NSEditorApi::CAscMenuEvent();
        pEvent->m_nType = ASC_MENU_EVENT_TYPE_CEF_CLOSE;
        pViewCef->GetCefView()->Apply(pEvent);
    }

    int GetTabIndexById(int nId)
    {
        QTabBar* pBar = this->tabBar();
        int nCount = pBar->count();

        for (int i = 0; i < nCount; ++i)
        {
            CCefView* pEditor = (CCefView*)(((QCefView*)(this->widget(i)))->GetCefView());
            if (pEditor->GetId() == nId)
                return i;
        }
        return -1;
    }
};

#endif // ASCTABWIDGET

