#include "CPolyTextOut.h"
#include "CRectangleWidget.h"
#include "CExtTextOutWidget.h"

#include <QPushButton>
#include <QBoxLayout>

CPolyTextOut::CPolyTextOut(QWidget *pParent)
        : CSharedWidget(pParent),
          m_unCounter(0)
{
        QVBoxLayout *pLayout = new QVBoxLayout();

        CRectangleWidget *pBoundsWidget     = new CRectangleWidget("Bounds");
        CFormWidget *pIGraphicsModeWidget   = new CFormWidget("iGraphicsMode", "1");
        CFormWidget *pExScaleWidget         = new CFormWidget("exScale", "1");
        CFormWidget *pEyScaleWidget         = new CFormWidget("eyScale", "1");

        QPushButton *pAddedButton = new QPushButton("Добавить объект текста");

        connect(pAddedButton, &QPushButton::clicked, this, &CPolyTextOut::on_createTextOut_clicked);

        m_arWidgets.push_back(pBoundsWidget);
        m_arWidgets.push_back(pIGraphicsModeWidget);
        m_arWidgets.push_back(pExScaleWidget);
        m_arWidgets.push_back(pEyScaleWidget);

        pLayout->addWidget(pBoundsWidget);
        pLayout->addWidget(pIGraphicsModeWidget);
        pLayout->addWidget(pExScaleWidget);
        pLayout->addWidget(pEyScaleWidget);

        pLayout->addWidget(pAddedButton);

        setLayout(pLayout);
}

CPolyTextOut::~CPolyTextOut()
{

}

QList<QStandardItem *> CPolyTextOut::GetData() const
{
        QList<QStandardItem *> arDatas;

        for (const CSharedWidget* pWidget : m_arWidgets)
                arDatas.append(pWidget->GetData());

        return arDatas;
}

void CPolyTextOut::on_createTextOut_clicked()
{
        CEmrTextWidget *pExtTextOutWidget = new CEmrTextWidget(++m_unCounter);
        m_arWidgets.push_back(pExtTextOutWidget);

        QBoxLayout *pMainLayout = (QBoxLayout*)layout();
        pMainLayout->insertWidget(pMainLayout->count() - 1, pExtTextOutWidget);
}
