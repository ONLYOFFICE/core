#ifndef CEXTSELECTCLIPRGNWIDGET_H
#define CEXTSELECTCLIPRGNWIDGET_H

#include "CSharedWidget.h"

class CExtSelectClipRgnWidget : public CSharedWidget
{
public:
        explicit CExtSelectClipRgnWidget(QWidget *pParent = NULL);
        virtual ~CExtSelectClipRgnWidget();
private slots:
        void on_createRgnData_clicked();
private:
        unsigned int m_unCounter;
};

#endif // CEXTSELECTCLIPRGNWIDGET_H
