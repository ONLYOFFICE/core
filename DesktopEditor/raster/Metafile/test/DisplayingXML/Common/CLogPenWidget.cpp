#include "CLogPenWidget.h"
#include "CPointWidget.h"
#include "CColorWidget.h"
#include "CFormWidget.h"

#include <QBoxLayout>
#include <QLabel>

CLogPenWidget::CLogPenWidget(QWidget *pParent) :
        CSharedWidget(pParent)
{
        QVBoxLayout *pLayout = new QVBoxLayout;

        QLabel *pName = new QLabel("LogPen:");
        pName->setStyleSheet("QLabel { text-decoration: underline; }");

        CFormWidget *pStyleWidget = new CFormWidget("PenStyle", "0");
        CPointWidget *pWidthWidget = new CPointWidget("Width");
        CColorWidget *pColorWidget = new CColorWidget("COLORREF");

        m_arWidgets.push_back(pStyleWidget);
        m_arWidgets.push_back(pWidthWidget);
        m_arWidgets.push_back(pColorWidget);

        pLayout->addWidget(pName);
        pLayout->addWidget(pStyleWidget);
        pLayout->addWidget(pWidthWidget);
        pLayout->addWidget(pColorWidget);

        setLayout(pLayout);
}

CLogPenWidget::~CLogPenWidget()
{

}

QList<QStandardItem *> CLogPenWidget::GetData() const
{
        QStandardItem *pItem = new QStandardItem("<LogPen>");

        for (const CSharedWidget* pWidget : m_arWidgets)
                pItem->appendRows(pWidget->GetData());

        return {pItem};
}
