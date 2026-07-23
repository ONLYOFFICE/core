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

#include "OOXContentTypesWriter.h"
#include "../../../DesktopEditor/common/File.h"
#include "../../../OOXML/Base/Base.h"

void OOXContentTypesWriter::AddWriter()
{
}
void OOXContentTypesWriter::AddContent( std::wstring sType, std::wstring sTarget )
{
	for (size_t i = 0 ;i < (int)m_aTargets.size(); i++ )
		if( sTarget == m_aTargets[i] )
			return;
	m_aTargets.push_back( sTarget );
	m_aTypes.push_back( sType );
}
void OOXContentTypesWriter::AddExtension( std::wstring sType, std::wstring sTarget )
{
	for (size_t i = 0 ;i < (int)m_aExtensions.size(); i++ )
		if( sTarget == m_aExtensions[i] )
			return;
	m_aExtensions.push_back( sTarget );
	m_aExtTypes.push_back( sType );
}
bool OOXContentTypesWriter::Save(std::wstring sFolder)
{
	NSFile::CFileBinary file;

	if (false == file.CreateFile(sFolder + FILE_SEPARATOR_STR + _T("[Content_Types].xml"))) return false;

	std::wstring sXml = CreateXml();

	std::string sXmlUTF = NSFile::CUtf8Converter::GetUtf8StringFromUnicode(sXml);

	file.WriteFile((const void*)sXmlUTF.c_str(), (DWORD)sXmlUTF.length());

	file.CloseFile();
	return true;
}
std::wstring OOXContentTypesWriter::CreateXml()
{
	std::wstring sResult;
	sResult += _T("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\" ?>\n");

	sResult += _T("<Types xmlns=\"http://schemas.openxmlformats.org/package/2006/content-types\">");

	sResult += _T("<Default Extension=\"rels\" ContentType=\"application/vnd.openxmlformats-package.relationships+xml\"/>");
	sResult += _T("<Default Extension=\"xml\" ContentType=\"application/xml\"/>");

	for (size_t i = 0; i < m_aExtensions.size(); i++ )
	{
		sResult += _T("<Default Extension=\"");
		sResult += m_aExtensions[i];
		sResult += _T("\" ContentType=\"");
		sResult += m_aExtTypes[i];
		sResult += _T("\"/>");
	}

	for (size_t i = 0; i < m_aTargets.size(); i++ )
	{
		sResult += _T("<Override PartName=\"");
		sResult += m_aTargets[i];
		sResult += _T("\" ContentType=\"");
		sResult += m_aTypes[i];
		sResult += _T("\"/>");
	}
	sResult += _T("</Types>");
	return sResult;
}

