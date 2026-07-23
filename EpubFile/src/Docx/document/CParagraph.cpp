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

#include "CParagraph.h"
#include "CParagraphProperties.h"
#include "CRun.h"
#include "../hyperlink/CHyperlink.h"
#include <stack>
#include <string>

CParagraph::CParagraph()
    : CElement(L"p", L"", L"w")
{
}

CParagraph::CParagraph(std::wstring sText, bool bNewPage)
    : CElement(L"p", L"", L"w")
{
    SetDefoult();
    SetText(sText, bNewPage);

    if (bNewPage)
        SetTagNewPage();
}

void CParagraph::SetText(std::wstring sText, bool bNewPage)
{
    std::stack <CElement*> stElements;
    for (int i = 0; i < GetCountChildrens(); i++)
        stElements.push(GetChildren(i));

    while (!stElements.empty())
    {
        CElement* oElement = stElements.top();
        stElements.pop();
        if (oElement->GetName() == L"t")
        {
            oElement->setValue(sText);
            return;
        }
        for (int i = 0; i < oElement->GetCountChildrens(); i++)
            stElements.push(oElement->GetChildren(i));
    }
}

void CParagraph::AddText(std::wstring sText, bool bNewPage)
{
    while (true)
    {
        for (int i = 0; i < GetCountChildrens(); i++)
        {
            CElement oElement = *GetChildren(i);
            if (oElement.GetName() == L"t")
            {
                oElement.setValue(oElement.GetValue() + sText);
                return;
            }
        }
    }
}

void CParagraph::SetDefoult()
{
    AddArgument(L"rsidR", L"003939F5");
    AddArgument(L"rsidRDefault", L"00481163");

    CElement *oBookmarkStart = new CElement(L"bookmarkStart", L"", L"w");
    oBookmarkStart->AddArgument(L"id", L"0");
    oBookmarkStart->AddArgument(L"name", L"_GoBack");
    AddChildren(oBookmarkStart);

    CElement *oBookmarkEnd = new CElement(L"bookmarkEnd", L"", L"w");
    oBookmarkEnd->AddArgument(L"id", L"0");
    AddChildren(oBookmarkEnd);

    CRun *oRun = new CRun;
    oRun->SetDefoult();
    AddChildren(oRun);
}

void CParagraph::SetTagNewPage()
{
    CRun *oRun = new CRun;
        CElement *oBr = new CElement(L"br", L"", L"w");
        oBr->AddArgument(L"type", L"page");
    oRun->AddChildren(oBr);
    AddChildren(oRun, 0);

}

void CParagraph::AddHyperlinkToc(int nIdToc)
{
    for (int i = 0; i < GetCountChildrens(); i++)
    {
        CElement *oElement = GetChildren(i);
        if (oElement->GetName() == L"bookmarkStart")
        {
            oElement->DeleteArgument(L"name");
            oElement->AddArgument(L"name", L"_Toc" + std::to_wstring(nIdToc));
        }
    }
}

void CParagraph::AddLinkToFile(CDocRelationshipsXml *oDocRel, std::wstring sPathFile)
{
    CHyperlink *oHyperlink = new CHyperlink;
    oHyperlink->CreateFileLink(sPathFile);
    if (!IsEmpty())
        Clear();

    AddArgument(L"rsidR", L"00EF05E0");
    AddArgument(L"rsidRDefault", L"006D386F");
    AddArgument(L"rsidP", L"00670FDF");

    AddChildren(oHyperlink);

    oDocRel->AddLinkToFile(sPathFile, oHyperlink->GetId());
}

void CParagraph::SetInCenter()
{
    for (int i = 0; i < GetCountChildrens(); i++)
    {
        CElement *oElement = GetChildren(i);
        if (oElement->GetName() == L"pPr")
        {
            CElement *oJc = new CElement(L"jc", L"", L"w");
            oJc->AddArgument(L"val", L"center");
            oElement->AddChildren(oJc);
            return;
        }
    }
    CElement *oPPr = new CElement(L"pPr", L"", L"w");
        CElement *oJc = new CElement(L"jc", L"", L"w");
        oJc->AddArgument(L"val", L"center");
    oPPr->AddChildren(oJc, 0);
    AddChildren(oPPr, 0);
}
