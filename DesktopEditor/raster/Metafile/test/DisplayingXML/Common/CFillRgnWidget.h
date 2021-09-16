#ifndef CFILLRGNWIDGET_H
#define CFILLRGNWIDGET_H

#include "CSharedWidget.h"

class CFillRgnWidget : public CSharedWidget
{
public:
        explicit CFillRgnWidget(QWidget *pParent = NULL);
        virtual ~CFillRgnWidget();
private slots:
        void on_createRgnData_clicked();
private:
        unsigned int m_unCounter;
};

#endif // CFILLRGNWIDGET_H
