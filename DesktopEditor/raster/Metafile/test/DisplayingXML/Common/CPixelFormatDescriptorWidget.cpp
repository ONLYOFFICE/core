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

        CFormWidget *pDwFlagsWidget         = new CFormWidget("dwFlags", "0");
        CFormWidget *pIPixelTypeWidget      = new CFormWidget("iPixelType", "0");
        CFormWidget *pCColorBitsWidget      = new CFormWidget("cColorBits", "0");
        CFormWidget *pCRedBitsWidget        = new CFormWidget("cRedBits", "0");
        CFormWidget *pCRedShiftWidget       = new CFormWidget("cRedShift", "0");
        CFormWidget *pCGreenBitsWidget      = new CFormWidget("cGreenBits", "0");
        CFormWidget *pCGreenShiftWidget     = new CFormWidget("cGreenShift", "0");
        CFormWidget *pCBlueBitsWidget       = new CFormWidget("cBlueBits", "0");
        CFormWidget *pCBlueShiftWidget      = new CFormWidget("cBlueShift", "0");
        CFormWidget *pCAlphaBitsWidget      = new CFormWidget("cAlphaBits", "0");
        CFormWidget *pCAlphaShiftWidget     = new CFormWidget("cAlphaShift", "0");
        CFormWidget *pCAccumBitsWidget      = new CFormWidget("cAccumBits", "0");
        CFormWidget *pCAccumRedBitsWidget   = new CFormWidget("cAccumRedBits", "0");
        CFormWidget *pCAccumGreenBitsWidget = new CFormWidget("cAccumGreenBits", "0");
        CFormWidget *pCAccumBlueBitsWidget  = new CFormWidget("cAccumBlueBits", "0");
        CFormWidget *pCAccumAlphaBitsWidget = new CFormWidget("cAccumAlphaBits", "0");
        CFormWidget *pCDepthBitsWidget      = new CFormWidget("cDepthBits", "0");
        CFormWidget *pCStencilBitsWidget    = new CFormWidget("cStencilBits", "0");
        CFormWidget *pCAuxBuffersWidget     = new CFormWidget("cAuxBuffers", "0");
        CFormWidget *pBReservedWidget       = new CFormWidget("bReserved", "0");
        CFormWidget *pDwVisibleMaskWidget   = new CFormWidget("dwVisibleMask", "0");

        m_arWidgets.push_back(pDwFlagsWidget);
        m_arWidgets.push_back(pIPixelTypeWidget);
        m_arWidgets.push_back(pCColorBitsWidget);
        m_arWidgets.push_back(pCRedBitsWidget);
        m_arWidgets.push_back(pCRedShiftWidget);
        m_arWidgets.push_back(pCGreenBitsWidget);
        m_arWidgets.push_back(pCGreenShiftWidget);
        m_arWidgets.push_back(pCBlueBitsWidget);
        m_arWidgets.push_back(pCBlueShiftWidget);
        m_arWidgets.push_back(pCAlphaBitsWidget);
        m_arWidgets.push_back(pCAlphaShiftWidget);
        m_arWidgets.push_back(pCAccumBitsWidget);
        m_arWidgets.push_back(pCAccumRedBitsWidget);
        m_arWidgets.push_back(pCAccumGreenBitsWidget);
        m_arWidgets.push_back(pCAccumBlueBitsWidget);
        m_arWidgets.push_back(pCAccumAlphaBitsWidget);
        m_arWidgets.push_back(pCDepthBitsWidget);
        m_arWidgets.push_back(pCStencilBitsWidget);
        m_arWidgets.push_back(pCAuxBuffersWidget);
        m_arWidgets.push_back(pBReservedWidget);
        m_arWidgets.push_back(pDwVisibleMaskWidget);

        pLayout->addWidget(pName);
        pLayout->addWidget(pDwFlagsWidget);
        pLayout->addWidget(pIPixelTypeWidget);
        pLayout->addWidget(pCColorBitsWidget);
        pLayout->addWidget(pCRedBitsWidget);
        pLayout->addWidget(pCRedShiftWidget);
        pLayout->addWidget(pCGreenBitsWidget);
        pLayout->addWidget(pCGreenShiftWidget);
        pLayout->addWidget(pCBlueBitsWidget);
        pLayout->addWidget(pCBlueShiftWidget);
        pLayout->addWidget(pCAlphaBitsWidget);
        pLayout->addWidget(pCAlphaShiftWidget);
        pLayout->addWidget(pCAccumBitsWidget);
        pLayout->addWidget(pCAccumRedBitsWidget);
        pLayout->addWidget(pCAccumGreenBitsWidget);
        pLayout->addWidget(pCAccumBlueBitsWidget);
        pLayout->addWidget(pCAccumAlphaBitsWidget);
        pLayout->addWidget(pCDepthBitsWidget);
        pLayout->addWidget(pCStencilBitsWidget);
        pLayout->addWidget(pCAuxBuffersWidget);
        pLayout->addWidget(pBReservedWidget);
        pLayout->addWidget(pDwVisibleMaskWidget);

        setLayout(pLayout);
}

CPixelFormatDescriptorWidget::~CPixelFormatDescriptorWidget()
{

}

QList<QStandardItem *> CPixelFormatDescriptorWidget::GetData() const
{
        QStandardItem *pItem = new QStandardItem("<PixelFormatDescriptor>");

        for (const CSharedWidget* pWidget : m_arWidgets)
                pItem->appendRows(pWidget->GetData());

        return {pItem};
}
