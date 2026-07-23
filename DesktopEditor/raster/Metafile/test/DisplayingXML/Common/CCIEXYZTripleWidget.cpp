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

#include "CCIEXYZTripleWidget.h"
#include "CFormWidget.h"

#include <QBoxLayout>
#include <QLabel>

CCIEXYZTripleWidget::CCIEXYZTripleWidget(QWidget *pParent)
        : CSharedWidget(pParent)
{
        QVBoxLayout *pLayout = new QVBoxLayout();

        QLabel *pName = new QLabel("CIEXYZTriple");
        pName->setStyleSheet("QLabel { text-decoration: underline; }");

        pLayout->addWidget(pName);
        AddWidgets({new CIEXYZWidget("ciexyzRed"),
                    new CIEXYZWidget("ciexyzGreen"),
                    new CIEXYZWidget("ciexyzBlue")}, pLayout);

        setLayout(pLayout);
}

CCIEXYZTripleWidget::~CCIEXYZTripleWidget()
{

}

QList<QStandardItem *> CCIEXYZTripleWidget::GetData() const
{
        QStandardItem *pItem = new QStandardItem("<CIEXYZTriple>");
        pItem->appendRows(CSharedWidget::GetData());
        return {pItem};
}

CIEXYZWidget::CIEXYZWidget(const QString& qsName, QWidget *pParent)
        : CSharedWidget(pParent),
          m_qsName(qsName)
{
        QHBoxLayout *pLayout = new QHBoxLayout();

        QLabel *pName = new QLabel(((m_qsName.isEmpty()) ? "" : QString("%1:").arg(m_qsName)));
        pName->setStyleSheet("QLabel { text-decoration: underline; }");

        pLayout->addWidget(pName);
        AddWidgets({new CFormWidget("ciexyzX", "1.0"),
                    new CFormWidget("ciexyzY", "1.0"),
                    new CFormWidget("ciexyzZ", "1.0")}, pLayout);

        setLayout(pLayout);
}

CIEXYZWidget::~CIEXYZWidget()
{

}

QList<QStandardItem *> CIEXYZWidget::GetData() const
{
        QStandardItem *pItem = new QStandardItem(((m_qsName.isEmpty()) ? "<CIEXYZ>" : QString("<%1>").arg(m_qsName)));
        pItem->appendRows(CSharedWidget::GetData());
        return {pItem};
}
