#ifndef CEDITITEMWIDGET_H
#define CEDITITEMWIDGET_H

#include <QWidget>
#include <QTextEdit>
#include <QStandardItem>

#include "MainWindow.h"

namespace Ui {
        class CEditItemWidget;
}

class CEditItemWidget : public QWidget
{
        Q_OBJECT
    public:
        explicit CEditItemWidget(QWidget *parent = nullptr);
        ~CEditItemWidget();

        void SetMainWindow(MainWindow *pMainWindow);
        void SetItem(QStandardItem *pStandardItem);

    private slots:
        void on_Save_clicked();
        void on_Cancel_clicked();

        void on_DeleteItem_clicked();

    private:
        void ParsingItem();
        void ParsingAttachments(QStandardItem* pStandardItem, unsigned int unLevel = 0);

        void closeEvent(QCloseEvent *event) override;

        Ui::CEditItemWidget *ui;
        MainWindow *m_pMainWindow;
        QStandardItem *m_pStandardItem;

        QMap<QTextEdit*, QStandardItem*> m_oBind;
};

#endif // CEDITITEMWIDGET_H
