#include "CSmallTextOutWidget.h"
#include "CRectangleWidget.h"
#include "CFormWidget.h"

#include <QBoxLayout>

CSmallTextOutWidget::CSmallTextOutWidget(QWidget *pParent)
{
        QVBoxLayout *pLayout = new QVBoxLayout();

        AddWidgets({new CFormWidget("x",                "0"),
                    new CFormWidget("y",                "0"),
                    new CFormWidget("fuOptions",        "0"),
                    new CFormWidget("iGraphicsMode",    "0"),
                    new CFormWidget("exScale",          "0"),
                    new CFormWidget("eyScale",          "0"),
                    new CRectangleWidget("Bounds"),
                    new CFormWidget("TextString")}, pLayout);

        setLayout(pLayout);
}

CSmallTextOutWidget::~CSmallTextOutWidget()
{

}
