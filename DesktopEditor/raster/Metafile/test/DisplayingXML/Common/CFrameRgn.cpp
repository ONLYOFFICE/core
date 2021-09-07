#include "CFrameRgn.h"
#include "CFormWidget.h"
#include "CRectangleWidget.h"

#include <QPushButton>
#include <QBoxLayout>

CFrameRgn::CFrameRgn(QWidget *pParent)
        : CSharedWidget(pParent),
          m_unCounter(0)
{
        QVBoxLayout *pLayout = new QVBoxLayout;

        CRectangleWidget *pBoundsWidget = new CRectangleWidget("Bounds");
        CFormWidget *pIhBrushWidget     = new CFormWidget("ihBrush",    "1");
        CFormWidget *pWidthWidget       = new CFormWidget("Width",      "1");
        CFormWidget *pHeightWidget      = new CFormWidget("Height ",    "1");

        m_arWidgets.push_back(pBoundsWidget);
        m_arWidgets.push_back(pIhBrushWidget);
        m_arWidgets.push_back(pWidthWidget);
        m_arWidgets.push_back(pHeightWidget);

        QPushButton *pAddedButton = new QPushButton("Добавить значение");

        connect(pAddedButton, &QPushButton::clicked, this, &CFrameRgn::on_createRgnData_clicked);

        pLayout->addWidget(pBoundsWidget);
        pLayout->addWidget(pIhBrushWidget);
        pLayout->addWidget(pWidthWidget);
        pLayout->addWidget(pHeightWidget);
        pLayout->addWidget(pAddedButton);

        setLayout(pLayout);
}

CFrameRgn::~CFrameRgn()
{

}

QList<QStandardItem *> CFrameRgn::GetData() const
{
        QList<QStandardItem *> arDatas;

        for (const CSharedWidget* pWidget : m_arWidgets)
                arDatas.append(pWidget->GetData());

        return arDatas;
}

void CFrameRgn::on_createRgnData_clicked()
{
        CFormWidget *pDataItem = new CFormWidget(QString("RgnData%1").arg(++m_unCounter));
        m_arWidgets.push_back(pDataItem);

        QBoxLayout *pMainLayout = (QBoxLayout*)layout();
        pMainLayout->insertWidget(pMainLayout->count() - 1, pDataItem);
}
