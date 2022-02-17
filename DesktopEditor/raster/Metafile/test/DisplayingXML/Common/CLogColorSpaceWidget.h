#ifndef CLOGCOLORSPACEWIDGET_H
#define CLOGCOLORSPACEWIDGET_H

#include "CSharedWidget.h"

class CLogColorSpaceWidget : public CSharedWidget
{
public:
        explicit CLogColorSpaceWidget(QWidget *pParent = NULL);
        virtual ~CLogColorSpaceWidget();

        virtual QList<QStandardItem*> GetData() const override;
};

#endif // CLOGCOLORSPACEWIDGET_H
