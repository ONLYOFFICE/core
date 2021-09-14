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

        CFormWidget *pColorSpaceTypeWidget          = new CFormWidget("ColorSpaceType", "0");
        CFormWidget *pIntentWidget                  = new CFormWidget("Intent", "8");
        CCIEXYZTripleWidget *pCIEXYZTripleWidget    = new CCIEXYZTripleWidget();
        CFormWidget *pGammaRedWidget                = new CFormWidget("GammaRed", "1.0");
        CFormWidget *pGammaGreenWidget              = new CFormWidget("GammaGreen", "1.0");
        CFormWidget *pGammaBlueWidget               = new CFormWidget("GammaBlue", "1.0");
        CFormWidget *pFileNameWidget                = new CFormWidget("Filename ", "");

        m_arWidgets.push_back(pColorSpaceTypeWidget);
        m_arWidgets.push_back(pIntentWidget);
        m_arWidgets.push_back(pCIEXYZTripleWidget);
        m_arWidgets.push_back(pGammaRedWidget);
        m_arWidgets.push_back(pGammaGreenWidget);
        m_arWidgets.push_back(pGammaBlueWidget);
        m_arWidgets.push_back(pFileNameWidget);

        pLayout->addWidget(pName);
        pLayout->addWidget(pColorSpaceTypeWidget);
        pLayout->addWidget(pIntentWidget);
        pLayout->addWidget(pCIEXYZTripleWidget);
        pLayout->addWidget(pGammaRedWidget);
        pLayout->addWidget(pGammaGreenWidget);
        pLayout->addWidget(pGammaBlueWidget);
        pLayout->addWidget(pFileNameWidget);

        setLayout(pLayout);
}

CLogColorSpaceWidget::~CLogColorSpaceWidget()
{

}

QList<QStandardItem *> CLogColorSpaceWidget::GetData() const
{
        QStandardItem *pItem = new QStandardItem("<LogColorSpace>");

        for (const CSharedWidget *pWidget : m_arWidgets)
                pItem->appendRows(pWidget->GetData());

        return {pItem};
}
