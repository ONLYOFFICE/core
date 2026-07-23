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

#include "CStyles.h"
#include "CStyle.h"

CStyles::CStyles()
    :CElement(L"styles", L"", L"w")
{

}

void CStyles::SetDefoult()
{
    CElement *oDocDefaults = new CElement(L"docDefaults", L"", L"w");
    CElement *oRPrDefault = new CElement(L"rPrDefault", L"", L"w");
    CElement *oRPr = new CElement(L"rPr", L"", L"w");

    CElement *oRFonts = new CElement(L"rFonts", L"", L"w");
    oRFonts->AddArgument(L"asciiTheme", L"minorHAnsi");
    oRFonts->AddArgument(L"eastAsiaTheme", L"minorEastAsia");
    oRFonts->AddArgument(L"hAnsiTheme", L"minorHAnsi");
    oRFonts->AddArgument(L"cstheme", L"minorBidi");
    oRPr->AddChildren(oRFonts);

    CElement *oSz = new CElement(L"sz", L"", L"w");
    oSz->AddArgument(L"val", L"22");
    oRPr->AddChildren(oSz);

    CElement *oSzCs = new CElement(L"sz", L"", L"w");
    oSzCs->AddArgument(L"val", L"22");
    oRPr->AddChildren(oSzCs);

    CElement *oLang = new CElement(L"lang", L"", L"w");
    oLang->AddArgument(L"val", L"ru-RU");
    oLang->AddArgument(L"eastAsia", L"zh-TW");
    oLang->AddArgument(L"bidi", L"ar-SA");
    oRPr->AddChildren(oLang);

    oRPrDefault->AddChildren(oRPr);
    oDocDefaults->AddChildren(oRPrDefault);

    CElement *oPPrDefault = new CElement(L"pPrDefault", L"", L"w");
    CElement *oPPr = new CElement(L"pPr", L"", L"w");
    CElement *oSpacing = new CElement(L"spacing", L"", L"w");
    oSpacing->AddArgument(L"after", L"160");
    oSpacing->AddArgument(L"line", L"259");
    oSpacing->AddArgument(L"lineRule", L"auto");

    oPPr->AddChildren(oSpacing);
    oPPrDefault->AddChildren(oPPr);
    oDocDefaults->AddChildren(oPPrDefault);

    AddChildren(oDocDefaults);

    CStyle *oPStyle = new CStyle;
    oPStyle->SetDefoult();
    AddChildren(oPStyle);
}
