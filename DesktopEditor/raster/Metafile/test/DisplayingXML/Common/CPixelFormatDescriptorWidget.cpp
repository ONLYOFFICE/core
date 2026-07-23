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

        pLayout->addWidget(pName);

        AddWidgets({new CFormWidget("dwFlags",          "0"),
                    new CFormWidget("iPixelType",       "0"),
                    new CFormWidget("cColorBits",       "0"),
                    new CFormWidget("cRedBits",         "0"),
                    new CFormWidget("cRedShift",        "0"),
                    new CFormWidget("cGreenBits",       "0"),
                    new CFormWidget("cGreenShift",      "0"),
                    new CFormWidget("cBlueBits",        "0"),
                    new CFormWidget("cBlueShift",       "0"),
                    new CFormWidget("cAlphaBits",       "0"),
                    new CFormWidget("cAlphaShift",      "0"),
                    new CFormWidget("cAccumBits",       "0"),
                    new CFormWidget("cAccumRedBits",    "0"),
                    new CFormWidget("cAccumGreenBits",  "0"),
                    new CFormWidget("cAccumBlueBits",   "0"),
                    new CFormWidget("cAccumAlphaBits",  "0"),
                    new CFormWidget("cDepthBits",       "0"),
                    new CFormWidget("cStencilBits",     "0"),
                    new CFormWidget("cAuxBuffers",      "0"),
                    new CFormWidget("bReserved",        "0"),
                    new CFormWidget("dwVisibleMask",    "0")}, pLayout);

        setLayout(pLayout);
}

CPixelFormatDescriptorWidget::~CPixelFormatDescriptorWidget()
{

}

QList<QStandardItem *> CPixelFormatDescriptorWidget::GetData() const
{
        QStandardItem *pItem = new QStandardItem("<PixelFormatDescriptor>");
        pItem->appendRows(CSharedWidget::GetData());
        return {pItem};
}
