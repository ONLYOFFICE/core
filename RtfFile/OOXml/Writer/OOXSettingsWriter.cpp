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

#include "OOXSettingsWriter.h"

std::wstring OOXSettingsWriter::CreateXml()
{
	std::wstring sResult;
	sResult.append( _T("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\" ?>") );
	sResult.append( _T("\n") );
	sResult.append( _T("<w:settings xmlns:w = \"http://schemas.openxmlformats.org/wordprocessingml/2006/main\" xmlns:m = \"http://schemas.openxmlformats.org/officeDocument/2006/math\">") );
	sResult.append( m_sFileXml );
	sResult.append( _T("</w:settings>") );
	return sResult;
}
OOXSettingsWriter::OOXSettingsWriter(OOXWriter& oWriter,RtfDocument& oDocument ):m_oWriter(oWriter)
{
	m_oRelsWriter = OOXRelsWriterPtr( new OOXRelsWriter( _T("settings.xml"), oDocument ) );
	oWriter.m_oCustomRelsWriter.push_back( m_oRelsWriter );
}
void OOXSettingsWriter::AddContent( std::wstring sText )
{
	m_sFileXml += sText;
}
bool OOXSettingsWriter::Save( std::wstring sFolder )
{
	std::wstring pathWord = sFolder + FILE_SEPARATOR_STR + _T("word");

	if ( false == m_sFileXml.empty() )
	{
		//generate our xml
		std::wstring sXml = CreateXml();

		//todoooo REWRITE

		////take xml from template
		//std::wstring sFilename = sFolder +  _T("\\word\\settings.xml");
		//if( true == RtfUtility:: SaveResourceToFile( IDR_SETTINGS, L"XML", sFilename ) )
		//{
		//	XmlUtils::CXmlLiteReader oXmlReader1;
		//	XmlUtils::CXmlLiteReader oXmlReader2;

		//	if( TRUE == oXmlReader1.OpenFromFile( sFilename ) && TRUE == oXmlReader2.OpenFromXmlString( sXml ) &&
		//		TRUE == oXmlReader1.ReadRootNode( _T("w:settings") ) && TRUE == oXmlReader2.ReadRootNode( _T("w:settings") ) )
		//	{
		//		sXml = _T("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\" ?>");
		//		//sXml += RtfUtility::MergeXml( oXmlReader1, oXmlReader2 );
		//	}
		//}

		NSFile::CFileBinary file;
		if (false == file.CreateFile(pathWord + FILE_SEPARATOR_STR + _T("settings.xml"))) return false;

		m_oWriter.m_oDocRels.AddRelationship( _T("http://schemas.openxmlformats.org/officeDocument/2006/relationships/settings"), _T("settings.xml") );
		m_oWriter.m_oContentTypes.AddContent( _T("application/vnd.openxmlformats-officedocument.wordprocessingml.settings+xml"), _T("/word/settings.xml") );

		std::string sXmlUTF = NSFile::CUtf8Converter::GetUtf8StringFromUnicode(sXml);

		file.WriteFile((void*)sXmlUTF.c_str(), (DWORD)sXmlUTF.length());

		file.CloseFile();
		;
		return true;
	}
	else
	{
		//if( true == RtfUtility:: SaveResourceToFile( IDR_SETTINGS, L"XML", sFolder +  _T("\\word\\settings.xml") ) )
		//{
		//	m_oWriter.m_oDocRels.AddRelationship( _T("http://schemas.openxmlformats.org/officeDocument/2006/relationships/settings"), _T("settings.xml") );
		//	m_oWriter.m_oContentTypes.AddContent( _T("application/vnd.openxmlformats-officedocument.wordprocessingml.settings+xml"), _T("/word/settings.xml") );
		//}
	}
	return false;
}
bool OOXSettingsWriter::IsEmpty()
{
	return m_sFileXml.empty();
}
