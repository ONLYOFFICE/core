#include "CPointWidget.h"
#include "CFormWidget.h"

#include <QBoxLayout>
#include <QLabel>

CPointWidget::CPointWidget(const QString& qsNameRecord, unsigned int unIndex, QWidget *pParent):
        CSharedWidget(pParent),
        m_unIndex(unIndex),
        m_qsNameRecord(qsNameRecord)
{
        QHBoxLayout *pLayout = new QHBoxLayout;

        QLabel *pName = new QLabel((unIndex > 0) ? QString("%1%2:").arg(qsNameRecord).arg(unIndex) : QString("%1:").arg(qsNameRecord));
        pName->setStyleSheet("QLabel {text-decoration: underline; }");

        pLayout->addWidget(pName);

        AddWidgets({new CFormWidget("x", "0"),
                    new CFormWidget("y", "0")}, pLayout);

        setLayout(pLayout);
}

CPointWidget::~CPointWidget()
{

}

QList<QStandardItem *> CPointWidget::GetData() const
{
        QStandardItem *pPoint = new QStandardItem(((m_unIndex == 0)
                                                   ? QString("<%1>").arg(m_qsNameRecord)
                                                   : QString("<%1%2>").arg(m_qsNameRecord).arg(m_unIndex)));
        pPoint->appendRows(CSharedWidget::GetData());
        return {pPoint};
}
