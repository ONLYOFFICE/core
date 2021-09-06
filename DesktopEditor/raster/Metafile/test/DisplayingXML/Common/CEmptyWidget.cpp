#include "CEmptyWidget.h"

CEmptyWidget::CEmptyWidget(const QString& qsName, QWidget *pParent) :
        CSharedWidget(pParent),
        m_qsName(qsName)
{

}

CEmptyWidget::~CEmptyWidget()
{

}

QList<QStandardItem *> CEmptyWidget::GetData() const
{
        return {new QStandardItem(QString("<%1>").arg(m_qsName))};
}
