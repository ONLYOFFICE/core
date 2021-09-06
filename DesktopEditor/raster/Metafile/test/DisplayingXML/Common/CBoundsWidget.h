#ifndef CBOUNDSWIDGET_H
#define CBOUNDSWIDGET_H

#include "CSharedWidget.h"

class CReactangleWidget : public CSharedWidget
{
public:
        explicit CReactangleWidget(const QString& qsName, QWidget *pParent = NULL);
        virtual ~CReactangleWidget();

        virtual QList<QStandardItem*> GetData() const override;
private:
        QString m_qsName;
};

#endif // CBOUNDSWIDGET_H
