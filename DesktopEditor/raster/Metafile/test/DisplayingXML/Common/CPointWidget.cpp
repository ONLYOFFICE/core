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

        QString qsName = qsNameRecord;

        if (unIndex != 0)
                qsName += QString(" %1").arg(unIndex);

        qsName += ':';

        QLabel *pName = new QLabel(qsName);
        pName->setStyleSheet("QLabel {text-decoration: underline; }");

        CFormWidget *pXForm = new CFormWidget("x", "0");
        CFormWidget *pYForm = new CFormWidget("y", "0");

        pLayout->addWidget(pName);
        pLayout->addWidget(pXForm);
        pLayout->addWidget(pYForm);

        m_arWidgets.push_back(pXForm);
        m_arWidgets.push_back(pYForm);

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
        pPoint->appendRows(m_arWidgets[0]->GetData());
        pPoint->appendRows(m_arWidgets[1]->GetData());

        return {pPoint};
}
