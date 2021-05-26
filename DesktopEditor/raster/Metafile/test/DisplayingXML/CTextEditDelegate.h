#ifndef CTEXTEDITDELEGATE_H
#define CTEXTEDITDELEGATE_H

#include <QStyledItemDelegate>

class CTextEditDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit CTextEditDelegate(QWidget *parent = 0);

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
};

#endif // CTEXTEDITDELEGATE_H
