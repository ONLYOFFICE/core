#ifndef ASCEDITORWIDGET
#define ASCEDITORWIDGET

#include <QWidget>
#include <QTabWidget>
#include <QTabBar>
#include <QStyleOption>
#include <QPainter>
#include <QApplication>

namespace NSAscEditor
{
    enum EditorType
    {
        etDocument      = 0,
        etPresentation  = 1,
        etSpreadsheet   = 2
    };
}

#if 0
class CAscEditorWidget : public QWidget
{
    Q_OBJECT

public:
    NSAscEditor::EditorType m_etType;

public:
    CAscEditorWidget(QWidget *parent = 0, NSAscEditor::EditorType etType = NSAscEditor::etDocument) : QWidget(parent)
    {
        m_etType = etType;
        setStyleSheet("background-color:#FF00FF");
    }

    ~CAscEditorWidget()
    {
    }

    void SetIcon(int nIndex, QTabWidget* pWidget)
    {
        int nSelectedIndex = pWidget->tabBar()->currentIndex();

        QTabBar* pTabBar = pWidget->tabBar();

        bool bIsActive = (nIndex == nSelectedIndex) ? true : false;
        switch (m_etType)
        {
            case NSAscEditor::etPresentation:
            {
                pTabBar->setTabIcon(nIndex, bIsActive ? QIcon(":/Icons/tabicon_PE_active.png") : QIcon(":/Icons/tabicon_PE_normal.png"));
                break;
            }
            case NSAscEditor::etSpreadsheet:
            {
                pTabBar->setTabIcon(nIndex, bIsActive ? QIcon(":/Icons/tabicon_SE_active.png") : QIcon(":/Icons/tabicon_SE_normal.png"));
                break;
            }
            case NSAscEditor::etDocument:
            default:
            {
                pTabBar->setTabIcon(nIndex, bIsActive ? QIcon(":/Icons/tabicon_DE_active.png") : QIcon(":/Icons/tabicon_DE_normal.png"));
                break;
            }
        }

        pTabBar->setTabTextColor(nIndex, bIsActive ? QColor(255, 255, 255) : QColor(51, 51, 51));
    }

protected:
    void paintEvent(QPaintEvent* e)
    {
        Q_UNUSED(e);

        QStyleOption opt;
        opt.init(this);
        QPainter p(this);
        style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
    }
};
#else
#include "./cef/qcefwebview.h"
class CAscEditorWidget : public QCefWebView
{
    Q_OBJECT

public:
    NSAscEditor::EditorType m_etType;

public:
    CAscEditorWidget(QWidget *parent = 0, NSAscEditor::EditorType etType = NSAscEditor::etDocument, std::wstring strUrl = L"") : QCefWebView(parent)
    {
        this->setParentTabs((QTabWidget*)parent);
        m_etType = etType;

        std::wstring sUrl = QApplication::applicationDirPath().toStdWString();
        sUrl += L"/deploy/apps/api/documents/index.html";

        if (etType == NSAscEditor::etSpreadsheet)
            sUrl += L"?doctype=spreadsheet";
        if (etType == NSAscEditor::etPresentation)
            sUrl += L"?doctype=presentation";

        if (strUrl.length() != 0)
            sUrl = strUrl;

        this->load(sUrl);
    }

    virtual ~CAscEditorWidget()
    {
    }

    void SetIcon(int nIndex, QTabWidget* pWidget)
    {
        int nSelectedIndex = pWidget->tabBar()->currentIndex();

        QTabBar* pTabBar = pWidget->tabBar();

        bool bIsActive = (nIndex == nSelectedIndex) ? true : false;

        switch (m_etType)
        {
            case NSAscEditor::etPresentation:
            {
                pTabBar->setTabIcon(nIndex, bIsActive ? QIcon(":/Icons/tabicon_PE_active.png") : QIcon(":/Icons/tabicon_PE_normal.png"));
                break;
            }
            case NSAscEditor::etSpreadsheet:
            {
                pTabBar->setTabIcon(nIndex, bIsActive ? QIcon(":/Icons/tabicon_SE_active.png") : QIcon(":/Icons/tabicon_SE_normal.png"));
                break;
            }
            case NSAscEditor::etDocument:
            default:
            {
                pTabBar->setTabIcon(nIndex, bIsActive ? QIcon(":/Icons/tabicon_DE_active.png") : QIcon(":/Icons/tabicon_DE_normal.png"));
                break;
            }
        }

        //pTabBar->setTabTextColor(nIndex, bIsActive ? QColor(255, 255, 255) : QColor(51, 51, 51));
        pTabBar->setTabTextColor(nIndex, QColor(51, 51, 51));
    }
};
#endif

#endif // ASCEDITORWIDGET

