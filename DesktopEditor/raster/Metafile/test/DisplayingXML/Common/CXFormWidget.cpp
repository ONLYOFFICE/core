#include "CXFormWidget.h"
#include "CFormWidget.h"

#include <QBoxLayout>

CXFormWidget::CXFormWidget(QWidget *pParent)
        : CSharedWidget(pParent)
{
        QVBoxLayout *pLayout = new QVBoxLayout();

        AddWidgets({new CFormWidget("M11", "0"),
                    new CFormWidget("M12", "0"),
                    new CFormWidget("M21", "0"),
                    new CFormWidget("M22", "0"),
                    new CFormWidget("Dx",  "0"),
                    new CFormWidget("Dy",  "0")}, pLayout);

        setLayout(pLayout);
}

CXFormWidget::~CXFormWidget()
{

}

QList<QStandardItem *> CXFormWidget::GetData() const
{
        QStandardItem *pForm = new QStandardItem("<XForm>");
        pForm->appendRows(CSharedWidget::GetData());

        return {pForm};
}
