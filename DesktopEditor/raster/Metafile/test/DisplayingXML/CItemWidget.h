#ifndef CITEMWIDGET_H
#define CITEMWIDGET_H

#include <QStyleOptionViewItem>
#include <QPushButton>
#include <QTextEdit>
#include <QWidget>

class CItemWidget : public QWidget
{
        Q_OBJECT
    public:
        explicit CItemWidget(QWidget* parent = NULL);
        virtual ~CItemWidget();

        QString GetText();

        void SetText(const QString& qsString);
        void SetFont(const QFont&   oFont);
    private:
        QTextEdit   *m_pTextEdit;
};

#endif // CITEMWIDGET_H
