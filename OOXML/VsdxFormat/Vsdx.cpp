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
#include "Vsdx.h"

#include "../DocxFormat/App.h"
#include "../DocxFormat/Core.h"

#include "FileTypes_Draw.h"

#include "../../DesktopEditor/common/SystemUtils.h"

#include "VisioDocument.h"
#include "VisioConnections.h"
#include "Shapes.h"


OOX::Draw::CVsdx::CVsdx() : OOX::IFileContainer(dynamic_cast<OOX::Document*>(this))
{
}
OOX::Draw::CVsdx::CVsdx(const CPath& oFilePath) : OOX::IFileContainer(dynamic_cast<OOX::Document*>(this))
{
	m_bVisioPages = true;
	Read( oFilePath );
}
OOX::Draw::CVsdx::~CVsdx()
{

}	
bool OOX::Draw::CVsdx::Read(const CPath& oFilePath)
{
	m_sDocumentPath = oFilePath.GetPath();

	OOX::CRels oRels(oFilePath / FILE_SEPARATOR_STR);
	IFileContainer::Read(oRels, oFilePath, oFilePath);

	m_pDocument = Find(FileTypes::Document).smart_dynamic_cast<Draw::CDocumentFile>();
	m_pApp = Find(OOX::FileTypes::App).smart_dynamic_cast<CApp>();
	m_pCore = Find(OOX::FileTypes::Core).smart_dynamic_cast<CCore>();
	
	m_pContentTypes = new CContentTypes(m_sDocumentPath);
	return true;
}
bool OOX::Draw::CVsdx::Write(const CPath& oDirPath, OOX::CContentTypes &oContentTypes)
{  
	IFileContainer::Write(oDirPath / L"" , OOX::CPath(L""), oContentTypes);

    oContentTypes.Write(oDirPath);
    return true;
}
//-----------------------------------------------------------------------------------------------------------------------------
