#include "CTextEditDelegate.h"
#include "CXMLHighlighter.h"
#include "CItemWidget.h"

#include <QAbstractTextDocumentLayout>
#include <QApplication>
#include <QPushButton>
#include <QHBoxLayout>
#include <QTextEdit>
#include <QPainter>

CTextEditDelegate::CTextEditDelegate(QWidget *parent) :
    QStyledItemDelegate(parent),
    pFont(NULL)
{
    bLightMode = false;
}

CTextEditDelegate::~CTextEditDelegate()
{
    ClearFont();
}

void CTextEditDelegate::SetFont(QFont *pFont)
{
    ClearFont();
    this->pFont = pFont;
}

void CTextEditDelegate::SetMode(bool bIsLightMode)
{
    bLightMode = bIsLightMode;
}

QWidget *CTextEditDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    CItemWidget *pItemWidget = new CItemWidget(parent);

    if (NULL != pFont)
        pItemWidget->SetFont(*pFont);

    return pItemWidget;
}

void CTextEditDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QString qsValue = index.model()->data(index, Qt::EditRole).toString();
    CItemWidget* pItemWidget = static_cast<CItemWidget*>(editor);
    pItemWidget->SetText(qsValue);
}

void CTextEditDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    CItemWidget* pItemWidget = static_cast<CItemWidget*>(editor);
    model->setData(index, pItemWidget->GetText(), Qt::EditRole);
}

void CTextEditDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    editor->setGeometry(option.rect);
}

void CTextEditDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if (bLightMode)
        painter->drawText(option.rect, index.data().toString());
    else
    {
        QTextDocument oEmptyDocument;
        CXMLHighlighter oXMLHighlighter(&oEmptyDocument);
        oXMLHighlighter.PaintByRegex(index.data().toString(), painter, option.rect, pFont);
    }
}

void CTextEditDelegate::ClearFont()
{
    if (NULL != pFont)
        delete pFont;
}
