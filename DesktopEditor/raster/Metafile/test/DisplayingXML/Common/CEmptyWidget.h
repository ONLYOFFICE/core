#ifndef CEMPTYWIDGET_H
#define CEMPTYWIDGET_H

#include "CSharedWidget.h"

class CEmptyWidget : public CSharedWidget
{
public:
        explicit CEmptyWidget(const QString& qsName, QWidget *pParent = NULL);
        virtual ~CEmptyWidget();

        virtual QList<QStandardItem*> GetData() const override;
private:
        QString m_qsName;
};

#endif // CEMPTYWIDGET_H
