/*
 * Copyright (C) Ascensio System SIA, 2009-2026
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation, together with the
 * additional terms provided in the LICENSE file.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For
 * details, see the GNU AGPL at: https://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA by email at info@onlyoffice.com
 * or by postal mail at 20A-6 Ernesta Birznieka-Upisha Street, Riga,
 * LV-1050, Latvia, European Union.
 *
 * The interactive user interfaces in modified versions of the Program
 * are required to display Appropriate Legal Notices in accordance with
 * Section 5 of the GNU AGPL version 3.
 *
 * No trademark rights are granted under this License.
 *
 * All non-code elements of the Product, including illustrations,
 * icon sets, and technical writing content, are licensed under the
 * Creative Commons Attribution-ShareAlike 4.0 International License:
 * https://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 * This license applies only to such non-code elements and does not
 * modify or replace the licensing terms applicable to the Program's
 * source code, which remains licensed under the GNU Affero General
 * Public License v3.
 *
 * SPDX-License-Identifier: AGPL-3.0-only
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

        QPushButton *pCreateDxButton = new QPushButton("Create fields for Dx values");

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
