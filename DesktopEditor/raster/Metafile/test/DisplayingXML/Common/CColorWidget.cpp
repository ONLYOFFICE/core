#include "CColorWidget.h"
#include "Common.h"

#include <QColorDialog>
#include <QPushButton>
#include <QLabel>

CColorWidget::CColorWidget(const QString& qsName, QWidget *pParent) :
        CSharedWidget(pParent),
        m_qsName(qsName),
        m_pColorLayout(NULL)
{
        QVBoxLayout *pLayout = new QVBoxLayout;

        QLabel *pName = new QLabel(QString("%1:").arg(m_qsName));
        pName->setStyleSheet("QLabel { text-decoration: underline; }");

        QPushButton *pSelectableButton = new QPushButton("Выбрать цвет");

        connect(pSelectableButton, &QPushButton::clicked, this, &CColorWidget::on_select_color_clicked);

        pLayout->addWidget(pName);
        pLayout->addWidget(pSelectableButton);

        setLayout(pLayout);
}

CColorWidget::~CColorWidget()
{

}

QList<QStandardItem *> CColorWidget::GetData() const
{
        if (!m_oColor.isValid())
                return {};

        QStandardItem *pItem = new QStandardItem(QString("<%1>").arg(m_qsName));
        pItem->appendRow(new QStandardItem(QString("<r>%1</r>").arg(m_oColor.red())));
        pItem->appendRow(new QStandardItem(QString("<g>%1</g>").arg(m_oColor.green())));
        pItem->appendRow(new QStandardItem(QString("<b>%1</b>").arg(m_oColor.blue())));
        pItem->appendRow(new QStandardItem("<a>0</a>"));

        return {pItem};
}

void CColorWidget::on_select_color_clicked()
{
        m_oColor = QColorDialog::getColor(Qt::white);

        if (!m_oColor.isValid())
                return;

        if (NULL == m_pColorLayout)
        {
                m_pColorLayout = new QHBoxLayout;
                ((QHBoxLayout*)layout())->addLayout(m_pColorLayout);
        }
        else
                ClearLayout(m_pColorLayout);

        QLabel *pRedValue       = new QLabel(QString("Red: %1").arg(m_oColor.red()));
        QLabel *pGreenValue     = new QLabel(QString("Green: %1").arg(m_oColor.green()));
        QLabel *pBlueValue      = new QLabel(QString("Blue: %1").arg(m_oColor.blue()));

        QFrame *pColorFrame = new QFrame;
        pColorFrame->setStyleSheet(QString("background-color: rgb(%1, %2, %3)").arg(m_oColor.red()).arg(m_oColor.green()).arg(m_oColor.blue()));

        m_pColorLayout->addWidget(pRedValue);
        m_pColorLayout->addWidget(pGreenValue);
        m_pColorLayout->addWidget(pBlueValue);
        m_pColorLayout->addWidget(pColorFrame);

}
