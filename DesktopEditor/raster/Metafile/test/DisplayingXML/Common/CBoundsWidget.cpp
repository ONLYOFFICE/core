#include "CBoundsWidget.h"

#include "CFormWidget.h"
#include "Common.h"

#include <QFormLayout>
#include <QBoxLayout>
#include <QTextEdit>
#include <QLabel>

CBoundsWidget::CBoundsWidget(QWidget *pParent) :
        CSharedWidget(pParent)
{
        QGridLayout *pLayout = new QGridLayout;

        QLabel *pName = new QLabel("Bounds:");
        pName->setStyleSheet("QLabel { text-decoration: underline; }");

        CFormWidget *pLeftWidget    = new CFormWidget("Left",   "0");
        CFormWidget *pTopWidget     = new CFormWidget("Top",    "0");
        CFormWidget *pRightWidget   = new CFormWidget("Right",  "0");
        CFormWidget *pBottomWidget  = new CFormWidget("Bottom", "0");

        pLayout->addWidget(pName);

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

CBoundsWidget::~CBoundsWidget()
{

}

QList<QStandardItem*> CBoundsWidget::GetData() const
{
        QStandardItem *pStandardItem = new QStandardItem("<Bounds>");
        ConvertWidgets(pStandardItem);
        return {pStandardItem};
}
