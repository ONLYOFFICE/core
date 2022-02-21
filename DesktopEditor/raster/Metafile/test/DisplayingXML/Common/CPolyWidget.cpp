#include "CPolyWidget.h"

#include "CPointsWidget.h"

#include <QPushButton>
#include <QBoxLayout>
#include <QLabel>

CPolyWidget::CPolyWidget(QWidget *pParent):
        CSharedWidget(pParent),
        m_unCounter(0)
{
        QVBoxLayout *pLayout = new QVBoxLayout;

        QPushButton *pButtonAddPoly = new QPushButton("Add Poly");

        connect(pButtonAddPoly, &QPushButton::clicked, this, &CPolyWidget::on_createPoly_clicked);

        pLayout->addWidget(pButtonAddPoly);

        setLayout(pLayout);
}

CPolyWidget::~CPolyWidget()
{

}

QList<QStandardItem*> CPolyWidget::GetData() const
{
        if (m_arWidgets.empty())
                return {};

        QList<QStandardItem*> oPolyList;

        for (unsigned int unIndex = 0; unIndex < m_arWidgets.size(); ++unIndex)
        {
                QStandardItem *pPolyItem = new QStandardItem(QString("<PolygonPoint%1>").arg(unIndex + 1));
                pPolyItem->appendRows(m_arWidgets[unIndex]->GetData());
                oPolyList.append(pPolyItem);
        }

        return oPolyList;
}

void CPolyWidget::on_createPoly_clicked()
{
        QLabel *pName = new QLabel(QString("Poly %1:").arg(++m_unCounter));
        CPointsWidget *pPointsWiget = new CPointsWidget();

        m_arWidgets.push_back(pPointsWiget);

        QBoxLayout *pMainLayout = (QBoxLayout*)layout();
        pMainLayout->insertWidget(pMainLayout->count() - 1, pName);
        pMainLayout->insertWidget(pMainLayout->count() - 1, pPointsWiget);
}
