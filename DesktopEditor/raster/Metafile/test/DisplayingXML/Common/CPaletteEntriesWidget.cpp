#include "CPaletteEntriesWidget.h"
#include "CColorWidget.h"
#include "CFormWidget.h"

#include <QPushButton>
#include <QBoxLayout>

CPaletteEntriesWidget::CPaletteEntriesWidget(QWidget *pParent)
        : CSharedWidget(pParent),
          m_unCounter(0)
{
        QVBoxLayout *pLayout = new QVBoxLayout;

        AddWidgets({new CFormWidget("ihPal", "1"),
                    new CFormWidget("Start", "1")}, pLayout);

        QPushButton *pAddedButton = new QPushButton("Добавить цвет");

        connect(pAddedButton, &QPushButton::clicked, this, &CPaletteEntriesWidget::on_CreateColor_clicked);

        pLayout->addWidget(pAddedButton);

        setLayout(pLayout);
}

CPaletteEntriesWidget::~CPaletteEntriesWidget()
{

}

void CPaletteEntriesWidget::on_CreateColor_clicked()
{
        CColorWidget *pColorWidget = new CColorWidget(QString("Color%1").arg(++m_unCounter));
        pColorWidget->setReverseOrder(true);

        m_arWidgets.push_back(pColorWidget);

        QBoxLayout *pMainLayout = (QBoxLayout*)layout();
        pMainLayout->insertWidget(pMainLayout->count() - 1, pColorWidget);
}
