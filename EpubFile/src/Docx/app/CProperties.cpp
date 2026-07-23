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

#include "CProperties.h"

CProperties::CProperties()
    :CElement(L"Properties")
{
}

void CProperties::SetDefoult()
{
    AddArgument(L"xmlns", L"http://schemas.openxmlformats.org/officeDocument/2006/extended-properties");
    AddArgument(L"xmlns:vt", L"http://schemas.openxmlformats.org/officeDocument/2006/docPropsVTypes");

    CElement *oTemplate = new CElement(L"Template", L"Normal.dotm");
    AddChildren(oTemplate);

    CElement *oTotalTime = new CElement(L"TotalTime", L"0");
    AddChildren(oTotalTime);

    CElement *oPages = new CElement(L"Pages", L"1");
    AddChildren(oPages);

    CElement *oWords = new CElement(L"Words", L"0");
    AddChildren(oWords);

    CElement *oCharacters = new CElement(L"Characters", L"0");
    AddChildren(oCharacters);

    CElement *oApplication = new CElement(L"Application", L"Microsoft Office Word");
    AddChildren(oApplication);

    CElement *oDocSecurity = new CElement(L"DocSecurity", L"0");
    AddChildren(oDocSecurity);

    CElement *oLines = new CElement(L"Lines", L"0");
    AddChildren(oLines);

    CElement *oParagraphs = new CElement(L"Paragraphs", L"0");
    AddChildren(oParagraphs);

    CElement *oScaleCrop = new CElement(L"ScaleCrop", L"false");
    AddChildren(oScaleCrop);

    CElement *oCompany = new CElement(L"Company");
    AddChildren(oCompany);

    CElement *oLinksUpToDate = new CElement(L"LinksUpToDate", L"false");
    AddChildren(oLinksUpToDate);

    CElement *oCharactersWithSpaces = new CElement(L"CharactersWithSpaces", L"0");
    AddChildren(oCharactersWithSpaces);

    CElement *oSharedDoc = new CElement(L"SharedDoc", L"false");
    AddChildren(oSharedDoc);

    CElement *oHyperlinksChanged = new CElement(L"HyperlinksChanged", L"false");
    AddChildren(oHyperlinksChanged);

    CElement *oAppVersion = new CElement(L"AppVersion", L"16.0000");
    AddChildren(oAppVersion);
}
