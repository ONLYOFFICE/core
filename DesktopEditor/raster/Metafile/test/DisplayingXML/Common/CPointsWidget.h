#ifndef CPOINTSWIDGET_H
#define CPOINTSWIDGET_H

#include "CSharedWidget.h"

class CPointsWidget : public CSharedWidget
{
        Q_OBJECT
public:
        explicit CPointsWidget(unsigned int unIndex = 1, QWidget *pParent = NULL);
        virtual ~CPointsWidget();

        virtual QList<QStandardItem*> GetData() const override;
private slots:
        void on_createPoint_clicked();
private:
        unsigned int m_unIndex;
        unsigned int m_unCounter;
};

#endif // CPOINTSWIDGET_H
