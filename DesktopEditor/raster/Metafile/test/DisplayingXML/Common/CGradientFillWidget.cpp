#include "CGradientFillWidget.h"
#include "CRectangleWidget.h"
#include "Common.h"

#include <QPushButton>
#include <QLabel>

CGradientFillWidget::CGradientFillWidget(QWidget *pParent)
        : CSharedWidget(pParent),
          m_pNVerWidget(NULL),
          m_pNTriWidget(NULL),
          m_pUlModeWidget(NULL),
          m_pDataLayout(NULL)
{
        QVBoxLayout *pLayout = new QVBoxLayout();

        CRectangleWidget *pBoundsWidget = new CRectangleWidget("Bounds");
        m_pNVerWidget                   = new CFormWidget("nVer", "0");
        m_pNTriWidget                   = new CFormWidget("nTri", "0");
        m_pUlModeWidget                 = new CFormWidget("ulMode", "0");

        QPushButton *pCreateObjectsButton = new QPushButton("Создать объекты!");

        connect(pCreateObjectsButton, &QPushButton::clicked, this, &CGradientFillWidget::on_createObjects_clicked);

        m_arWidgets.push_back(pBoundsWidget);
        m_arWidgets.push_back(m_pNVerWidget);
        m_arWidgets.push_back(m_pNTriWidget);
        m_arWidgets.push_back(m_pUlModeWidget);

        pLayout->addWidget(pBoundsWidget);
        pLayout->addWidget(m_pNVerWidget);
        pLayout->addWidget(m_pNTriWidget);
        pLayout->addWidget(m_pUlModeWidget);
        pLayout->addWidget(pCreateObjectsButton);

        m_pDataLayout = new QVBoxLayout();

        pLayout->addLayout(m_pDataLayout);

        setLayout(pLayout);
}

CGradientFillWidget::~CGradientFillWidget()
{

}

QList<QStandardItem *> CGradientFillWidget::GetData() const
{
        QList<QStandardItem *> arData;

        for (const CSharedWidget *pWidget : m_arWidgets)
                arData.append(pWidget->GetData());

        return arData;
}

void CGradientFillWidget::on_createObjects_clicked()
{
        QString qsNVerValue = m_pNVerWidget->GetTextEditWidget()->toPlainText();
        QString qsNTriValue = m_pNTriWidget->GetTextEditWidget()->toPlainText();
        QString qsUlModeValue = m_pUlModeWidget->GetTextEditWidget()->toPlainText();

        if (qsNVerValue.isEmpty() || qsNTriValue.isEmpty() || qsUlModeValue.isEmpty())
                return;

        unsigned int unNVerValue   = qsNVerValue.toInt();
        unsigned int unNTriValue   = qsNTriValue.toInt();
        unsigned int unUlModeValue = qsUlModeValue.toInt();

        if (unNVerValue == 0 || unNTriValue == 0)
                return;

        ClearLayout(m_pDataLayout);

        while (m_arWidgets.size() > 4)
                m_arWidgets.pop_back();

        for (unsigned int unIndex = 0; unIndex < unNVerValue;)
        {
                CTriVertexWidget *pTriVertexWidget = new CTriVertexWidget(++unIndex);
                m_arWidgets.push_back(pTriVertexWidget);
                m_pDataLayout->addWidget(pTriVertexWidget);
        }

        for (unsigned int unIndex = 0; unIndex < unNTriValue;)
        {
                CSharedWidget *pGradientWidget;
                if (unUlModeValue == 2)
                        pGradientWidget = new CGradientTriangleWidget(++unIndex);
                else
                        pGradientWidget = new CGradientRectangleWidget(++unIndex);

                m_arWidgets.push_back(pGradientWidget);
                m_pDataLayout->addWidget(pGradientWidget);
        }

}

CTriVertexWidget::CTriVertexWidget(unsigned int unIndex, QWidget *pParent)
        : CSharedWidget(pParent),
          m_unIndex(unIndex)
{
        QVBoxLayout *pLayout = new QVBoxLayout();

        QLabel *pName = new QLabel(((m_unIndex > 0) ? QString("TriVertex%1:").arg(m_unIndex) : "TriVertex:"));
        pName->setStyleSheet("QLabel { text-decoration: underline; }");

        CFormWidget *pXWidget       = new CFormWidget("x", "0");
        CFormWidget *pYWidget       = new CFormWidget("y", "0");
        CFormWidget *pRedWidget     = new CFormWidget("Red", "0");
        CFormWidget *pGreenWidget   = new CFormWidget("Green", "0");
        CFormWidget *pBlueWidget    = new CFormWidget("Blue", "0");
        CFormWidget *pAlphaWidget   = new CFormWidget("Alpha", "0");

        m_arWidgets.push_back(pXWidget);
        m_arWidgets.push_back(pYWidget);
        m_arWidgets.push_back(pRedWidget);
        m_arWidgets.push_back(pGreenWidget);
        m_arWidgets.push_back(pBlueWidget);
        m_arWidgets.push_back(pAlphaWidget);

        pLayout->addWidget(pName);
        pLayout->addWidget(pXWidget);
        pLayout->addWidget(pYWidget);
        pLayout->addWidget(pRedWidget);
        pLayout->addWidget(pGreenWidget);
        pLayout->addWidget(pBlueWidget);
        pLayout->addWidget(pAlphaWidget);

        setLayout(pLayout);
}

CTriVertexWidget::~CTriVertexWidget()
{

}

QList<QStandardItem *> CTriVertexWidget::GetData() const
{
        QStandardItem *pItem = new QStandardItem(((m_unIndex > 0) ? QString("<TriVertex%1>").arg(m_unIndex) : "<TriVertex>"));

        for (const CSharedWidget* pWidget : m_arWidgets)
                pItem->appendRows(pWidget->GetData());

        return {pItem};
}

CGradientRectangleWidget::CGradientRectangleWidget(unsigned int unIndex, QWidget *pParent)
        : CSharedWidget(pParent),
          m_unIndex(unIndex)
{
        QHBoxLayout *pLayout = new QHBoxLayout();

        QLabel *pName = new QLabel(((m_unIndex > 0) ? QString("GradientRectangle%1:").arg(m_unIndex) : "GradientRectangle:"));
        pName->setStyleSheet("QLabel { text-decoration: underline; }");

        CFormWidget *pUpperLeftWidget  = new CFormWidget("UpperLeft", "0");
        CFormWidget *pLowerRightWidget = new CFormWidget("LowerRight", "0");

        m_arWidgets.push_back(pUpperLeftWidget);
        m_arWidgets.push_back(pLowerRightWidget);

        pLayout->addWidget(pName);
        pLayout->addWidget(pUpperLeftWidget);
        pLayout->addWidget(pLowerRightWidget);

        setLayout(pLayout);
}

CGradientRectangleWidget::~CGradientRectangleWidget()
{

}

QList<QStandardItem *> CGradientRectangleWidget::GetData() const
{
        QStandardItem *pItem = new QStandardItem(((m_unIndex > 0) ? QString("<GradientRectangle%1>").arg(m_unIndex) : "<GradientRectangle>"));

        for (const CSharedWidget* pWidget : m_arWidgets)
                pItem->appendRows(pWidget->GetData());

        return {pItem};
}

CGradientTriangleWidget::CGradientTriangleWidget(unsigned int unIndex, QWidget *pParent)
        : CSharedWidget(pParent),
          m_unIndex(unIndex)
{
        QHBoxLayout *pLayout = new QHBoxLayout();

        QLabel *pName = new QLabel(((m_unIndex > 0) ? QString("GradientTriangle%1:").arg(m_unIndex) : "GradientTriangle:"));
        pName->setStyleSheet("QLabel { text-decoration: underline; }");

        CFormWidget *pVertex1Widget  = new CFormWidget("Vertex1", "0");
        CFormWidget *pVertex2Widget  = new CFormWidget("Vertex2", "0");
        CFormWidget *pVertex3Widget  = new CFormWidget("Vertex3", "0");

        m_arWidgets.push_back(pVertex1Widget);
        m_arWidgets.push_back(pVertex2Widget);
        m_arWidgets.push_back(pVertex3Widget);

        pLayout->addWidget(pName);
        pLayout->addWidget(pVertex1Widget);
        pLayout->addWidget(pVertex2Widget);
        pLayout->addWidget(pVertex3Widget);

        setLayout(pLayout);
}

CGradientTriangleWidget::~CGradientTriangleWidget()
{

}

QList<QStandardItem *> CGradientTriangleWidget::GetData() const
{
        QStandardItem *pItem = new QStandardItem(((m_unIndex > 0) ? QString("<GradientTriangle%1>").arg(m_unIndex) : "<GradientTriangle>"));

        for (const CSharedWidget* pWidget : m_arWidgets)
                pItem->appendRows(pWidget->GetData());

        return {pItem};
}
