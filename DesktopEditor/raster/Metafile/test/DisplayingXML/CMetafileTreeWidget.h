#ifndef CMETAFILETREEWIDGET_H
#define CMETAFILETREEWIDGET_H

#include <QFrame>
#include <QObject>
#include <QWidget>
#include <QTextEdit>
#include <QPushButton>

#include "CTextEditDelegate.h"
#include "CMetafileTreeView.h"
#include "MainWindow.h"

class CMetafileTreeWidget : public QWidget
{
        Q_OBJECT
    public:
        CMetafileTreeWidget(QWidget *parent = NULL);
        ~CMetafileTreeWidget();

        void Clear();
        void SetMainWindow(MainWindow *pMainWindow);

        //Методы QTreeView
        void setItemDelegate(CTextEditDelegate *pTextEditDelegate);
        void expandAll();
        void collapseAll();
        QAbstractItemModel* model();
        void setFrameShadow(QFrame::Shadow enShadow);
        void setSelectionBehavior(QAbstractItemView::SelectionBehavior unBehavior);
        void setAutoExpandDelay(int nDelay);
        void setRootIsDecorated(bool bShow);
        void setAnimated(bool bEnable);
        QHeaderView* header();

        //Методы CMetafileTreeView
        void SetMetafile(const std::wstring& wsXmlFilePath);
        bool IsClearTree();
        void SetMode(bool bLightMode);
        void ClearTree();
        QMap<QString, unsigned int> GetStatistics();

        bool SaveInXmlFile(const std::wstring& wsSaveFilePath);

        void EditItem(QStandardItem* pStandardItem);

    protected:
        virtual void mouseReleaseEvent(QMouseEvent* event) override;

    private slots:
        void slotFindNext();
        void slotFindPrev();
        void slotRBClickedOnMetafileTree(QPoint oPoint);
        void slotDeleteItem(QStandardItem *pDeletedItem);

    private:
        void InsertRecord(QStandardItem *pParentItem, unsigned int unRow, bool bAfterRecord = true);

        QTextEdit *m_pFindText;
        QPushButton *m_pFindNextButton;
        QPushButton *m_pFindPrevButton;

        CMetafileTreeView *m_pMetafileTreeView;
        int m_nIndexSelectedItem;
        QStandardItem *m_pSelectedItem;

        MainWindow *m_pMainWindow;
};

#endif // CMETAFILETREEWIDGET_H
