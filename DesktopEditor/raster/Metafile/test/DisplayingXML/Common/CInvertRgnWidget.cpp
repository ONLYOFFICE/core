#include "CInvertRgnWidget.h"
#include "CFormWidget.h"
#include "CRectangleWidget.h"

#include <QPushButton>
#include <QBoxLayout>

CInvertRgnWidget::CInvertRgnWidget(QWidget *pParent)
        : CSharedWidget(pParent),
          m_unCounter(0)
{
        QVBoxLayout *pLayout = new QVBoxLayout();

        QPushButton *pAddedButton       = new QPushButton("Добавить значение");

        connect(pAddedButton, &QPushButton::clicked, this, &CInvertRgnWidget::on_createRgnData_clicked);

        AddWidget(new CRectangleWidget("Bounds"), pLayout);

        pLayout->addWidget(pAddedButton);

        setLayout(pLayout);
}

CInvertRgnWidget::~CInvertRgnWidget()
{

}

void CInvertRgnWidget::on_createRgnData_clicked()
{
        CFormWidget *pDataItem = new CFormWidget(QString("RgnData%1").arg(++m_unCounter));
        m_arWidgets.push_back(pDataItem);

        QBoxLayout *pMainLayout = (QBoxLayout*)layout();
        pMainLayout->insertWidget(pMainLayout->count() - 1, pDataItem);
}
