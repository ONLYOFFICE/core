#ifndef CSIZEWIDGET_H
#define CSIZEWIDGET_H

#include "CSharedWidget.h"

class CSizeWidget : public CSharedWidget
{
public:
        explicit CSizeWidget(QWidget *pParent = NULL);
        virtual ~CSizeWidget();

        virtual QList<QStandardItem*> GetData() const override;

};

#endif // CSIZEWIDGET_H
