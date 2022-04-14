#include "CFormWidget.h"
#include "CFillRgnWidget.h"
#include "CRectangleWidget.h"

#include <QPushButton>
#include <QBoxLayout>

CFillRgnWidget::CFillRgnWidget(QWidget *pParent)
        : CSharedWidget(pParent),
          m_unCounter(0)
{
        QVBoxLayout *pLayout = new QVBoxLayout;

        QPushButton *pAddedButton = new QPushButton("Добавить значение");

        connect(pAddedButton, &QPushButton::clicked, this, &CFillRgnWidget::on_createRgnData_clicked);

        AddWidgets({new CRectangleWidget("Bounds"),
                    new CFormWidget("ihBrush", "1")}, pLayout);

        pLayout->addWidget(pAddedButton);

        setLayout(pLayout);
}

CFillRgnWidget::~CFillRgnWidget()
{

}

void CFillRgnWidget::on_createRgnData_clicked()
{
        CFormWidget *pDataItem = new CFormWidget(QString("RgnData%1").arg(++m_unCounter));
        m_arWidgets.push_back(pDataItem);

        QBoxLayout *pMainLayout = (QBoxLayout*)layout();
        pMainLayout->insertWidget(pMainLayout->count() - 1, pDataItem);
}
