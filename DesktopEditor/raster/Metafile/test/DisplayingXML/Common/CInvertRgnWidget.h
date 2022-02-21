#ifndef CINVERTRGNWIDGET_H
#define CINVERTRGNWIDGET_H

#include "CSharedWidget.h"

class CInvertRgnWidget : public CSharedWidget
{
public:
        explicit CInvertRgnWidget(QWidget *pParent = NULL);
        virtual ~CInvertRgnWidget();
private slots:
        void on_createRgnData_clicked();
private:
        unsigned int m_unCounter;
};

#endif // CINVERTRGNWIDGET_H
