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

        pLayout->addWidget(pName);

        AddWidgets({new CFormWidget("PenStyle", "0"),
                    new CPointWidget("Width"),
                    new CColorWidget("COLORREF")}, pLayout);

        setLayout(pLayout);
}

CLogPenWidget::~CLogPenWidget()
{

}

QList<QStandardItem *> CLogPenWidget::GetData() const
{
        QStandardItem *pItem = new QStandardItem("<LogPen>");
        pItem->appendRows(CSharedWidget::GetData());
        return {pItem};
}
