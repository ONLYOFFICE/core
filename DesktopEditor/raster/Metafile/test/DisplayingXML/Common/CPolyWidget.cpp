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

#include "CPolyWidget.h"

#include "CPointsWidget.h"

#include <QPushButton>
#include <QBoxLayout>
#include <QLabel>

CPolyWidget::CPolyWidget(QWidget *pParent):
        CSharedWidget(pParent),
        m_unCounter(0)
{
        QVBoxLayout *pLayout = new QVBoxLayout;

        QPushButton *pButtonAddPoly = new QPushButton("Add Poly");

        connect(pButtonAddPoly, &QPushButton::clicked, this, &CPolyWidget::on_createPoly_clicked);

        pLayout->addWidget(pButtonAddPoly);

        setLayout(pLayout);
}

CPolyWidget::~CPolyWidget()
{

}

QList<QStandardItem*> CPolyWidget::GetData() const
{
        if (m_arWidgets.empty())
                return {};

        QList<QStandardItem*> oPolyList;

        for (unsigned int unIndex = 0; unIndex < m_arWidgets.size(); ++unIndex)
        {
                QStandardItem *pPolyItem = new QStandardItem(QString("<PolygonPoint%1>").arg(unIndex + 1));
                pPolyItem->appendRows(m_arWidgets[unIndex]->GetData());
                oPolyList.append(pPolyItem);
        }

        return oPolyList;
}

void CPolyWidget::on_createPoly_clicked()
{
        QLabel *pName = new QLabel(QString("Poly %1:").arg(++m_unCounter));
        CPointsWidget *pPointsWiget = new CPointsWidget();

        m_arWidgets.push_back(pPointsWiget);

        QBoxLayout *pMainLayout = (QBoxLayout*)layout();
        pMainLayout->insertWidget(pMainLayout->count() - 1, pName);
        pMainLayout->insertWidget(pMainLayout->count() - 1, pPointsWiget);
}
