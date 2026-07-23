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

#include "OOXNumberingWriter.h"

OOXNumberingWriter::OOXNumberingWriter(OOXWriter& oWriter,RtfDocument& oDocument ):m_oWriter(oWriter)
{
	m_oRelsWriter = OOXRelsWriterPtr( new OOXRelsWriter( _T("numbering.xml"), oDocument ) );
	oWriter.m_oCustomRelsWriter.push_back( m_oRelsWriter );
	//m_sFileXml += oDocument.RenderToOOX(&oWriter,&oDocument,"numbering.xml","");
}
void OOXNumberingWriter::AddNumbering( std::wstring sText )
{
	m_sFileXml += sText;
}
bool OOXNumberingWriter::Save( std::wstring sFolder )
{
	if( false == m_sFileXml.empty() )
	{
		NSFile::CFileBinary file;

		if (false == file.CreateFile(sFolder + FILE_SEPARATOR_STR + _T("word") + FILE_SEPARATOR_STR + _T("numbering.xml"))) return false;

		m_oWriter.m_oDocRels.AddRelationship( _T("http://schemas.openxmlformats.org/officeDocument/2006/relationships/numbering"), _T("numbering.xml") );
		m_oWriter.m_oContentTypes.AddContent( _T("application/vnd.openxmlformats-officedocument.wordprocessingml.numbering+xml"), _T("/word/numbering.xml") );

		std::wstring sXml = CreateXml();
		std::string sXmlUTF = NSFile::CUtf8Converter::GetUtf8StringFromUnicode(sXml);

		file.WriteFile((void*)sXmlUTF.c_str(), (DWORD)sXmlUTF.length());
		file.CloseFile();
		return true;

	}
	return false;
}
bool OOXNumberingWriter::IsEmpty()
{
	return m_sFileXml.empty();
}
std::wstring OOXNumberingWriter::CreateXml()
{
	std::wstring sResult;
	sResult.append( _T("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\" ?>") );
	sResult.append( _T("\n") );
	sResult.append( _T("<w:numbering xmlns:w=\"http://schemas.openxmlformats.org/wordprocessingml/2006/main\" xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" xmlns:v=\"urn:schemas-microsoft-com:vml\" xmlns:o=\"urn:schemas-microsoft-com:office:office\" xmlns:wp=\"http://schemas.openxmlformats.org/drawingml/2006/wordprocessingDrawing\" xmlns:m=\"http://schemas.openxmlformats.org/officeDocument/2006/math\" xmlns:w10=\"urn:schemas-microsoft-com:office:word\">") );
	sResult.append( m_sFileXml );
	sResult.append( _T("</w:numbering>") );
	return sResult;
}
