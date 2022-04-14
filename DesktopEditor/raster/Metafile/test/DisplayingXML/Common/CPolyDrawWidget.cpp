#include "CPolyDrawWidget.h"
#include "CRectangleWidget.h"
#include "CPointWidget.h"
#include "CFormWidget.h"

#include <QPushButton>
#include <QHBoxLayout>

CPolyDrawWidget::CPolyDrawWidget(QWidget *pParent)
        : CSharedWidget(pParent),
          m_unCounter(0)
{
        QVBoxLayout *pLayout = new QVBoxLayout;

        QPushButton *pAddedPoint        = new QPushButton("Добавить точку");

        connect(pAddedPoint, &QPushButton::clicked, this, &CPolyDrawWidget::on_createPoint_clicked);

        AddWidget(new CRectangleWidget("Bounds"), pLayout);

        pLayout->addWidget(pAddedPoint);

        setLayout(pLayout);
}

CPolyDrawWidget::~CPolyDrawWidget()
{

}

QList<QStandardItem *> CPolyDrawWidget::GetData() const
{
        QList<QStandardItem*> arDatas;

        arDatas.append(m_arWidgets[0]->GetData());

        if (m_arWidgets.size() > 1)
        {
                for (unsigned int unIndex = 1; unIndex < m_arWidgets.size(); unIndex += 2)
                        arDatas.append(m_arWidgets[unIndex]->GetData());

                for (unsigned int unIndex = 2; unIndex < m_arWidgets.size(); unIndex += 2)
                        arDatas.append(m_arWidgets[unIndex]->GetData());
        }

        return arDatas;
}

void CPolyDrawWidget::on_createPoint_clicked()
{
        CPointWidget *pPoint = new CPointWidget("Point", ++m_unCounter);
        CFormWidget *pAbType = new CFormWidget(QString("AbType%1").arg(m_unCounter), "0");

        m_arWidgets.push_back(pPoint);
        m_arWidgets.push_back(pAbType);

        QBoxLayout *pMainLayout = (QBoxLayout*)layout();
        pMainLayout->insertWidget(pMainLayout->count() - 1, pPoint);
        pMainLayout->insertWidget(pMainLayout->count() - 1, pAbType);
}
