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

        pLayout->addWidget(pName);
        AddWidgets({new CIEXYZWidget("ciexyzRed"),
                    new CIEXYZWidget("ciexyzGreen"),
                    new CIEXYZWidget("ciexyzBlue")}, pLayout);

        setLayout(pLayout);
}

CCIEXYZTripleWidget::~CCIEXYZTripleWidget()
{

}

QList<QStandardItem *> CCIEXYZTripleWidget::GetData() const
{
        QStandardItem *pItem = new QStandardItem("<CIEXYZTriple>");
        pItem->appendRows(CSharedWidget::GetData());
        return {pItem};
}

CIEXYZWidget::CIEXYZWidget(const QString& qsName, QWidget *pParent)
        : CSharedWidget(pParent),
          m_qsName(qsName)
{
        QHBoxLayout *pLayout = new QHBoxLayout();

        QLabel *pName = new QLabel(((m_qsName.isEmpty()) ? "" : QString("%1:").arg(m_qsName)));
        pName->setStyleSheet("QLabel { text-decoration: underline; }");

        pLayout->addWidget(pName);
        AddWidgets({new CFormWidget("ciexyzX", "1.0"),
                    new CFormWidget("ciexyzY", "1.0"),
                    new CFormWidget("ciexyzZ", "1.0")}, pLayout);

        setLayout(pLayout);
}

CIEXYZWidget::~CIEXYZWidget()
{

}

QList<QStandardItem *> CIEXYZWidget::GetData() const
{
        QStandardItem *pItem = new QStandardItem(((m_qsName.isEmpty()) ? "<CIEXYZ>" : QString("<%1>").arg(m_qsName)));
        pItem->appendRows(CSharedWidget::GetData());
        return {pItem};
}
