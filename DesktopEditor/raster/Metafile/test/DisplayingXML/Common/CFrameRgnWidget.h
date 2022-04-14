#ifndef CFRAMERGNWIDGET_H
#define CFRAMERGNWIDGET_H

#include "CSharedWidget.h"

class CFrameRgnWidget : public CSharedWidget
{
public:
        explicit CFrameRgnWidget(QWidget *pParent = NULL);
        virtual ~CFrameRgnWidget();
private slots:
        void on_createRgnData_clicked();
private:
        unsigned int m_unCounter;
};

#endif // CFRAMERGNWIDGET_H
