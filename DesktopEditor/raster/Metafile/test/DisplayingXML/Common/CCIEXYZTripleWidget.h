#ifndef CCIEXYZTRIPLEWIDGET_H
#define CCIEXYZTRIPLEWIDGET_H

#include "CSharedWidget.h"

class CCIEXYZTripleWidget : public CSharedWidget
{
public:
        explicit CCIEXYZTripleWidget(QWidget *pParent = NULL);
        virtual ~CCIEXYZTripleWidget();

        virtual QList<QStandardItem*> GetData() const override;
};

class CIEXYZWidget : public CSharedWidget
{
public:
        explicit CIEXYZWidget(const QString& qsName = "", QWidget *pParent = NULL);
        virtual ~CIEXYZWidget();

        virtual QList<QStandardItem*> GetData() const override;
private:
        QString m_qsName;
};

#endif // CCIEXYZTRIPLEWIDGET_H
