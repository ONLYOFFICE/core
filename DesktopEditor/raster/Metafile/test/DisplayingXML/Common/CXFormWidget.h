#ifndef CXFORMWIDGET_H
#define CXFORMWIDGET_H

#include "CSharedWidget.h"

class CXFormWidget : public CSharedWidget
{
public:
        explicit CXFormWidget(QWidget *pParent = NULL);
        virtual ~CXFormWidget();

        QList<QStandardItem *> GetData() const override;
};

#endif // CXFORMWIDGET_H
