#include "CCIEXYZTripleWidget.h"
#include "CFormWidget.h"

#include <QBoxLayout>
#include <QLabel>

CCIEXYZTripleWidget::CCIEXYZTripleWidget(QWidget *pParent)
        : CSharedWidget(pParent)
{
        QVBoxLayout *pLayout = new QVBoxLayout();

        QLabel *pName = new QLabel("CIEXYZTriple");
        pName->setStyleSheet("QLabel { text-decoration: underline; }");

        CIEXYZWidget *pRedWidget = new CIEXYZWidget("ciexyzRed");
        CIEXYZWidget *pGreenWidget = new CIEXYZWidget("ciexyzGreen");
        CIEXYZWidget *pBlueWidget = new CIEXYZWidget("ciexyzBlue");

        m_arWidgets.push_back(pRedWidget);
        m_arWidgets.push_back(pGreenWidget);
        m_arWidgets.push_back(pBlueWidget);

        pLayout->addWidget(pName);
        pLayout->addWidget(pRedWidget);
        pLayout->addWidget(pGreenWidget);
        pLayout->addWidget(pBlueWidget);

        setLayout(pLayout);
}

CCIEXYZTripleWidget::~CCIEXYZTripleWidget()
{

}

QList<QStandardItem *> CCIEXYZTripleWidget::GetData() const
{
        QStandardItem *pItem = new QStandardItem("<CIEXYZTriple>");

        for (const CSharedWidget* pWidget : m_arWidgets)
                pItem->appendRows(pWidget->GetData());

        return {pItem};
}

CIEXYZWidget::CIEXYZWidget(const QString& qsName, QWidget *pParent)
        : CSharedWidget(pParent),
          m_qsName(qsName)
{
        QHBoxLayout *pLayout = new QHBoxLayout();

        QLabel *pName = new QLabel(((m_qsName.isEmpty()) ? "" : QString("%1:").arg(m_qsName)));
        pName->setStyleSheet("QLabel { text-decoration: underline; }");

        CFormWidget *pXWidget = new CFormWidget("ciexyzX", "1.0");
        CFormWidget *pYWidget = new CFormWidget("ciexyzY", "1.0");
        CFormWidget *pZWidget = new CFormWidget("ciexyzZ", "1.0");

        m_arWidgets.push_back(pXWidget);
        m_arWidgets.push_back(pYWidget);
        m_arWidgets.push_back(pZWidget);

        pLayout->addWidget(pName);
        pLayout->addWidget(pXWidget);
        pLayout->addWidget(pYWidget);
        pLayout->addWidget(pZWidget);

        setLayout(pLayout);
}

CIEXYZWidget::~CIEXYZWidget()
{

}

QList<QStandardItem *> CIEXYZWidget::GetData() const
{
        QStandardItem *pItem = new QStandardItem(((m_qsName.isEmpty()) ? "<CIEXYZ>" : QString("<%1>").arg(m_qsName)));

        for (const CSharedWidget* pWidget : m_arWidgets)
                pItem->appendRows(pWidget->GetData());

        return {pItem};
}
