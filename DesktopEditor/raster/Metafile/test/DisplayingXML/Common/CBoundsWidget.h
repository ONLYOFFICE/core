#ifndef CBOUNDSWIDGET_H
#define CBOUNDSWIDGET_H

#include "CSharedWidget.h"

class CBoundsWidget : public CSharedWidget
{
public:
        explicit CBoundsWidget(QWidget *pParent = NULL);
        virtual ~CBoundsWidget();

        virtual QList<QStandardItem*> GetData() const override;
};

#endif // CBOUNDSWIDGET_H
