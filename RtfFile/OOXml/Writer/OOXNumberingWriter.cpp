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
