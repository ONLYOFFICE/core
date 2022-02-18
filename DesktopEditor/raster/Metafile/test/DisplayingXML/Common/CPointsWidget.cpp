#include "CPointsWidget.h"

#include "CPointWidget.h"
#include "CFormWidget.h"
#include "Common.h"

#include <QLabel>
#include <QTextEdit>
#include <QBoxLayout>
#include <QFormLayout>
#include <QPushButton>

CPointsWidget::CPointsWidget(QWidget *pParent):
        CSharedWidget(pParent),
        m_unCounter(0)
{
        QVBoxLayout *pLayout = new QVBoxLayout;

        QPushButton *pButtonAddPoint = new QPushButton("Добавить точку");

        connect(pButtonAddPoint, &QPushButton::clicked, this, &CPointsWidget::on_createPoint_clicked);

        pLayout->addWidget(pButtonAddPoint);

        setLayout(pLayout);
}

CPointsWidget::~CPointsWidget()
{

}

void CPointsWidget::on_createPoint_clicked()
{
        CPointWidget *pPoint = new CPointWidget("Point", ++m_unCounter);
        m_arWidgets.push_back(pPoint);

        QBoxLayout *pMainLayout = (QBoxLayout*)layout();
        pMainLayout->insertWidget(pMainLayout->count() - 1, pPoint);
}
