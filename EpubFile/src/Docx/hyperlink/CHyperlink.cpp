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

#include "CHyperlink.h"

#include <random>
#include <string>

CHyperlink::CHyperlink()
    : CElement(L"hyperlink", L"", L"w")
{
    std::mt19937 rng;
    rng.seed(std::random_device()());
    std::uniform_int_distribution<std::mt19937::result_type> dist(1, 999999);

    m_nId = dist(rng);
}

void CHyperlink::SetDefoult()
{

}

int CHyperlink::GetId()
{
    return m_nId;
}

void CHyperlink::CreateTocHyperlink(std::wstring sText)
{
    AddArgument(L"anchor", L"_Toc" + std::to_wstring(m_nId));

        CElement *oRun1 = new CElement(L"r", L"", L"w");
        oRun1->AddArgument(L"rsidRPr", L"006E6199");
            CElement *oRPr1 = new CElement(L"rPr", L"", L"w");
                CElement *oRStyle1 = new CElement(L"rStyle", L"", L"w");
                oRStyle1->AddArgument(L"val", L"a4");
            oRPr1->AddChildren(oRStyle1);
                CElement *oNoProof1 = new CElement(L"noProof", L"", L"w");
            oRPr1->AddChildren(oNoProof1);
        oRun1->AddChildren(oRPr1);
            CElement *oText = new CElement(L"t", sText, L"w");
        oRun1->AddChildren(oText);
    AddChildren(oRun1);

        CElement *oRun2 = new CElement(L"r", L"", L"w");
            CElement *oRPr2 = new CElement(L"rPr", L"", L"w");
                CElement *oNoProof2 = new CElement(L"noProof", L"", L"w");
            oRPr2->AddChildren(oNoProof2);
                CElement *oWebHidden2 = new CElement(L"webHidden", L"", L"w");
            oRPr2->AddChildren(oWebHidden2);
        oRun2->AddChildren(oRPr2);
            CElement *oTab = new CElement(L"tab", L"", L"w");
        oRun2->AddChildren(oTab);
    AddChildren(oRun2);

        CElement *oRun3 = new CElement(L"r", L"", L"w");
            CElement *oRPr3 = new CElement(L"rPr", L"", L"w");
                CElement *oNoProof3 = new CElement(L"noProof", L"", L"w");
            oRPr3->AddChildren(oNoProof3);
                CElement *oWebHidden3 = new CElement(L"webHidden", L"", L"w");
            oRPr3->AddChildren(oWebHidden3);
        oRun3->AddChildren(oRPr3);
            CElement *oFldChar3 = new CElement(L"fldChar", L"", L"w");
            oFldChar3->AddArgument(L"fldCharType", L"begin");
        oRun3->AddChildren(oFldChar3);
    AddChildren(oRun3);

        CElement *oRun4 = new CElement(L"r", L"", L"w");
            CElement *oRPr4 = new CElement(L"rPr", L"", L"w");
                CElement *oNoProof4 = new CElement(L"noProof", L"", L"w");
            oRPr4->AddChildren(oNoProof4);
                CElement *oWebHidden4 = new CElement(L"webHidden", L"", L"w");
            oRPr4->AddChildren(oWebHidden4);
        oRun4->AddChildren(oRPr4);
            CElement *oInstrText4 = new CElement(L"instrText", L" PAGEREF _Toc" + std::to_wstring(m_nId) +L" \\h ", L"w", false);
            oInstrText4->AddArgument(L"xml:space", L"preserve");
        oRun4->AddChildren(oInstrText4);
    AddChildren(oRun4);

        CElement *oRun5 = new CElement(L"r", L"", L"w");
            CElement *oRPr5 = new CElement(L"rPr", L"", L"w");
                CElement *oNoProof5 = new CElement(L"noProof", L"", L"w");
            oRPr5->AddChildren(oNoProof5);
                CElement *oWebHidden5 = new CElement(L"webHidden", L"", L"w");
            oRPr5->AddChildren(oWebHidden5);
        oRun5->AddChildren(oRPr5);
    AddChildren(oRun5);

        CElement *oRun6 = new CElement(L"r", L"", L"w");
            CElement *oRPr6 = new CElement(L"rPr", L"", L"w");
                CElement *oNoProof6 = new CElement(L"noProof", L"", L"w");
            oRPr6->AddChildren(oNoProof6);
                CElement *oWebHidden6 = new CElement(L"webHidden", L"", L"w");
            oRPr6->AddChildren(oWebHidden6);
        oRun6->AddChildren(oRPr6);
            CElement *oFldChar6 = new CElement(L"fldChar", L"", L"w");
            oFldChar6->AddArgument(L"fldCharType", L"separate");
        oRun6->AddChildren(oFldChar6);
    AddChildren(oRun6);

        CElement *oRun7 = new CElement(L"r", L"", L"w");
            CElement *oRPr7 = new CElement(L"rPr", L"", L"w");
                CElement *oNoProof7 = new CElement(L"noProof", L"", L"w");
            oRPr7->AddChildren(oNoProof7);
                CElement *oWebHidden7 = new CElement(L"webHidden", L"", L"w");
            oRPr7->AddChildren(oWebHidden7);
        oRun7->AddChildren(oRPr7);
            CElement *oText7 = new CElement(L"t", L"#", L"w");
        oRun7->AddChildren(oText7);
    AddChildren(oRun7);

        CElement *oRun8 = new CElement(L"r", L"", L"w");
            CElement *oRPr8 = new CElement(L"rPr", L"", L"w");
                CElement *oNoProof8 = new CElement(L"noProof", L"", L"w");
            oRPr8->AddChildren(oNoProof8);
                CElement *oWebHidden8 = new CElement(L"webHidden", L"", L"w");
            oRPr8->AddChildren(oWebHidden8);
        oRun8->AddChildren(oRPr8);
            CElement *oFldChar8 = new CElement(L"fldChar", L"", L"w");
            oFldChar8->AddArgument(L"fldCharType", L"end");
        oRun8->AddChildren(oFldChar8);
    AddChildren(oRun8);
}

void CHyperlink::CreateFileLink(std::wstring sText)
{
    std::wstring _sText = sText;
    if (_sText.find(L"\\") != std::wstring::npos)
        _sText = _sText.substr(_sText.find_last_of(L"\\") + 1, _sText.length());

    EditBoolNamespace(false);
    AddArgument(L"r:id", L"rId" + std::to_wstring(m_nId));
        CElement *oRun = new CElement(L"r", L"", L"w");
        oRun->AddArgument(L"rsidR", L"00F17446");
        oRun->AddArgument(L"rsidRPr", L"006D386F");
            CElement *oRPr = new CElement(L"rPr", L"", L"w");
                CElement *oRStyle = new CElement(L"rStyle", L"", L"w");
                oRStyle->AddArgument(L"val", L"a3");
            oRPr->AddChildren(oRStyle);
        oRun->AddChildren(oRPr);
            CElement *oText = new CElement(L"t", _sText, L"w");
        oRun->AddChildren(oText);
    AddChildren(oRun);
        CElement *oBookmarkStart = new CElement(L"bookmarkStart", L"", L"w");
        oBookmarkStart->AddArgument(L"id", std::to_wstring(m_nId + 3));
        oBookmarkStart->AddArgument(L"name", L"_GoBack");
    AddChildren(oBookmarkStart);
        CElement *oBookmarkEnd = new CElement(L"bookmarkEnd", L"", L"w");
        oBookmarkEnd->AddArgument(L"id", std::to_wstring(m_nId + 3));
    AddChildren(oBookmarkEnd);
}
