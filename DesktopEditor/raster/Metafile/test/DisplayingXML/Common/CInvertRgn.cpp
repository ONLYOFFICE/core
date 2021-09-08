#include "CInvertRgn.h"
#include "CFormWidget.h"
#include "CRectangleWidget.h"

#include <QPushButton>
#include <QBoxLayout>

CInvertRgn::CInvertRgn(QWidget *pParent)
        : CSharedWidget(pParent),
          m_unCounter(0)
{
        QVBoxLayout *pLayout = new QVBoxLayout();

        CRectangleWidget *pBoundsWidget = new CRectangleWidget("Bounds");
        QPushButton *pAddedButton       = new QPushButton("Добавить значение");

        connect(pAddedButton, &QPushButton::clicked, this, &CInvertRgn::on_createRgnData_clicked);

        m_arWidgets.push_back(pBoundsWidget);

        pLayout->addWidget(pBoundsWidget);
        pLayout->addWidget(pAddedButton);

        setLayout(pLayout);
}

CInvertRgn::~CInvertRgn()
{

}

QList<QStandardItem *> CInvertRgn::GetData() const
{
        QList<QStandardItem *> arDatas;

        for (const CSharedWidget* pWidget : m_arWidgets)
                arDatas.append(pWidget->GetData());

        return arDatas;
}

void CInvertRgn::on_createRgnData_clicked()
{
        CFormWidget *pDataItem = new CFormWidget(QString("RgnData%1").arg(++m_unCounter));
        m_arWidgets.push_back(pDataItem);

        QBoxLayout *pMainLayout = (QBoxLayout*)layout();
        pMainLayout->insertWidget(pMainLayout->count() - 1, pDataItem);
}
