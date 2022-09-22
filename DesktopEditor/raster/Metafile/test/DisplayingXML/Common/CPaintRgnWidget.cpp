#include "CFormWidget.h"
#include "CPaintRgnWidget.h"
#include "CRectangleWidget.h"

#include <QBoxLayout>

CPaintRgnWidget::CPaintRgnWidget(QWidget *pParent)
        : CSharedWidget(pParent)
{
        QVBoxLayout *pLayout = new QVBoxLayout();

        AddWidgets({new CRectangleWidget("Bounds"),
                    new CFormWidget("RgnData")}, pLayout);

        setLayout(pLayout);
}

CPaintRgnWidget::~CPaintRgnWidget()
{

}
