#include "CSizeWidget.h"
#include "CFormWidget.h"

#include <QBoxLayout>
#include <QLabel>

CSizeWidget::CSizeWidget(const QString& qsName, QWidget *pParent) :
        CSharedWidget(pParent),
        m_qsName(qsName)
{
        QHBoxLayout *pLayout = new QHBoxLayout;

        QLabel *pName = new QLabel(m_qsName);
        pName->setStyleSheet("QLabel {text-decoration: underline; }");

        pLayout->addWidget(pName);

        AddWidgets({new CFormWidget("cx", "0"),
                    new CFormWidget("cy", "0")}, pLayout);

        setLayout(pLayout);
}

CSizeWidget::~CSizeWidget()
{

}

QList<QStandardItem *> CSizeWidget::GetData() const
{
        QStandardItem *pSize = new QStandardItem(QString("<%1>").arg(m_qsName));
        pSize->appendRows(CSharedWidget::GetData());

        return {pSize};
}
