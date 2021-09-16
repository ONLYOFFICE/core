#include "CPixelFormatDescriptorWidget.h"
#include "CFormWidget.h"

#include <QBoxLayout>
#include <QLabel>

CPixelFormatDescriptorWidget::CPixelFormatDescriptorWidget(QWidget *pParent)
        : CSharedWidget(pParent)
{
        QVBoxLayout *pLayout = new QVBoxLayout();

        QLabel *pName = new QLabel("PixelFormatDescriptor:");
        pName->setStyleSheet("QLabel { text-decoration: underline; }");

        pLayout->addWidget(pName);

        AddWidgets({new CFormWidget("dwFlags",          "0"),
                    new CFormWidget("iPixelType",       "0"),
                    new CFormWidget("cColorBits",       "0"),
                    new CFormWidget("cRedBits",         "0"),
                    new CFormWidget("cRedShift",        "0"),
                    new CFormWidget("cGreenBits",       "0"),
                    new CFormWidget("cGreenShift",      "0"),
                    new CFormWidget("cBlueBits",        "0"),
                    new CFormWidget("cBlueShift",       "0"),
                    new CFormWidget("cAlphaBits",       "0"),
                    new CFormWidget("cAlphaShift",      "0"),
                    new CFormWidget("cAccumBits",       "0"),
                    new CFormWidget("cAccumRedBits",    "0"),
                    new CFormWidget("cAccumGreenBits",  "0"),
                    new CFormWidget("cAccumBlueBits",   "0"),
                    new CFormWidget("cAccumAlphaBits",  "0"),
                    new CFormWidget("cDepthBits",       "0"),
                    new CFormWidget("cStencilBits",     "0"),
                    new CFormWidget("cAuxBuffers",      "0"),
                    new CFormWidget("bReserved",        "0"),
                    new CFormWidget("dwVisibleMask",    "0")}, pLayout);

        setLayout(pLayout);
}

CPixelFormatDescriptorWidget::~CPixelFormatDescriptorWidget()
{

}

QList<QStandardItem *> CPixelFormatDescriptorWidget::GetData() const
{
        QStandardItem *pItem = new QStandardItem("<PixelFormatDescriptor>");
        pItem->appendRows(CSharedWidget::GetData());
        return {pItem};
}
