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

#include "OOXRelsWriter.h"

OOXRelsWriter::OOXRelsWriter( std::wstring sFileName, RtfDocument& oDocument ):m_oDocument(oDocument)
{
	m_sFileName = sFileName;
}
std::wstring OOXRelsWriter::AddRelationship( std::wstring sType, std::wstring sTarget, bool bTargetModeInternal )
{
	for (size_t i = 0 ;i < m_aTargets.size(); i++ )
	{
		if( sTarget == m_aTargets[i] )
			return m_aIDs[i];
	}
	m_aTargets.push_back( sTarget );
	m_aTypes.push_back( sType );
	std::wstring sId = m_oDocument.m_oIdGenerator.Generate_rId();
	m_aIDs.push_back( sId );
	m_aModes.push_back( bTargetModeInternal );
	return sId;
}
std::wstring OOXRelsWriter::CreateXml()
{
	std::wstring sResult;
	sResult += _T("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n");

	sResult += _T("<Relationships xmlns=\"http://schemas.openxmlformats.org/package/2006/relationships\">");

	for (size_t i = 0; i < m_aTargets.size(); i++ )
	{
		sResult += _T("<Relationship Id=\"");
		sResult += m_aIDs[i];
		sResult += _T("\" Type=\"");
		sResult += m_aTypes[i];
		sResult += _T("\" Target=\"");
		sResult += m_aTargets[i];
		sResult += _T("\"");
		if( false == m_aModes[i] )
			sResult += _T(" TargetMode=\"External\"");
		sResult += _T("/>");
	}
	sResult += _T("</Relationships>");
	return sResult;
}
bool OOXRelsWriter::Save( std::wstring sFolder )
{
	if( m_aTargets.size() < 1 )return false;

	std::wstring pathRels = sFolder + FILE_SEPARATOR_STR + _T("_rels");
	NSDirectory::CreateDirectory(pathRels) ;

	NSFile::CFileBinary file;
	if (false == file.CreateFile(pathRels + FILE_SEPARATOR_STR + m_sFileName + _T(".rels"))) return false;

	std::wstring sXml = CreateXml();

	std::string sXmlUTF = NSFile::CUtf8Converter::GetUtf8StringFromUnicode(sXml);

	file.WriteFile((void*)sXmlUTF.c_str(), (DWORD)sXmlUTF.length());
	file.CloseFile();

	return true;
}
