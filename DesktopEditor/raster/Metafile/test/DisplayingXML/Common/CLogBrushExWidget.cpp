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

        pLayout->addWidget(pName);

        AddWidgets({new CFormWidget("BrushStyle", "0"),
                    new CColorWidget("Color"),
                    new CFormWidget("BrushHatch", "0")}, pLayout);

        setLayout(pLayout);
}

CLogBrushExWidget::~CLogBrushExWidget()
{

}

QList<QStandardItem *> CLogBrushExWidget::GetData() const
{
        QStandardItem *pItem = new QStandardItem("<LogBrush>");
        pItem->appendRows(CSharedWidget::GetData());
        return {pItem};
}
