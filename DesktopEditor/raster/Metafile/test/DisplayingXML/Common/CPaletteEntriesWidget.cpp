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

        CFormWidget *pIhPalWidget = new CFormWidget("ihPal", "1");
        CFormWidget *pStartWidget = new CFormWidget("Start", "1");

        m_arWidgets.push_back(pIhPalWidget);
        m_arWidgets.push_back(pStartWidget);

        QPushButton *pAddedButton = new QPushButton("Добавить цвет");

        connect(pAddedButton, &QPushButton::clicked, this, &CPaletteEntriesWidget::on_CreateColor_clicked);

        pLayout->addWidget(pIhPalWidget);
        pLayout->addWidget(pStartWidget);
        pLayout->addWidget(pAddedButton);

        setLayout(pLayout);
}

CPaletteEntriesWidget::~CPaletteEntriesWidget()
{

}

QList<QStandardItem *> CPaletteEntriesWidget::GetData() const
{
        QList<QStandardItem*> arDatas;

        for (const CSharedWidget* oWidget : m_arWidgets)
                arDatas.append(oWidget->GetData());

        return arDatas;
}

void CPaletteEntriesWidget::on_CreateColor_clicked()
{
        CColorWidget *pColorWidget = new CColorWidget(QString("Color%1").arg(++m_unCounter));
        pColorWidget->setReverseOrder(true);

        m_arWidgets.push_back(pColorWidget);

        QBoxLayout *pMainLayout = (QBoxLayout*)layout();
        pMainLayout->insertWidget(pMainLayout->count() - 1, pColorWidget);
}
