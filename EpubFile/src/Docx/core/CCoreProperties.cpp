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

#include "CCoreProperties.h"

CCoreProperties::CCoreProperties()
    : CElement(L"coreProperties",L"", L"cp")
{

}

void CCoreProperties::SetDefoult()
{
    AddArgument(L"xmlns:cp", L"http://schemas.openxmlformats.org/package/2006/metadata/core-properties");
    AddArgument(L"xmlns:dc", L"http://purl.org/dc/elements/1.1/");
    AddArgument(L"xmlns:dcterms", L"http://purl.org/dc/terms/");
    AddArgument(L"xmlns:dcmitype", L"http://purl.org/dc/dcmitype/");
    AddArgument(L"xmlns:xsi", L"http://www.w3.org/2001/XMLSchema-instance");

    CElement *oTitle = new CElement(L"title", L"", L"dc");
    AddChildren(oTitle);

    CElement *oSubject = new CElement(L"subject", L"Green", L"dc");
    AddChildren(oSubject);

    CElement *oCreator = new CElement(L"creator", L"", L"dc");
    AddChildren(oCreator);

    CElement *oKeywords = new CElement(L"keywords", L"", L"cp");
    AddChildren(oKeywords);

    CElement *oDescription = new CElement(L"description", L"", L"dc");
    AddChildren(oDescription);

    CElement *oLastModifiedBy = new CElement(L"lastModifiedBy", L"Green", L"cp");
    AddChildren(oLastModifiedBy);

    CElement *oRevision = new CElement(L"revision", L"", L"cp");
    AddChildren(oRevision);

    CElement *oCreated = new CElement(L"created", L"", L"dcterms", false);
    oCreated->AddArgument(L"xsi:type", L"dcterms:W3CDTF");
    oCreated->setValue(L"2020-07-17T09:14:00Z");
    AddChildren(oCreated);

    CElement *oModified = new CElement(L"modified", L"", L"dcterms", false);
    oModified->AddArgument(L"xsi:type", L"dcterms:W3CDTF");
    oModified->setValue(L"2020-07-17T09:14:00Z");
    AddChildren(oModified);
}
