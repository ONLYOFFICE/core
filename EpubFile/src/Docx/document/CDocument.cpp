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

#include "CDocument.h"
#include "CBody.h"

CDocument::CDocument()
    : CElement(L"document", L"", L"w", false)
{
}

void CDocument::SetDefoult()
{
    AddArgument(L"xmlns:wpc", L"http://schemas.microsoft.com/office/word/2010/wordprocessingCanvas");
    AddArgument(L"xmlns:cx", L"http://schemas.microsoft.com/office/drawing/2014/chartex");
    AddArgument(L"xmlns:mc", L"http://schemas.openxmlformats.org/markup-compatibility/2006");
    AddArgument(L"xmlns:o", L"urn:schemas-microsoft-com:office:office");
    AddArgument(L"xmlns:r", L"http://schemas.openxmlformats.org/officeDocument/2006/relationships");
    AddArgument(L"xmlns:m", L"http://schemas.openxmlformats.org/officeDocument/2006/math");
    AddArgument(L"xmlns:v", L"urn:schemas-microsoft-com:vml");
    AddArgument(L"xmlns:wp14", L"http://schemas.microsoft.com/office/word/2010/wordprocessingDrawing");
    AddArgument(L"xmlns:wp", L"http://schemas.openxmlformats.org/drawingml/2006/wordprocessingDrawing");
    AddArgument(L"xmlns:w10", L"urn:schemas-microsoft-com:office:word");
    AddArgument(L"xmlns:w", L"http://schemas.openxmlformats.org/wordprocessingml/2006/main");
    AddArgument(L"xmlns:w14", L"http://schemas.microsoft.com/office/word/2010/wordml");
    AddArgument(L"xmlns:w15", L"http://schemas.microsoft.com/office/word/2012/wordml");
    AddArgument(L"xmlns:w16se", L"http://schemas.microsoft.com/office/word/2015/wordml/symex");
    AddArgument(L"xmlns:wpg", L"http://schemas.microsoft.com/office/word/2010/wordprocessingGroup");
    AddArgument(L"xmlns:wpi", L"http://schemas.microsoft.com/office/word/2010/wordprocessingInk");
    AddArgument(L"xmlns:wne", L"http://schemas.microsoft.com/office/word/2006/wordml");
    AddArgument(L"xmlns:wps", L"http://schemas.microsoft.com/office/word/2010/wordprocessingShape");
    AddArgument(L"mc:Ignorable", L"w14 w15 w16se wp14");


    CBody *oBody = new CBody;
    oBody->SetDefoult();
    AddChildren(oBody);
}

bool CDocument::AddParagraph(std::wstring sText, bool bNewPage)
{
    CParagraph *oNewParapgraph = new CParagraph(sText, bNewPage);

    if (!AddParagraph(oNewParapgraph))
        return false;
    return true;
}

bool CDocument::AddParagraph(CParagraph *oParagraph)
{
    for (int i = 0; i < GetCountChildrens(); i++)
    {
        if (GetChildren(i)->GetName() == L"body")
        {
            GetChildren(i)->AddChildren(oParagraph, GetChildren(i)->GetCountChildrens() - 1);
            return true;
        }
    }
    return false;
}

CParagraph* CDocument::GetParagraph(int nIndex)
{
    if (nIndex < 0)
        return NULL;

    for (int i = 0; i < GetCountChildrens(); i++)
    {
        if (GetChildren(i)->GetName() == L"body")
        {
            int nNumber = 0;
            for (int j = 0; j < GetChildren(i)->GetCountChildrens(); j++)
            {
                if (GetChildren(i)->GetChildren(j)->GetName() == L"p")
                {
                    if (nNumber == nIndex)
                        return static_cast<CParagraph*>(GetChildren(i)->GetChildren(j));
                    nNumber++;
                }
            }
            return NULL;
        }
    }
}
