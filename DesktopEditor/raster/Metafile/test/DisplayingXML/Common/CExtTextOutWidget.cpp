#include "CExtTextOutWidget.h"
#include "CRectangleWidget.h"
#include "CPointWidget.h"
#include "CFormWidget.h"

#include <QPushButton>
#include <QHBoxLayout>
#include <QLabel>

CExtTextOutWidget::CExtTextOutWidget(QWidget *pParent)
        : CSharedWidget(pParent)
{
        QVBoxLayout *pLayout = new QVBoxLayout();

        CRectangleWidget *pBoundsWidget     = new CRectangleWidget("Bounds");
        CFormWidget *pIGraphicsModeWidget   = new CFormWidget("iGraphicsMode", "1");
        CFormWidget *pExScaleWidget         = new CFormWidget("exScale", "1");
        CFormWidget *pEyScaleWidget         = new CFormWidget("eyScale", "1");
        CEmrTextWidget *pEmrTextWidget      = new CEmrTextWidget();

        m_arWidgets.push_back(pBoundsWidget);
        m_arWidgets.push_back(pIGraphicsModeWidget);
        m_arWidgets.push_back(pExScaleWidget);
        m_arWidgets.push_back(pEyScaleWidget);
        m_arWidgets.push_back(pEmrTextWidget);

        pLayout->addWidget(pBoundsWidget);
        pLayout->addWidget(pIGraphicsModeWidget);
        pLayout->addWidget(pExScaleWidget);
        pLayout->addWidget(pEyScaleWidget);
        pLayout->addWidget(pEmrTextWidget);

        setLayout(pLayout);
}

CExtTextOutWidget::~CExtTextOutWidget()
{

}

QList<QStandardItem *> CExtTextOutWidget::GetData() const
{
        QList<QStandardItem *> arDatas;

        for (const CSharedWidget *pWidget : m_arWidgets)
                arDatas.append(pWidget->GetData());

        return arDatas;
}

CEmrTextWidget::CEmrTextWidget(unsigned int unIndex, QWidget *pParent)
        : CSharedWidget(pParent),
          m_unIndex(unIndex),
          m_pTextWidget(NULL),
          m_pDxWidget(NULL)
{
        QVBoxLayout *pLayout = new QVBoxLayout();

        QLabel *pName = new QLabel(((m_unIndex == 0) ? "EmrText:" : QString("EmrText%1:").arg(m_unIndex)));
        pName->setStyleSheet("QLabel { text-decoration: underline; }");

        CPointWidget *pReferenceWidget          = new CPointWidget("Reference");
        CFormWidget *pOptionsWidget             = new CFormWidget("Options", "2");
        CRectangleWidget *pRectangleWidget      = new CRectangleWidget("Rectangle");
        m_pTextWidget                             = new CFormWidget("Text");

        QPushButton *pCreateDxButton = new QPushButton("Создать поля для Dx значений");

        connect(pCreateDxButton, &QPushButton::clicked, this, &CEmrTextWidget::on_createDx_clicked);

        m_arWidgets.push_back(pReferenceWidget);
        m_arWidgets.push_back(pOptionsWidget);
        m_arWidgets.push_back(pRectangleWidget);
        m_arWidgets.push_back(m_pTextWidget);

        pLayout->addWidget(pName);
        pLayout->addWidget(pReferenceWidget);
        pLayout->addWidget(pOptionsWidget);
        pLayout->addWidget(pRectangleWidget);
        pLayout->addWidget(m_pTextWidget);
        pLayout->addWidget(pCreateDxButton);

        setLayout(pLayout);
}

CEmrTextWidget::~CEmrTextWidget()
{

}

QList<QStandardItem *> CEmrTextWidget::GetData() const
{
        QStandardItem *pItem = new QStandardItem(((m_unIndex == 0) ? "<EmrText>" : QString("<ExtText%1>").arg(m_unIndex)));

        for (const CSharedWidget *pWidget : m_arWidgets)
                pItem->appendRows(pWidget->GetData());

        return {pItem};
}

void CEmrTextWidget::on_createDx_clicked()
{
        if (NULL == m_pTextWidget)
                return;

        QString qsValue = m_pTextWidget->GetTextEditWidget()->toPlainText();

        if (qsValue.isEmpty())
                return;

        if (NULL != m_pDxWidget)
        {
               delete layout()->takeAt(layout()->indexOf(m_pDxWidget))->widget();
        }

        m_pDxWidget = new CDxWidget(qsValue);
        m_arWidgets.push_back(m_pDxWidget);

        layout()->addWidget(m_pDxWidget);
}

CDxWidget::CDxWidget(const QString& qsValues, QWidget *pParent)
        : CSharedWidget(pParent)
{
        if (qsValues.isEmpty())
                return;

        QVBoxLayout *pLayout = new QVBoxLayout();

        QLabel *pName = new QLabel("Dx:");
        pName->setStyleSheet("QLabel { text-decoration: underline; }");

        pLayout->addWidget(pName);

        for (unsigned int unIndex = 0; unIndex < qsValues.length(); ++unIndex)
        {
                CFormWidget *pDxValue = new CFormWidget(QString("Dx%1").arg(unIndex + 1));
                m_arWidgets.push_back(pDxValue);
                pLayout->addWidget(pDxValue);
        }

        setLayout(pLayout);
}

CDxWidget::~CDxWidget()
{

}

QList<QStandardItem *> CDxWidget::GetData() const
{
        QStandardItem *pItem = new QStandardItem("<Dx>");

        for (const CSharedWidget* pWidget : m_arWidgets)
                pItem->appendRows(pWidget->GetData());

        return {pItem};
}
