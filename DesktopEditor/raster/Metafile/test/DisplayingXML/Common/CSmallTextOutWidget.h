#ifndef CSMALLTEXTOUT_H
#define CSMALLTEXTOUT_H

#include "CSharedWidget.h"


class CSmallTextOutWidget : public CSharedWidget
{
public:
        explicit CSmallTextOutWidget(QWidget *pParent = NULL);
        virtual ~CSmallTextOutWidget();

        virtual QList<QStandardItem*> GetData() const override;
};

#endif // CSMALLTEXTOUT_H
