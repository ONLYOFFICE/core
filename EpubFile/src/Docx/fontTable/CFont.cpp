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

#include "CFont.h"

CFont::CFont()
    : CElement(L"font", L"", L"w")
{

}

void CFont::SetDefoult()
{
    AddArgument(L"name", L"Calibri");

    CElement *oPanose1 = new CElement(L"panose1", L"", L"w");
    oPanose1->AddArgument(L"val", L"020F0502020204030204");
    AddChildren(oPanose1);

    CElement *oCharset = new CElement(L"charset", L"", L"w");
    oCharset->AddArgument(L"val", L"CC");
    AddChildren(oCharset);

    CElement *oFamily = new CElement(L"family", L"", L"w");
    oFamily->AddArgument(L"val", L"swiss");
    AddChildren(oFamily);

    CElement *oPitch = new CElement(L"pitch", L"", L"w");
    oPitch->AddArgument(L"val", L"variable");
    AddChildren(oPitch);

    CElement *oSig = new CElement(L"sig", L"", L"w");
    oSig->AddArgument(L"usb0", L"E4002EFF");
    oSig->AddArgument(L"usb1", L"C000247B");
    oSig->AddArgument(L"usb2", L"00000009");
    oSig->AddArgument(L"usb3", L"00000000");
    oSig->AddArgument(L"csb0", L"000001FF");
    oSig->AddArgument(L"csb1", L"00000000");
    AddChildren(oSig);

}
