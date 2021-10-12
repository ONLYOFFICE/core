#include "CTextEditDelegate.h"
#include "CXMLHighlighter.h"
#include "CItemWidget.h"

#include <QAbstractTextDocumentLayout>
#include <QApplication>
#include <QPushButton>
#include <QHBoxLayout>
#include <QTextEdit>
#include <QPainter>

#include <QComboBox>

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
        QString qsValue;

        if (index.data(1) == true)
                qsValue = "<Buffer>...<Buffer>";
        else
                qsValue = index.data(0).toString();

        if (index.data(5) == true)
        {
                QRect oRect(option.rect);

                oRect.setTop(oRect.top() + 3);
                oRect.setBottom(oRect.bottom() - 3);

                QPen oOldPen = painter->pen();

                QPen oNewPen = oOldPen;
                oNewPen.setStyle(Qt::DashLine);

                painter->setPen(oNewPen);

                painter->drawRect(oRect);

                painter->setPen(oOldPen);
        }

        if (bLightMode)
                painter->drawText(option.rect, qsValue);
        else
        {
                QTextDocument oEmptyDocument;
                CXMLHighlighter oXMLHighlighter(&oEmptyDocument);
                oXMLHighlighter.PaintByRegex(qsValue, painter, option.rect, pFont);
        }
}

void CTextEditDelegate::ClearFont()
{
        if (NULL != pFont)
                delete pFont;
}
