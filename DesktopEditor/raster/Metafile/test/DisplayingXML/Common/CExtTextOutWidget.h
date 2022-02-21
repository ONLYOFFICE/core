#ifndef CEXTTEXTOUTWIDGET_H
#define CEXTTEXTOUTWIDGET_H

#include "CFormWidget.h"

class CExtTextOutWidget : public CSharedWidget
{
public:
        explicit CExtTextOutWidget(QWidget *pParent = NULL);
        virtual ~CExtTextOutWidget();
};

class CDxWidget;

class CEmrTextWidget : public CSharedWidget
{
public:
        explicit CEmrTextWidget(unsigned int unIndex = 0, QWidget *pParent = NULL);
        virtual ~CEmrTextWidget();

        virtual QList<QStandardItem*> GetData() const override;
private slots:
        void on_createDx_clicked();
private:
        unsigned int m_unIndex;
        CFormWidget* m_pTextWidget;
        CDxWidget* m_pDxWidget;
};

class CDxWidget: public CSharedWidget
{
public:
        explicit CDxWidget(const QString& qsValues, QWidget *pParent = NULL);
        virtual ~CDxWidget();

        virtual QList<QStandardItem*> GetData() const override;
};

#endif // CEXTTEXTOUTWIDGET_H
