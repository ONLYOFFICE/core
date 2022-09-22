#ifndef CGRADIENTFILLWIDGET_H
#define CGRADIENTFILLWIDGET_H

#include "CSharedWidget.h"
#include "CFormWidget.h"

#include <QBoxLayout>

class CGradientFillWidget : public CSharedWidget
{
public:
        explicit CGradientFillWidget(QWidget *pParent = NULL);
        virtual ~CGradientFillWidget();
private slots:
        void on_createObjects_clicked();
private:
        CFormWidget *m_pNVerWidget;
        CFormWidget *m_pNTriWidget;
        CFormWidget *m_pUlModeWidget;
        QVBoxLayout *m_pDataLayout;
};

class CTriVertexWidget : public CSharedWidget
{
public:
        explicit CTriVertexWidget(unsigned int unIndex = 0, QWidget *pParent = NULL);
        virtual ~CTriVertexWidget();

        virtual QList<QStandardItem*> GetData() const override;
private:
        unsigned int m_unIndex;
};

class CGradientRectangleWidget : public CSharedWidget
{
public:
        explicit CGradientRectangleWidget(unsigned int unIndex = 0, QWidget *pParent = NULL);
        virtual ~CGradientRectangleWidget();

        virtual QList<QStandardItem*> GetData() const override;
private:
        unsigned int m_unIndex;
};

class CGradientTriangleWidget : public CSharedWidget
{
public:
        explicit CGradientTriangleWidget(unsigned int unIndex = 0, QWidget *pParent = NULL);
        virtual ~CGradientTriangleWidget();

        virtual QList<QStandardItem*> GetData() const override;
private:
        unsigned int m_unIndex;
};

#endif // CGRADIENTFILLWIDGET_H
