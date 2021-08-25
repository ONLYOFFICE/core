#ifndef CEDITITEMWIDGET_H
#define CEDITITEMWIDGET_H

#include <QWidget>
#include <QStandardItem>

namespace Ui {
        class CEditItemWidget;
}

class CEditItemWidget : public QWidget
{
        Q_OBJECT
public:
        explicit CEditItemWidget(QWidget *parent = nullptr);
        ~CEditItemWidget();

        void SetMainWindow(QWidget *pMainWidget);
        void SetItem(QStandardItem *pStandardItem);

private slots:
    void on_Save_clicked();
    void on_Cancel_clicked();

private:
        void ParsingItem();
        void ParsingAttachments(QStandardItem* pStandardItem, unsigned int unLevel = 0);

        Ui::CEditItemWidget *ui;
        QWidget *m_pMainWindow;
        QStandardItem *m_pStandardItem;
};

#endif // CEDITITEMWIDGET_H
