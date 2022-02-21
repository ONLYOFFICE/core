#ifndef CEDITITEMWIDGET_H
#define CEDITITEMWIDGET_H

#include <QWidget>
#include <QTextEdit>
#include <QBoxLayout>
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
        void slotSaveButtonCliked();
        void slotCancelButtonClicked();

        void slotDeleteButtonClicked();

    signals:
        void signalDeleteItem(QStandardItem *pDeletedItem);

    private:
        void ParsingItem();
        void ParsingAttachments(QStandardItem* pStandardItem, unsigned int unLevel = 0);

        void closeEvent(QCloseEvent *event) override;
        void DeleteItem(QStandardItem *pStandardItem);

        Ui::CEditItemWidget *ui;
        MainWindow *m_pMainWindow;
        QStandardItem *m_pStandardItem;
        QBoxLayout *m_pContentLayout;
        QBoxLayout *m_pButtonsLayout;

        QMap<QTextEdit*, QStandardItem*> m_oBind;
};

#endif // CEDITITEMWIDGET_H
