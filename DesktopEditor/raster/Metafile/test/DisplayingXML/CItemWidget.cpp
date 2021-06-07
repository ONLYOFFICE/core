#include <QStandardItem>
#include <QHBoxLayout>
#include <QTreeView>

#include "CXMLHighlighter.h"
#include "CItemWidget.h"

CItemWidget::CItemWidget(QWidget *parent) :
    QWidget(parent),
    m_pTextEdit(new QTextEdit),
    m_pDeleteButton(new QPushButton)
{
    QHBoxLayout *pHBoxLayout = new QHBoxLayout(this);

    pHBoxLayout->setMargin(0);

    m_pTextEdit->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_pTextEdit->setReadOnly(true);

    m_pDeleteButton->setText("Delete");
    m_pDeleteButton->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Expanding);
    m_pDeleteButton->setStyleSheet("QPushButton {background-color:red;}");

    connect(m_pDeleteButton, SIGNAL(clicked()), this, SLOT(on_deleteButton_clicked()));

    pHBoxLayout->addWidget(m_pTextEdit);
    pHBoxLayout->addWidget(m_pDeleteButton);

    CXMLHighlighter *pXMLHighlighter = new CXMLHighlighter(m_pTextEdit->document());
}

CItemWidget::~CItemWidget()
{
    delete m_pTextEdit;
    delete m_pDeleteButton;
}

QString CItemWidget::GetText()
{
    return m_pTextEdit->toPlainText();
}

void CItemWidget::SetText(const QString &qsString)
{
    m_pTextEdit->setText(qsString);

    QTreeView *pTreeView = static_cast<QTreeView*>(parent()->parent());
    QModelIndex oModelIndex = pTreeView->indexAt(this->pos());
    if (oModelIndex.isValid())
    {
        QStandardItem *pStandardItem = static_cast<QStandardItem*>(oModelIndex.internalPointer());
        if (pStandardItem->child(oModelIndex.row())->rowCount() == 0)
            m_pTextEdit->setReadOnly(false);
    }
}

void CItemWidget::SetFont(const QFont &oFont)
{
    m_pTextEdit->setFont(oFont);
    m_pDeleteButton->setStyleSheet(QString("QPushButton {font: bold %1px; background-color:red;}").arg(oFont.pointSize()));
}

void CItemWidget::on_deleteButton_clicked()
{
    QTreeView *pTreeView = static_cast<QTreeView*>(parent()->parent());
    QModelIndex oModelIndex = pTreeView->indexAt(this->pos());
    if (oModelIndex.isValid())
    {
        QStandardItem *pStandardItem = static_cast<QStandardItem*>(oModelIndex.internalPointer());
        pStandardItem->removeRow(oModelIndex.row());

        QStandardItem *pStandardItemParent = pStandardItem->parent();
        if (pStandardItem->rowCount() == 0)
            pStandardItemParent->removeRow(pStandardItem->index().row());
    }
}
