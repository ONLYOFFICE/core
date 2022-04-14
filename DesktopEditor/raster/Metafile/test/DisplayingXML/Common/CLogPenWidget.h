#ifndef CLOGPENWIDGET_H
#define CLOGPENWIDGET_H

#include "CSharedWidget.h"

class CLogPenWidget : public CSharedWidget
{
public:
        explicit CLogPenWidget(QWidget *pParent = NULL);
        virtual ~CLogPenWidget();

        virtual QList<QStandardItem*> GetData() const override;

};

#endif // CLOGPENWIDGET_H
