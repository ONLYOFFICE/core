#include "CPolyTextOutWidget.h"
#include "CRectangleWidget.h"
#include "CExtTextOutWidget.h"

#include <QPushButton>
#include <QBoxLayout>

CPolyTextOutWidget::CPolyTextOutWidget(QWidget *pParent)
        : CSharedWidget(pParent),
          m_unCounter(0)
{
        QVBoxLayout *pLayout = new QVBoxLayout();

        AddWidgets({new CRectangleWidget("Bounds"),
                    new CFormWidget("iGraphicsMode", "1"),
                    new CFormWidget("exScale", "1"),
                    new CFormWidget("eyScale", "1")}, pLayout);

        QPushButton *pAddedButton = new QPushButton("Добавить объект текста");

        connect(pAddedButton, &QPushButton::clicked, this, &CPolyTextOutWidget::on_createTextOut_clicked);

        pLayout->addWidget(pAddedButton);

        setLayout(pLayout);
}

CPolyTextOutWidget::~CPolyTextOutWidget()
{

}

void CPolyTextOutWidget::on_createTextOut_clicked()
{
        CEmrTextWidget *pExtTextOutWidget = new CEmrTextWidget(++m_unCounter);
        m_arWidgets.push_back(pExtTextOutWidget);

        QBoxLayout *pMainLayout = (QBoxLayout*)layout();
        pMainLayout->insertWidget(pMainLayout->count() - 1, pExtTextOutWidget);
}
