#include "CSmallTextOutWidget.h"
#include "CRectangleWidget.h"
#include "CFormWidget.h"

#include <QBoxLayout>

CSmallTextOutWidget::CSmallTextOutWidget(QWidget *pParent)
{
        QVBoxLayout *pLayout = new QVBoxLayout();

        CFormWidget *pXWidget               = new CFormWidget("x", "0");
        CFormWidget *pYWidget               = new CFormWidget("y", "0");
        CFormWidget *pFuOptionsWidget       = new CFormWidget("fuOptions", "0");
        CFormWidget *pIGraphicsModeWidget   = new CFormWidget("iGraphicsMode", "0");
        CFormWidget *pExScaleWidget         = new CFormWidget("exScale", "0");
        CFormWidget *pEyScaleWidget         = new CFormWidget("eyScale", "0");
        CRectangleWidget *pBoundsWidget     = new CRectangleWidget("Bounds");
        CFormWidget *pTextStringWidget      = new CFormWidget("TextString");

        m_arWidgets.push_back(pXWidget);
        m_arWidgets.push_back(pYWidget);
        m_arWidgets.push_back(pFuOptionsWidget);
        m_arWidgets.push_back(pIGraphicsModeWidget);
        m_arWidgets.push_back(pExScaleWidget);
        m_arWidgets.push_back(pEyScaleWidget);
        m_arWidgets.push_back(pBoundsWidget);
        m_arWidgets.push_back(pTextStringWidget);

        pLayout->addWidget(pXWidget);
        pLayout->addWidget(pYWidget);
        pLayout->addWidget(pFuOptionsWidget);
        pLayout->addWidget(pIGraphicsModeWidget);
        pLayout->addWidget(pExScaleWidget);
        pLayout->addWidget(pEyScaleWidget);
        pLayout->addWidget(pBoundsWidget);
        pLayout->addWidget(pTextStringWidget);

        setLayout(pLayout);
}

CSmallTextOutWidget::~CSmallTextOutWidget()
{

}

QList<QStandardItem *> CSmallTextOutWidget::GetData() const
{
        QList<QStandardItem *> arDatas;

        for (const CSharedWidget* pWidget : m_arWidgets)
                arDatas.append(pWidget->GetData());

        return arDatas;
}
