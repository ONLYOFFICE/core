#include "CLogPaletteWidget.h"
#include "CColorWidget.h"

#include <QPushButton>
#include <QBoxLayout>
#include <QLabel>

CLogPaletteWidget::CLogPaletteWidget(QWidget *pParent)
        : CSharedWidget(pParent),
          m_unCounter(0)
{
        QVBoxLayout *pLayout = new QVBoxLayout;

        QLabel *pName = new QLabel("LogPallete");
        pName->setStyleSheet("QLabel { text-decoration: underline; }");

        QPushButton *pAddedButton = new QPushButton("Добавить цвет");

        connect(pAddedButton, &QPushButton::clicked, this, &CLogPaletteWidget::on_CreateColor_clicked);

        pLayout->addWidget(pName);
        pLayout->addWidget(pAddedButton);

        setLayout(pLayout);
}

CLogPaletteWidget::~CLogPaletteWidget()
{

}

QList<QStandardItem *> CLogPaletteWidget::GetData() const
{
        QStandardItem *pStandardItem = new QStandardItem("<LogPalette>");
        pStandardItem->appendRow(new QStandardItem("<Version>768</Version>"));
        pStandardItem->appendRows(CSharedWidget::GetData());
        return {pStandardItem};
}

void CLogPaletteWidget::on_CreateColor_clicked()
{
        CColorWidget *pColorWidget = new CColorWidget(QString("Color%1").arg(++m_unCounter));
        pColorWidget->setReverseOrder(true);

        m_arWidgets.push_back(pColorWidget);

        QBoxLayout *pMainLayout = (QBoxLayout*)layout();
        pMainLayout->insertWidget(pMainLayout->count() - 1, pColorWidget);
}
