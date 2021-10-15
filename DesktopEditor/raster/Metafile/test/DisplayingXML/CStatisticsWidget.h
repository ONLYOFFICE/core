#ifndef CSTATISTICSWIDGET_H
#define CSTATISTICSWIDGET_H

#include <QWidget>
#include "ui_Statistics.h"

namespace Ui {
        class CStatisticsWidget;
}

class CStatisticsWidget : public QWidget
{
        Q_OBJECT
    public:
        CStatisticsWidget(QWidget *parent = NULL);
        ~CStatisticsWidget();

        void SetMainWindow(QWidget *pMainWidget);
        void SetStatistics(const QMap<QString, unsigned int>& mStaticstics);

    private:
        Ui::CStatisticsWidget *ui;
        QWidget *pMainWindow;

        virtual void resizeEvent(QResizeEvent *) override;
        virtual void closeEvent(QCloseEvent *event) override;
};

#endif // CSTATISTICSWIDGET_H
