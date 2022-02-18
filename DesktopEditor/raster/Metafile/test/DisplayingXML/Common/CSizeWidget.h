#ifndef CSIZEWIDGET_H
#define CSIZEWIDGET_H

#include "CSharedWidget.h"

class CSizeWidget : public CSharedWidget
{
public:
        explicit CSizeWidget(const QString& qsName, QWidget *pParent = NULL);
        virtual ~CSizeWidget();

        virtual QList<QStandardItem*> GetData() const override;
private:
        QString m_qsName;
};

#endif // CSIZEWIDGET_H
