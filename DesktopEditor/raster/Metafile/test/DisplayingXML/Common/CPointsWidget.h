#ifndef CPOINTSWIDGET_H
#define CPOINTSWIDGET_H

#include "CSharedWidget.h"

class CPointsWidget : public CSharedWidget
{
        Q_OBJECT
public:
        explicit CPointsWidget(QWidget *pParent = NULL);
        virtual ~CPointsWidget();
private slots:
        void on_createPoint_clicked();
private:
        unsigned int m_unCounter;
};

#endif // CPOINTSWIDGET_H
