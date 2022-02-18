#include "CFrameRgnWidget.h"
#include "CFormWidget.h"
#include "CRectangleWidget.h"

#include <QPushButton>
#include <QBoxLayout>

CFrameRgnWidget::CFrameRgnWidget(QWidget *pParent)
        : CSharedWidget(pParent),
          m_unCounter(0)
{
        QVBoxLayout *pLayout = new QVBoxLayout;

        QPushButton *pAddedButton = new QPushButton("Добавить значение");

        connect(pAddedButton, &QPushButton::clicked, this, &CFrameRgnWidget::on_createRgnData_clicked);

        AddWidgets({new CRectangleWidget("Bounds"),
                    new CFormWidget("ihBrush", "1"),
                    new CFormWidget("Width",   "1"),
                    new CFormWidget("Height ", "1")}, pLayout);

        pLayout->addWidget(pAddedButton);

        setLayout(pLayout);
}

CFrameRgnWidget::~CFrameRgnWidget()
{

}

void CFrameRgnWidget::on_createRgnData_clicked()
{
        CFormWidget *pDataItem = new CFormWidget(QString("RgnData%1").arg(++m_unCounter));
        m_arWidgets.push_back(pDataItem);

        QBoxLayout *pMainLayout = (QBoxLayout*)layout();
        pMainLayout->insertWidget(pMainLayout->count() - 1, pDataItem);
}
