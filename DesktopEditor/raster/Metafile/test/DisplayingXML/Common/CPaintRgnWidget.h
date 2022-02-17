#ifndef CPAINTRGNWIDGET_H
#define CPAINTRGNWIDGET_H

#include "CSharedWidget.h"

class CPaintRgnWidget : public CSharedWidget
{
public:
        explicit CPaintRgnWidget(QWidget *pParent = NULL);
        virtual ~CPaintRgnWidget();
};

#endif // CPAINTRGNWIDGET_H
