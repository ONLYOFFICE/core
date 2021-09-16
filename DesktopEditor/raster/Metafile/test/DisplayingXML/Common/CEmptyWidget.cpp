#include "CEmptyWidget.h"

#include <QLabel>
#include <QBoxLayout>

CEmptyWidget::CEmptyWidget(const QString& qsName, QWidget *pParent) :
        CSharedWidget(pParent),
        m_qsName(qsName)
{
        QVBoxLayout *pLayout = new QVBoxLayout();
        pLayout->addWidget(new QLabel("Запись не содержит параметров"));
        setLayout(pLayout);
}

CEmptyWidget::~CEmptyWidget()
{

}

QList<QStandardItem *> CEmptyWidget::GetData() const
{
        return {new QStandardItem(QString("<%1>").arg(m_qsName))};
}
