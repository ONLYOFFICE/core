#include "CSizeWidget.h"
#include "CFormWidget.h"

#include <QBoxLayout>
#include <QLabel>

CSizeWidget::CSizeWidget(QWidget *pParent) :
        CSharedWidget(pParent)
{
        QHBoxLayout *pLayout = new QHBoxLayout;

        QLabel *pName = new QLabel("Size");
        pName->setStyleSheet("QLabel {text-decoration: underline; }");

        CFormWidget *pXForm = new CFormWidget("cx", "0");
        CFormWidget *pYForm = new CFormWidget("cy", "0");

        pLayout->addWidget(pName);
        pLayout->addWidget(pXForm);
        pLayout->addWidget(pYForm);

        m_arWidgets.push_back(pXForm);
        m_arWidgets.push_back(pYForm);

        setLayout(pLayout);
}

CSizeWidget::~CSizeWidget()
{

}

QList<QStandardItem *> CSizeWidget::GetData() const
{
        QStandardItem *pSize = new QStandardItem("<Extent>");
        pSize->appendRows(m_arWidgets[0]->GetData());
        pSize->appendRows(m_arWidgets[1]->GetData());

        return {pSize};
}
