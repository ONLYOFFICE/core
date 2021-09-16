#include "CFormWidget.h"
#include "CCIEXYZTripleWidget.h"
#include "CLogColorSpaceWidget.h"

#include <QBoxLayout>
#include <QLabel>

CLogColorSpaceWidget::CLogColorSpaceWidget(QWidget *pParent)
        : CSharedWidget(pParent)
{
        QVBoxLayout *pLayout = new QVBoxLayout();

        QLabel *pName = new QLabel("LogColorSpace:");
        pName->setStyleSheet("QLabel { text-decoration: underline; }");

        pLayout->addWidget(pName);

        AddWidgets({new CFormWidget("ColorSpaceType",   "0"),
                    new CFormWidget("Intent",           "8"),
                    new CCIEXYZTripleWidget(),
                    new CFormWidget("GammaRed",         "1.0"),
                    new CFormWidget("GammaGreen",       "1.0"),
                    new CFormWidget("GammaBlue",        "1.0"),
                    new CFormWidget("Filename")}, pLayout);

        setLayout(pLayout);
}

CLogColorSpaceWidget::~CLogColorSpaceWidget()
{

}

QList<QStandardItem *> CLogColorSpaceWidget::GetData() const
{
        QStandardItem *pItem = new QStandardItem("<LogColorSpace>");
        pItem->appendRows(CSharedWidget::GetData());
        return {pItem};
}
