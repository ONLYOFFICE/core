#ifndef CSTATISTICSWIDGET_H
#define CSTATISTICSWIDGET_H

#include <QWidget>
#include "ui_Statistics.h"

class CStatisticsWidget : public QWidget
{
    Q_OBJECT
public:
    CStatisticsWidget(QWidget *parent = NULL);
    ~CStatisticsWidget();

    void SetMainWindow(QWidget *pMainWidget);
    void SetStatistics(QMap<QString, unsigned int>* mStaticstics);

private slots:
    void on_StatisticsExit_clicked();

private:
    Ui::Form *ui;
    QWidget *pMainWindow;
    virtual void resizeEvent(QResizeEvent *);
};

#endif // CSTATISTICSWIDGET_H
