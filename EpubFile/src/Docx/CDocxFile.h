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

#ifndef CDOCXFILE_H
#define CDOCXFILE_H

#include <iostream>

#include "document/CDocumentXml.h"
#include "contentType/CContentTypeXml.h"
#include "fontTable/CFontTableXml.h"
#include "settings/CSettingsXml.h"
#include "webSettings/CWebSettingsXml.h"
#include "style/CStylesXml.h"
#include "rels/CRelationshipsXml.h"
#include "docRels/CDocRelationshipsXml.h"
#include "app/CAppXml.h"
#include "core/CCoreXml.h"

#include "document/CParagraph.h"
#include "../CBookToc.h"

class CDocxFile
{
    std::wstring m_sTempDir;
    std::wstring m_sRelsDir;
    std::wstring m_sWordDir;
    std::wstring m_sDocPropsDir;

    CDocumentXml        *m_oDocumentXml;
    CContentTypeXml     *m_oContentTypeXml;
    CFontTableXml       *m_oFontTableXml;
    CSettingsXml        *m_oSettingsXml;
    CWebSettingsXml     *m_oWebSettingsXml;
    CStylesXml          *m_oStylesXml;
    CRelationshipsXml   *m_oRelationshipsXml;
    CDocRelationshipsXml*m_oDocRelationshipsXml;
    CAppXml             *m_oAppXml;
    CCoreXml            *m_oCoreXml;

    std::wstring sTemplate;
    std::wstring sApplication;
    std::wstring sCompany;
    int nTotalTime;
    int nCountPages;
    int nCountCharacters;
    int nDocSecurity;
    int nCharactersWithSpaces;
    double dAppVersion;
    bool bScaleCrop;
    bool bLinksUpToDate;
    bool bSharedDoc;
    bool bHyperlinksChanged;

    bool CreateRelsDir();
    bool CreateWordDir();
    bool CreateDocPropsDir();

    bool CreateDocumentXml();
    bool CreateFontTableXml();
    bool CreateSettingsXml();
    bool CreateWebSettingsXml();
    bool CreateStylesXml();
    bool CreateContentTypesXml();
    bool CreateThemeXml();
    bool CreateRelsXml();
    bool CreateDocRelsXml();
    bool CreateAppXml();
    bool CreateCoreXml();
public:
    CDocxFile();
    ~CDocxFile();

    bool CreateTempFiles(std::wstring sPathFile, std::wstring sPathTempDir);

    bool AddParagraph(std::wstring sText, bool bNewPAge = false);
    bool AddParagraph(CParagraph *oParagraph);
    bool AddBookToc(CBookToc *oBookToc);
    bool AddRelationship(CElement *oRelationship);
//    bool AddHypelink(std::wstring sText, );
//    bool OpenFile(std::wstring sPathFile);

    bool SaveToFile();
};

#endif // CDOCXFILE_H
