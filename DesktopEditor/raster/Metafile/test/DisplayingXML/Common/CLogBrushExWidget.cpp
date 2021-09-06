#include "CLogBrushExWidget.h"
#include "CColorWidget.h"
#include "CFormWidget.h"

#include <QBoxLayout>
#include <QLabel>

CLogBrushExWidget::CLogBrushExWidget(QWidget *pParent)
        : CSharedWidget(pParent)
{
        QVBoxLayout *pLayout = new QVBoxLayout;

        QLabel *pName = new QLabel("LogBrush:");
        pName->setStyleSheet("QLabel { text-decoration: underline; }");

        CFormWidget *pStyle     = new CFormWidget("BrushStyle", "0");
        CColorWidget *pColor    = new CColorWidget("Color");
        CFormWidget *pHatch     = new CFormWidget("BrushHatch", "0");

        m_arWidgets.push_back(pStyle);
        m_arWidgets.push_back(pColor);
        m_arWidgets.push_back(pHatch);

        pLayout->addWidget(pName);
        pLayout->addWidget(pStyle);
        pLayout->addWidget(pColor);
        pLayout->addWidget(pHatch);

        setLayout(pLayout);
}

CLogBrushExWidget::~CLogBrushExWidget()
{

}

QList<QStandardItem *> CLogBrushExWidget::GetData() const
{
        QStandardItem *pItem = new QStandardItem("<LogBrush>");

        for (const CSharedWidget* pWidget : m_arWidgets)
                pItem->appendRows(pWidget->GetData());

        return {pItem};
}
