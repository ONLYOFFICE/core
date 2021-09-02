#include "CPointsWidget.h"

#include "CFormWidget.h"
#include "Common.h"

#include <QLabel>
#include <QTextEdit>
#include <QBoxLayout>
#include <QFormLayout>
#include <QPushButton>

CPointsWidget::CPointsWidget(unsigned int unIndex, QWidget *pParent):
        CSharedWidget(pParent),
        m_unIndex(unIndex),
        m_unCounter(0)
{
        QVBoxLayout *pLayout = new QVBoxLayout;

        QPushButton *pButtonAddPoint = new QPushButton("Add Point");

        connect(pButtonAddPoint, &QPushButton::clicked, this, &CPointsWidget::on_createPoint_clicked);

        pLayout->addWidget(pButtonAddPoint);

        setLayout(pLayout);
}

CPointsWidget::~CPointsWidget()
{

}

QList<QStandardItem*> CPointsWidget::GetData() const
{
        if (m_arWidgets.empty())
                return {};

        QList<QStandardItem*> oListItem;

        for (unsigned int unIndex = 0; unIndex < m_arWidgets.size();)
        {
                QStandardItem *pPoint = new QStandardItem(QString("<Point%1>").arg(unIndex / 2 + 1));
                pPoint->appendRows(m_arWidgets[unIndex++]->GetData());
                pPoint->appendRows(m_arWidgets[unIndex++]->GetData());

                oListItem.append(pPoint);
        }

        return oListItem;
}

void CPointsWidget::on_createPoint_clicked()
{
        ++m_unCounter;

        QHBoxLayout *pLayout = new QHBoxLayout;

        QLabel *pName = new QLabel(QString("Point %1: ").arg(m_unCounter));
        pName->setStyleSheet("QLabel {text-decoration: underline; }");

        CFormWidget *pXForm = new CFormWidget("x", "0");
        CFormWidget *pYForm = new CFormWidget("y", "0");

        pLayout->addWidget(pName);
        pLayout->addWidget(pXForm);
        pLayout->addWidget(pYForm);

        m_arWidgets.push_back(pXForm);
        m_arWidgets.push_back(pYForm);

        QBoxLayout *pMainLayout = (QBoxLayout*)layout();
        pMainLayout->insertLayout(pMainLayout->count() - 1, pLayout);
}
