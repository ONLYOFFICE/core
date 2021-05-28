#include "CTextEditDelegate.h"
#include "CXMLHighlighter.h"

#include <QAbstractTextDocumentLayout>
#include <QApplication>
#include <QTextEdit>
#include <QPainter>
#include <QDebug>

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
    QTextEdit *pTextEditor = new QTextEdit(parent);
    pTextEditor->setReadOnly(true);

    pTextEditor->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    pTextEditor->document()->setDocumentMargin(0);

    if (NULL != pFont)
        pTextEditor->setFont(*pFont);

    CXMLHighlighter *pXMLHighlighter = new CXMLHighlighter(pTextEditor->document());
    return pTextEditor;
}

void CTextEditDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QString qsValue = index.model()->data(index, Qt::EditRole).toString();

    QTextEdit* pTextEditor = static_cast<QTextEdit*>(editor);
    pTextEditor->setText(qsValue);
}

void CTextEditDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QTextEdit* pTextEditor = static_cast<QTextEdit*>(editor);
    QString qsValue = pTextEditor->toPlainText();
    model->setData(index, qsValue, Qt::EditRole);
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
