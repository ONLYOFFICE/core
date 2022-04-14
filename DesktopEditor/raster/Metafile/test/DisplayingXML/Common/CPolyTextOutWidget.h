#ifndef CPOLYTEXTOUTWIDGET_H
#define CPOLYTEXTOUTWIDGET_H

#include "CSharedWidget.h"

class CPolyTextOutWidget : public CSharedWidget
{
public:
        explicit CPolyTextOutWidget(QWidget *pParent = NULL);
        virtual ~CPolyTextOutWidget();
private slots:
        void on_createTextOut_clicked();
private:
        unsigned int m_unCounter;
};

#endif // CPOLYTEXTOUTWIDGET_H
