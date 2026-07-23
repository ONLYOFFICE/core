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

#include "CColorWidget.h"
#include "Common.h"

#include <QColorDialog>
#include <QPushButton>
#include <QLabel>

CColorWidget::CColorWidget(const QString& qsName, QWidget *pParent) :
        CSharedWidget(pParent),
        m_qsName(qsName),
        m_pColorLayout(NULL),
        m_bReverseOrder(false)
{
        QVBoxLayout *pLayout = new QVBoxLayout;

        QLabel *pName = new QLabel(QString("%1:").arg(m_qsName));
        pName->setStyleSheet("QLabel { text-decoration: underline; }");

        QPushButton *pSelectableButton = new QPushButton("Select color");

        connect(pSelectableButton, &QPushButton::clicked, this, &CColorWidget::on_select_color_clicked);

        pLayout->addWidget(pName);
        pLayout->addWidget(pSelectableButton);

        setLayout(pLayout);
}

CColorWidget::~CColorWidget()
{

}

void CColorWidget::setReverseOrder(bool bReverseOrder)
{
        m_bReverseOrder = bReverseOrder;
}

QList<QStandardItem *> CColorWidget::GetData() const
{
        if (!m_oColor.isValid())
                return {};

        QStandardItem *pItem = new QStandardItem(QString("<%1>").arg(m_qsName));

        if (m_bReverseOrder)
        {
                pItem->appendRow(new QStandardItem("<a>0</a>"));
                pItem->appendRow(new QStandardItem(QString("<b>%1</b>").arg(m_oColor.blue())));
                pItem->appendRow(new QStandardItem(QString("<g>%1</g>").arg(m_oColor.green())));
                pItem->appendRow(new QStandardItem(QString("<r>%1</r>").arg(m_oColor.red())));
        }
        else
        {
                pItem->appendRow(new QStandardItem(QString("<r>%1</r>").arg(m_oColor.red())));
                pItem->appendRow(new QStandardItem(QString("<g>%1</g>").arg(m_oColor.green())));
                pItem->appendRow(new QStandardItem(QString("<b>%1</b>").arg(m_oColor.blue())));
                pItem->appendRow(new QStandardItem("<a>0</a>"));
        }

        return {pItem};
}

void CColorWidget::on_select_color_clicked()
{
        m_oColor = QColorDialog::getColor(Qt::white);

        if (!m_oColor.isValid())
                return;

        if (NULL == m_pColorLayout)
        {
                m_pColorLayout = new QHBoxLayout;
                ((QHBoxLayout*)layout())->addLayout(m_pColorLayout);
        }
        else
        {
                ClearLayout(m_pColorLayout);
        }

        QFrame *pColorFrame = new QFrame;
        pColorFrame->setStyleSheet(QString("background-color: rgb(%1, %2, %3)").arg(m_oColor.red()).arg(m_oColor.green()).arg(m_oColor.blue()));

        m_pColorLayout->addWidget(new QLabel(QString("Red: %1").arg(m_oColor.red())));
        m_pColorLayout->addWidget(new QLabel(QString("Green: %1").arg(m_oColor.green())));
        m_pColorLayout->addWidget(new QLabel(QString("Blue: %1").arg(m_oColor.blue())));
        m_pColorLayout->addWidget(pColorFrame);
}
