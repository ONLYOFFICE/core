#include "CFormWidget.h"

#include "Common.h"

#include <QFormLayout>
#include <QTextEdit>
#include <QLabel>

CFormWidget::CFormWidget(const QString& qsName, const QString& qsDefaultValue, QWidget *pParent):
        CSharedWidget(pParent),
        m_qsName(qsName),
        m_pValue(NULL)
{
        QFormLayout *pLauout = new QFormLayout;
        pLauout->setMargin(0);

        m_pValue = new QTextEdit(qsDefaultValue);
        AlignTextEdit(m_pValue);

        pLauout->addRow(new QLabel(m_qsName + ":"), m_pValue);

        setLayout(pLauout);
        setFrameShape(QFrame::NoFrame);
}

CFormWidget::~CFormWidget()
{

}

QList<QStandardItem*> CFormWidget::GetData() const
{
        if (m_pValue->toPlainText().isEmpty())
                return {new QStandardItem(QString("<%1/>").arg(m_qsName))};
        else
                return {new QStandardItem(QString("<%1>%2</%1>").arg(m_qsName).arg(m_pValue->toPlainText()))};
}

QTextEdit *CFormWidget::GetTextEditWidget() const
{
        return m_pValue;
}
