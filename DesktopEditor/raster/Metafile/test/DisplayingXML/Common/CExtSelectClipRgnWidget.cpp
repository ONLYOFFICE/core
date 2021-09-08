#include "CFormWidget.h"
#include "CRectangleWidget.h"
#include "CExtSelectClipRgnWidget.h"

#include <QPushButton>
#include <QBoxLayout>

CExtSelectClipRgnWidget::CExtSelectClipRgnWidget(QWidget *pParent)
        : CSharedWidget(pParent),
          m_unCounter(0)
{
        QVBoxLayout *pLayout = new QVBoxLayout;

        CRectangleWidget *pRectangleWidget = new CRectangleWidget("Bounds");
        CFormWidget *pIhBrushWidget = new CFormWidget("RegionMode", "1");

        QPushButton *pAddedButton = new QPushButton("Добавить значение");

        connect(pAddedButton, &QPushButton::clicked, this, &CExtSelectClipRgnWidget::on_createRgnData_clicked);

        m_arWidgets.push_back(pRectangleWidget);
        m_arWidgets.push_back(pIhBrushWidget);

        pLayout->addWidget(pRectangleWidget);
        pLayout->addWidget(pIhBrushWidget);
        pLayout->addWidget(pAddedButton);

        setLayout(pLayout);
}

CExtSelectClipRgnWidget::~CExtSelectClipRgnWidget()
{

}

QList<QStandardItem *> CExtSelectClipRgnWidget::GetData() const
{
        QList<QStandardItem *> arDatas;

        for (const CSharedWidget* pWidget : m_arWidgets)
                arDatas.append(pWidget->GetData());

        return arDatas;
}

void CExtSelectClipRgnWidget::on_createRgnData_clicked()
{
        CFormWidget *pDataItem = new CFormWidget(QString("RgnData%1").arg(++m_unCounter));
        m_arWidgets.push_back(pDataItem);

        QBoxLayout *pMainLayout = (QBoxLayout*)layout();
        pMainLayout->insertWidget(pMainLayout->count() - 1, pDataItem);
}
