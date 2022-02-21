#ifndef CRECTANGLEWIDGET_H
#define CRECTANGLEWIDGET_H

#include "CSharedWidget.h"

class CRectangleWidget : public CSharedWidget
{
public:
        explicit CRectangleWidget(const QString& qsName, QWidget *pParent = NULL);
        virtual ~CRectangleWidget();

        virtual QList<QStandardItem*> GetData() const override;
private:
        QString m_qsName;
};

#endif // CRECTANGLEWIDGET_H
