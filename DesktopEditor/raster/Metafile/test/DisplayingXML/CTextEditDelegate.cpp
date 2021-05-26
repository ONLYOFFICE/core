#include "CTextEditDelegate.h"
#include "CXMLHighlighter.h"

#include <QAbstractTextDocumentLayout>
#include <QApplication>
#include <QTextEdit>
#include <QPainter>
#include <QDebug>

CTextEditDelegate::CTextEditDelegate(QWidget *parent) :
    QStyledItemDelegate(parent)
{
}

QWidget *CTextEditDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QTextEdit *pTextEditor = new QTextEdit(parent);
    pTextEditor->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    pTextEditor->document()->setDocumentMargin(0);

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
    //TODO: видимо нужно вручную парсить строку и разделять на блоки и затем отрисовывать со своим стилем
    painter->drawText(option.rect, index.data().toString());

//    QStyleOptionViewItem itemOption(option);

//    initStyleOption(&itemOption, index);
//    itemOption.text = qsValue;  // override text

//    QApplication::style()->drawControl(QStyle::CE_ItemViewItem, &itemOption, painter, nullptr);

}
