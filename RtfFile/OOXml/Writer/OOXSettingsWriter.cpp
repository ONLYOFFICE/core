/*
 * (c) Copyright Ascensio System SIA 2010-2019
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
 * street, Riga, Latvia, EU, LV-1050.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
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
		//генерируем свою xml
		std::wstring sXml = CreateXml();

		//todoooo ПЕРЕПИСАТЬ

		////берем xml из шаблона
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
