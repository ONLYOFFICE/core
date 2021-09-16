#include "CRectangleWidget.h"

#include "CFormWidget.h"
#include "Common.h"

#include <QFormLayout>
#include <QBoxLayout>
#include <QTextEdit>
#include <QLabel>

CRectangleWidget::CRectangleWidget(const QString& qsName, QWidget *pParent) :
        CSharedWidget(pParent),
        m_qsName(qsName)
{
        QGridLayout *pLayout = new QGridLayout;

        QLabel *pName = new QLabel(QString("%1:").arg(m_qsName));
        pName->setStyleSheet("QLabel { text-decoration: underline; }");

        pLayout->addWidget(pName);

        CFormWidget *pLeftWidget    = new CFormWidget("Left",   "0");
        CFormWidget *pTopWidget     = new CFormWidget("Top",    "0");
        CFormWidget *pRightWidget   = new CFormWidget("Right",  "0");
        CFormWidget *pBottomWidget  = new CFormWidget("Bottom", "0");

        pLayout->addWidget(pLeftWidget,   1, 0);
        pLayout->addWidget(pTopWidget,    2, 0);
        pLayout->addWidget(pRightWidget,  1, 1);
        pLayout->addWidget(pBottomWidget, 2, 1);

        m_arWidgets.push_back(pLeftWidget);
        m_arWidgets.push_back(pTopWidget);
        m_arWidgets.push_back(pRightWidget);
        m_arWidgets.push_back(pBottomWidget);

        setLayout(pLayout);
}

CRectangleWidget::~CRectangleWidget()
{

}

QList<QStandardItem*> CRectangleWidget::GetData() const
{
        QStandardItem *pStandardItem = new QStandardItem(QString("<%1>").arg(m_qsName));
        pStandardItem->appendRows(CSharedWidget::GetData());
        return {pStandardItem};
}
