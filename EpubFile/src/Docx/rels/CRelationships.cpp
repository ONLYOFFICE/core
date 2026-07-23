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

#include "CRelationships.h"

CRelationships::CRelationships()
    : CElement(L"Relationships")
{
}

void CRelationships::SetDefoult()
{
    AddArgument(L"xmlns", L"http://schemas.openxmlformats.org/package/2006/relationships");

    CElement *oRelationship1 = new CElement(L"Relationship");
    oRelationship1->AddArgument(L"Id", L"rId3");
    oRelationship1->AddArgument(L"Type", L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/extended-properties");
    oRelationship1->AddArgument(L"Target", L"docProps/app.xml");
    AddChildren(oRelationship1);

    CElement *oRelationship2 = new CElement(L"Relationship");
    oRelationship2->AddArgument(L"Id", L"rId2");
    oRelationship2->AddArgument(L"Type", L"http://schemas.openxmlformats.org/package/2006/relationships/metadata/core-properties");
    oRelationship2->AddArgument(L"Target", L"docProps/core.xml");
    AddChildren(oRelationship2);

    CElement *oRelationship3 = new CElement(L"Relationship");
    oRelationship3->AddArgument(L"Id", L"rId1");
    oRelationship3->AddArgument(L"Type", L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/officeDocument");
    oRelationship3->AddArgument(L"Target", L"word/document.xml");
    AddChildren(oRelationship3);
}
