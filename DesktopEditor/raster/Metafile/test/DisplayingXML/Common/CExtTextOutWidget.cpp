/*
 * (c) Copyright Ascensio System SIA 2010-2024
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at 20A-6 Ernesta Birznieka-Upish
 * street, Riga, Latvia, EU, LV-1050.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */

#include "CExtTextOutWidget.h"
#include "CRectangleWidget.h"
#include "CPointWidget.h"
#include "CFormWidget.h"

#include <QPushButton>
#include <QHBoxLayout>
#include <QLabel>

CExtTextOutWidget::CExtTextOutWidget(QWidget *pParent)
        : CSharedWidget(pParent)
{
        QVBoxLayout *pLayout = new QVBoxLayout();

        AddWidgets({new CRectangleWidget("Bounds"),
                    new CFormWidget("iGraphicsMode",    "1"),
                    new CFormWidget("exScale",          "1"),
                    new CFormWidget("eyScale",          "1"),
                    new CEmrTextWidget()}, pLayout);

        setLayout(pLayout);
}

CExtTextOutWidget::~CExtTextOutWidget()
{

}

CEmrTextWidget::CEmrTextWidget(unsigned int unIndex, QWidget *pParent)
        : CSharedWidget(pParent),
          m_unIndex(unIndex),
          m_pTextWidget(NULL),
          m_pDxWidget(NULL)
{
        QVBoxLayout *pLayout = new QVBoxLayout();

        QLabel *pName = new QLabel(((m_unIndex == 0) ? "EmrText:" : QString("EmrText%1:").arg(m_unIndex)));
        pName->setStyleSheet("QLabel { text-decoration: underline; }");

        m_pTextWidget = new CFormWidget("Text");

        QPushButton *pCreateDxButton = new QPushButton("Создать поля для Dx значений");

        connect(pCreateDxButton, &QPushButton::clicked, this, &CEmrTextWidget::on_createDx_clicked);

        pLayout->addWidget(pName);

        AddWidgets({new CPointWidget("Reference"),
                    new CFormWidget("Options", "2"),
                    new CRectangleWidget("Rectangle"),
                    m_pTextWidget}, pLayout);

        pLayout->addWidget(pCreateDxButton);

        setLayout(pLayout);
}

CEmrTextWidget::~CEmrTextWidget()
{

}

QList<QStandardItem *> CEmrTextWidget::GetData() const
{
        QStandardItem *pItem = new QStandardItem(((m_unIndex == 0) ? "<EmrText>" : QString("<ExtText%1>").arg(m_unIndex)));
        pItem->appendRows(CSharedWidget::GetData());

        if (NULL != m_pDxWidget)
                pItem->appendRows(m_pDxWidget->GetData());

        return {pItem};
}

void CEmrTextWidget::on_createDx_clicked()
{
        if (NULL == m_pTextWidget)
                return;

        QString qsValue = m_pTextWidget->GetTextEditWidget()->toPlainText();

        if (qsValue.isEmpty())
                return;

        if (NULL != m_pDxWidget)
        {
                delete layout()->takeAt(layout()->indexOf(m_pDxWidget))->widget();
        }

        m_pDxWidget = new CDxWidget(qsValue);
        layout()->addWidget(m_pDxWidget);
}

CDxWidget::CDxWidget(const QString& qsValues, QWidget *pParent)
        : CSharedWidget(pParent)
{
        if (qsValues.isEmpty())
                return;

        QVBoxLayout *pLayout = new QVBoxLayout();

        QLabel *pName = new QLabel("Dx:");
        pName->setStyleSheet("QLabel { text-decoration: underline; }");

        pLayout->addWidget(pName);

        for (unsigned int unIndex = 0; unIndex < qsValues.length(); ++unIndex)
        {
                CFormWidget *pDxValue = new CFormWidget(QString("Dx%1").arg(unIndex + 1));
                AddWidget(pDxValue, pLayout);
        }

        setLayout(pLayout);
}

CDxWidget::~CDxWidget()
{

}

QList<QStandardItem *> CDxWidget::GetData() const
{
        QStandardItem *pItem = new QStandardItem("<Dx>");
        pItem->appendRows(CSharedWidget::GetData());
        return {pItem};
}
