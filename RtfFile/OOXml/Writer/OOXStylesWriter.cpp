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

#include "OOXStylesWriter.h"

OOXStylesWriter::OOXStylesWriter(OOXWriter& oWriter,RtfDocument& oDocument ):m_oWriter(oWriter)
{
	m_oRelsWriter = OOXRelsWriterPtr( new OOXRelsWriter( L"styles.xml", oDocument ) );
	oWriter.m_oCustomRelsWriter.push_back( m_oRelsWriter );
}
OOXStylesWriter::~OOXStylesWriter()
{
	int i = 0;
}
void OOXStylesWriter::AddContent( std::wstring sText )
{
	m_sFileXml += sText;
}
bool OOXStylesWriter::Save( std::wstring sFolder )
{
	std::wstring pathWord = sFolder + FILE_SEPARATOR_STR + L"word";

	//if( false == m_sFileXml.empty() )
	{
		NSFile::CFileBinary file;
		if (false == file.CreateFile(pathWord + FILE_SEPARATOR_STR + L"styles.xml")) return false;

		m_oWriter.m_oDocRels.AddRelationship( L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/styles", L"styles.xml" );
		m_oWriter.m_oContentTypes.AddContent( L"application/vnd.openxmlformats-officedocument.wordprocessingml.styles+xml", L"/word/styles.xml" );

		std::wstring sXml = CreateXml();
		std::string sXmlUTF = NSFile::CUtf8Converter::GetUtf8StringFromUnicode(sXml);

		file.WriteFile((void*)sXmlUTF.c_str(), (DWORD)sXmlUTF.length());

		file.CloseFile();
		return true;
	}
	//else
	//{
	//	//todooo default style !!
	//}
	return false;
}
std::wstring OOXStylesWriter::CreateXml()
{
	std::wstring sResult;
	sResult.append( L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\" ?>" );
	sResult.append( L"\n" );
	sResult.append( L"<w:styles xmlns:w = \"http://schemas.openxmlformats.org/wordprocessingml/2006/main\">" );
	sResult.append( m_sFileXml );
	sResult.append( L"</w:styles>" );
	return sResult;
}
