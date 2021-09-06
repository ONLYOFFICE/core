#ifndef CLOGBRUSHEXWIDGET_H
#define CLOGBRUSHEXWIDGET_H

#include "CSharedWidget.h"

class CLogBrushExWidget : public CSharedWidget
{
public:
        explicit CLogBrushExWidget(QWidget *pParent = NULL);
        virtual ~CLogBrushExWidget();

        virtual QList<QStandardItem*> GetData() const override;
};

#endif // CLOGBRUSHEXWIDGET_H
