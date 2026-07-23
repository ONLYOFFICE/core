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

#include "CContentType.h"

CContentType::CContentType()
    : CElement(L"Types")
{
}

void CContentType::SetDefoult()
{
    AddArgument(L"xmlns", L"http://schemas.openxmlformats.org/package/2006/content-types");

    CElement *oDefault1 = new CElement(L"Default");
    oDefault1->AddArgument(L"Extension", L"rels");
    oDefault1->AddArgument(L"ContentType", L"application/vnd.openxmlformats-package.relationships+xml");
    AddChildren(oDefault1);

    CElement *oDefault2 = new CElement(L"Default");
    oDefault2->AddArgument(L"Extension", L"xml");
    oDefault2->AddArgument(L"ContentType", L"application/xml");
    AddChildren(oDefault2);

    CElement *oOverride1 = new CElement(L"Override");
    oOverride1->AddArgument(L"PartName", L"/word/document.xml");
    oOverride1->AddArgument(L"ContentType", L"application/vnd.openxmlformats-officedocument.wordprocessingml.document.main+xml");
    AddChildren(oOverride1);

    CElement *oOverride2 = new CElement(L"Override");
    oOverride2->AddArgument(L"PartName", L"/word/styles.xml");
    oOverride2->AddArgument(L"ContentType", L"application/vnd.openxmlformats-officedocument.wordprocessingml.styles+xml");
    AddChildren(oOverride2);

    CElement *oOverride3 = new CElement(L"Override");
    oOverride3->AddArgument(L"PartName", L"/word/settings.xml");
    oOverride3->AddArgument(L"ContentType", L"application/vnd.openxmlformats-officedocument.wordprocessingml.settings+xml");
    AddChildren(oOverride3);

    CElement *oOverride4 = new CElement(L"Override");
    oOverride4->AddArgument(L"PartName", L"/word/webSettings.xml");
    oOverride4->AddArgument(L"ContentType", L"application/vnd.openxmlformats-officedocument.wordprocessingml.webSettings+xml");
    AddChildren(oOverride4);

    CElement *oOverride5 = new CElement(L"Override");
    oOverride5->AddArgument(L"PartName", L"/word/fontTable.xml");
    oOverride5->AddArgument(L"ContentType", L"application/vnd.openxmlformats-officedocument.wordprocessingml.fontTable+xml");
    AddChildren(oOverride5);

    CElement *oOverride6 = new CElement(L"Override");
    oOverride6->AddArgument(L"PartName", L"/word/theme/theme1.xml");
    oOverride6->AddArgument(L"ContentType", L"application/vnd.openxmlformats-officedocument.theme+xml");
    AddChildren(oOverride6);

    CElement *oOverride7 = new CElement(L"Override");
    oOverride7->AddArgument(L"PartName", L"/docProps/core.xml");
    oOverride7->AddArgument(L"ContentType", L"application/vnd.openxmlformats-package.core-properties+xml");
    AddChildren(oOverride7);

    CElement *oOverride8 = new CElement(L"Override");
    oOverride8->AddArgument(L"PartName", L"/docProps/app.xml");
    oOverride8->AddArgument(L"ContentType", L"application/vnd.openxmlformats-officedocument.extended-properties+xml");
    AddChildren(oOverride8);
}
