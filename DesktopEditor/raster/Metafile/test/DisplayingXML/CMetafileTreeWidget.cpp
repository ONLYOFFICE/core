#include "CMetafileTreeWidget.h"

#include <QMenu>
#include <QBoxLayout>

#include "CRecordCreator.h"
#include "Common/Common.h"

CMetafileTreeWidget::CMetafileTreeWidget(QWidget *parent)
        : QWidget(parent),
          m_pFindText(NULL),
          m_pFindNextButton(NULL),
          m_pFindPrevButton(NULL),
          m_pMetafileTreeView(NULL),
          m_nIndexSelectedItem(0),
          m_pSelectedItem(NULL)
{
        QVBoxLayout *pLayout = new QVBoxLayout(this);
        QHBoxLayout *pFindLayout = new QHBoxLayout();

        m_pFindText = new QTextEdit();
        AlignTextEdit(m_pFindText);

        m_pFindNextButton = new QPushButton("Find next");
        m_pFindPrevButton = new QPushButton("Find prev");

        connect(m_pFindNextButton, &QPushButton::clicked, this, &CMetafileTreeWidget::slotFindNext);
        connect(m_pFindPrevButton, &QPushButton::clicked, this, &CMetafileTreeWidget::slotFindPrev);

        QFont oFont = m_pFindText->font();
        oFont.setPointSize(oFont.pointSize());

        m_pFindNextButton->setFont(oFont);
        m_pFindPrevButton->setFont(oFont);

        pFindLayout->addWidget(m_pFindText);
        pFindLayout->addWidget(m_pFindNextButton);
        pFindLayout->addWidget(m_pFindPrevButton);

        pLayout->addLayout(pFindLayout);

        m_pMetafileTreeView = new CMetafileTreeView();

        connect(m_pMetafileTreeView, &CMetafileTreeView::clickedRightMouseButton, this, &CMetafileTreeWidget::slotRBClickedOnMetafileTree);
        connect(m_pMetafileTreeView, &CMetafileTreeView::signalDeleteItem, this,  &CMetafileTreeWidget::slotDeleteItem);

        pLayout->addWidget(m_pMetafileTreeView);
}

CMetafileTreeWidget::~CMetafileTreeWidget()
{
        if (NULL != m_pFindText)
                delete m_pFindText;
        if (NULL != m_pFindNextButton)
                delete m_pFindNextButton;
        if (NULL != m_pFindPrevButton)
                delete m_pFindPrevButton;
        if (NULL != m_pMetafileTreeView)
                delete m_pMetafileTreeView;

        m_pSelectedItem = NULL;
}

void CMetafileTreeWidget::Clear()
{
        ClearTree();

        if (NULL != m_pFindText)
                m_pFindText->clear();

        m_nIndexSelectedItem = 0;
        m_pSelectedItem = NULL;
}

void CMetafileTreeWidget::SetMainWindow(MainWindow *pMainWindow)
{
        m_pMainWindow = pMainWindow;
}

void CMetafileTreeWidget::setItemDelegate(CTextEditDelegate *pTextEditDelegate)
{
        if (NULL != m_pMetafileTreeView)
                m_pMetafileTreeView->setItemDelegate(pTextEditDelegate);
}

void CMetafileTreeWidget::expandAll()
{
        if (NULL != m_pMetafileTreeView)
                m_pMetafileTreeView->expandAll();
}

void CMetafileTreeWidget::collapseAll()
{
        if (NULL != m_pMetafileTreeView)
                m_pMetafileTreeView->collapseAll();
}

QAbstractItemModel* CMetafileTreeWidget::model()
{
        if (NULL != m_pMetafileTreeView)
                return m_pMetafileTreeView->model();

        return NULL;
}

void CMetafileTreeWidget::setFrameShadow(QFrame::Shadow enShadow)
{
        if (NULL != m_pMetafileTreeView)
                m_pMetafileTreeView->setFrameShadow(enShadow);
}

void CMetafileTreeWidget::setSelectionBehavior(QAbstractItemView::SelectionBehavior unBehavior)
{
        if (NULL != m_pMetafileTreeView)
                m_pMetafileTreeView->setSelectionBehavior(unBehavior);
}

void CMetafileTreeWidget::setAutoExpandDelay(int nDelay)
{
        if (NULL != m_pMetafileTreeView)
                m_pMetafileTreeView->setAutoExpandDelay(nDelay);
}

void CMetafileTreeWidget::setRootIsDecorated(bool bShow)
{
        if (NULL != m_pMetafileTreeView)
                m_pMetafileTreeView->setRootIsDecorated(bShow);
}

void CMetafileTreeWidget::setAnimated(bool bEnable)
{
        if (NULL != m_pMetafileTreeView)
                m_pMetafileTreeView->setAnimated(bEnable);
}

QHeaderView* CMetafileTreeWidget::header()
{
        if (NULL != m_pMetafileTreeView)
                return m_pMetafileTreeView->header();

        return NULL;
}

void CMetafileTreeWidget::SetMetafile(const std::wstring &wsXmlFilePath)
{
        if (NULL != m_pMetafileTreeView)
                m_pMetafileTreeView->SetMetafile(wsXmlFilePath);
}

bool CMetafileTreeWidget::IsClearTree()
{
        if (NULL != m_pMetafileTreeView)
                return m_pMetafileTreeView->IsClear();

        return false;
}

void CMetafileTreeWidget::SetMode(bool bLightMode)
{
        if (NULL != m_pMetafileTreeView)
                m_pMetafileTreeView->SetMode(bLightMode);
}

void CMetafileTreeWidget::ClearTree()
{
        if (NULL != m_pMetafileTreeView)
                m_pMetafileTreeView->Clear();
}

QMap<QString, unsigned int> CMetafileTreeWidget::GetStatistics()
{
        if (NULL != m_pMetafileTreeView)
        {
                QMap<QString, unsigned int> mStatictics;
                unsigned int unIndexRecord = 0;

                QModelIndex oMainIndex = m_pMetafileTreeView->model()->index(0, 0);
                QModelIndex oIndex = oMainIndex.child(unIndexRecord++, 0);

                while (oIndex.isValid())
                {
                        QString qsNameRecord = oIndex.data(2).toString();
                        qsNameRecord = qsNameRecord.mid(1, qsNameRecord.length() - 2);
                        ++mStatictics[qsNameRecord];
                        oIndex = oMainIndex.child(unIndexRecord++, 0);
                }

                return mStatictics;
        }

        return {};
}

bool CMetafileTreeWidget::SaveInXmlFile(const std::wstring &wsSaveFilePath)
{
        if (NULL != m_pMetafileTreeView)
                return m_pMetafileTreeView->SaveInXmlFile(wsSaveFilePath);

        return false;
}

void CMetafileTreeWidget::EditItem(QStandardItem *pStandardItem)
{
        if (NULL != m_pMetafileTreeView)
                m_pMetafileTreeView->EditItem(pStandardItem);
}

void CMetafileTreeWidget::mouseReleaseEvent(QMouseEvent *event)
{
        m_pSelectedItem = NULL;
        QWidget::mouseReleaseEvent(event);
}

void CMetafileTreeWidget::slotFindNext()
{
        if (NULL == m_pFindText || NULL == m_pMetafileTreeView || IsClearTree())
                return;

        QString qsFindText = m_pFindText->toPlainText().toLower();

        if (NULL != m_pSelectedItem)
                m_pSelectedItem->setData(false, 5);

        if (qsFindText.isEmpty())
                return;

        QModelIndex oMainIndex = m_pMetafileTreeView->model()->index(0, 0);
        QModelIndex oIndex = oMainIndex.child(m_nIndexSelectedItem++, 0);

        while (oIndex.isValid())
        {
                QString qsTextItem = oIndex.data(2).toString().toLower();
                if (qsTextItem.contains(qsFindText))
                {
                        QStandardItem *pStandardItem = static_cast<QStandardItem*>(oIndex.internalPointer());
                        QStandardItem *pItem = pStandardItem->child(oIndex.row(), oIndex.column());

                        m_pMetafileTreeView->scrollTo(oIndex);

                        m_pSelectedItem = pItem;
                        pItem->setData(true, 5);
                        m_pMetafileTreeView->update();
                        return;
                }
                oIndex = oMainIndex.child(m_nIndexSelectedItem++, 0);
        }

        m_nIndexSelectedItem = 0;
}

void CMetafileTreeWidget::slotFindPrev()
{
        if (NULL == m_pFindText || NULL == m_pMetafileTreeView || m_pFindText->toPlainText().isEmpty() || IsClearTree() || m_nIndexSelectedItem == 0)
                return;

        if (NULL == m_pFindText || NULL == m_pMetafileTreeView || IsClearTree())
                return;

        QString qsFindText = m_pFindText->toPlainText().toLower();

        if (NULL != m_pSelectedItem)
                m_pSelectedItem->setData(false, 5);

        if (qsFindText.isEmpty())
                return;

        QModelIndex oMainIndex = m_pMetafileTreeView->model()->index(0, 0);
        QModelIndex oIndex = oMainIndex.child(--m_nIndexSelectedItem - 1, 0);

        while (oIndex.isValid())
        {
                QString qsTextItem = oIndex.data(2).toString().toLower();
                if (qsTextItem.contains(qsFindText))
                {
                        QStandardItem *pStandardItem = static_cast<QStandardItem*>(oIndex.internalPointer());
                        QStandardItem *pItem = pStandardItem->child(oIndex.row(), oIndex.column());

                        m_pMetafileTreeView->scrollTo(oIndex);

                        m_pSelectedItem = pItem;
                        pItem->setData(true, 5);
                        m_pMetafileTreeView->update();
                        return;
                }
                oIndex = oMainIndex.child(--m_nIndexSelectedItem - 1, 0);
        }

        m_nIndexSelectedItem = 0;
}

void CMetafileTreeWidget::slotRBClickedOnMetafileTree(QPoint oPoint)
{
        if (NULL == model())
                return;

        QModelIndex oModelIndex = m_pMetafileTreeView->indexAt(oPoint);

        if (!oModelIndex.isValid())
                return;

        QStandardItem *pStandardItem = static_cast<QStandardItem*>(oModelIndex.internalPointer());
        QStandardItem *pItem = pStandardItem->child(oModelIndex.row(), oModelIndex.column());

        QMenu oContextMenu;

        oContextMenu.addAction("Edit", this, [this, pItem](){m_pMetafileTreeView->EditItem(pItem);});

        if (true == pItem->data(3))
        {
                oContextMenu.addAction("Insert before", this, [this, pStandardItem, oModelIndex](){InsertRecord(pStandardItem, oModelIndex.row());});
                oContextMenu.addAction("Insert after", this,  [this, pStandardItem, oModelIndex](){InsertRecord(pStandardItem, oModelIndex.row(), false);});
        }

        oContextMenu.exec(m_pMetafileTreeView->mapToGlobal(oPoint));
}

void CMetafileTreeWidget::slotDeleteItem(QStandardItem *pDeletedItem)
{
        if (pDeletedItem == m_pSelectedItem)
        {
                m_pSelectedItem = NULL;
                m_nIndexSelectedItem = 0;
        }
        else
                --m_nIndexSelectedItem;
}

void CMetafileTreeWidget::InsertRecord(QStandardItem *pParentItem, unsigned int unRow, bool bAfterRecord)
{
        if (NULL == pParentItem)
                return;

        CRecordCreator *pRecordCreator = new CRecordCreator();
        pRecordCreator->SetMainWindow(m_pMainWindow);

        QStandardItem *pItem = pRecordCreator->CreateRecord();

        if (NULL != pItem)
        {
                pParentItem->insertRow(unRow + ((bAfterRecord) ? 0 : 1), pItem);
                if (m_pMainWindow->SaveInXmlFile(L"Temp.xml") &&
                    m_pMainWindow->ConvertToEmf(L"Temp.xml"))
                        m_pMainWindow->DisplayingFile(L"TempFile.emf", false);
        }
}
