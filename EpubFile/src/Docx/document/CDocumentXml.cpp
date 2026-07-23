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

#include "CDocumentXml.h"
#include "../../../../DesktopEditor/xml/include/xmlutils.h"
#include "CDocument.h"

CDocumentXml::CDocumentXml()
    : CXmlFile()
{
}

void CDocumentXml::SetDefoult()
{
    CDocument *oDocument = new CDocument();
    oDocument->SetDefoult();
    SetXmlStructure(oDocument);
}

bool CDocumentXml::AddParagraph(std::wstring sText, bool bNewPAge)
{
    if (IsEmpty())
        return false;

    CElement* oDocumentStructure = GetXmlStructure();
    if (oDocumentStructure->GetName() != L"document")
        return false;

    CDocument *oDocument = static_cast<CDocument*>(oDocumentStructure);
    if (!oDocument->AddParagraph(sText, bNewPAge))
        return false;

    return true;
}

bool CDocumentXml::AddParagraph(CParagraph *oParagraph)
{
    if (IsEmpty() || oParagraph == NULL)
        return false;

    CElement* oDocumentStructure = GetXmlStructure();
    if (oDocumentStructure->GetName() != L"document")
        return false;

    CDocument *oDocument = static_cast<CDocument*>(oDocumentStructure);
    if (!oDocument->AddParagraph(oParagraph))
        return false;

    return true;
}

bool CDocumentXml::AddElement(CElement *oElement, bool bEnd)
{
    if (IsEmpty() || oElement == NULL)
        return false;

    CElement* oDocumentStructure = GetXmlStructure();
    if (oDocumentStructure->GetName() != L"document")
        return false;

    CDocument *oDocument = static_cast<CDocument*>(oDocumentStructure);
    if (bEnd)
        oDocument->AddChildren(oElement, oDocument->GetCountChildrens() - 1);
    else
        oDocument->AddChildren(oElement, 0);
    return true;
}

CParagraph* CDocumentXml::GetParagraph(int nIndex)
{
    return static_cast<CDocument*>(GetXmlStructure())->GetParagraph(nIndex);
}

