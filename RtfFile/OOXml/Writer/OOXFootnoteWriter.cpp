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

#include "OOXFootnoteWriter.h"

OOXFootnoteWriter::OOXFootnoteWriter( OOXWriter& oWriter,RtfDocument& oDocument ):m_oWriter(oWriter),m_oDocument(oDocument)
{
	m_oRelsWriter = OOXRelsWriterPtr( new OOXRelsWriter( _T("footnotes.xml"), oDocument ) );
	oWriter.m_oCustomRelsWriter.push_back( m_oRelsWriter );
}
void OOXFootnoteWriter::AddFootnoteBegin( std::wstring sType, int nID, std::wstring sText )
{
	std::wstring sFootnote;
	sFootnote += _T("<w:footnote");
	if( false == sType.empty() )
	{
		sFootnote += _T(" w:type=\"");
		sFootnote += sType;
		sFootnote += _T("\"");

	}
	if( PROP_DEF != nID )
		sFootnote += L" w:id=\"" + std::to_wstring(nID) + L"\"";
	sFootnote += _T(">");
	sFootnote += sText;
	sFootnote += _T("</w:footnote>");

	m_sFootnotes.insert( m_sFootnotes.begin() , sFootnote.begin(), sFootnote.end() );
}
void OOXFootnoteWriter::AddFootnote( std::wstring sType, int nID, std::wstring sText )
{
	m_sFootnotes += _T("<w:footnote");
	if( !sType.empty() )
	{
		m_sFootnotes += _T(" w:type=\"");
		m_sFootnotes += sType;
		m_sFootnotes += _T("\"");
	}
	if( PROP_DEF != nID )
		m_sFootnotes += L" w:id=\"" + std::to_wstring(nID) + L"\"";
	m_sFootnotes += _T(">");
	m_sFootnotes += sText;
	m_sFootnotes += _T("</w:footnote>");
}
bool OOXFootnoteWriter::Save( std::wstring sFolder )
{
	if(  m_sFootnotes.empty() ) return false;

	NSFile::CFileBinary file;
	if (false == file.CreateFile(sFolder + FILE_SEPARATOR_STR + _T("footnotes.xml"))) return false;

	m_oWriter.m_oDocRels.AddRelationship( _T("http://schemas.openxmlformats.org/officeDocument/2006/relationships/footnotes"), _T("footnotes.xml") );
	m_oWriter.m_oContentTypes.AddContent( _T("application/vnd.openxmlformats-officedocument.wordprocessingml.footnotes+xml"), _T("/word/footnotes.xml") );

	std::wstring sXml = CreateXml();

	std::string sXmlUTF = NSFile::CUtf8Converter::GetUtf8StringFromUnicode(sXml);

	file.WriteFile((void*)sXmlUTF.c_str(), (DWORD)sXmlUTF.length());

	file.CloseFile();
	return true;
}
std::wstring OOXFootnoteWriter::CreateXml()
{
	std::wstring sResult;
	sResult += _T("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n");

	sResult += _T("<w:footnotes xmlns:wpc=\"http://schemas.microsoft.com/office/word/2008/6/28/wordprocessingCanvas\" xmlns:mc=\"http://schemas.openxmlformats.org/markup-compatibility/2006\" xmlns:o=\"urn:schemas-microsoft-com:office:office\" xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" xmlns:m=\"http://schemas.openxmlformats.org/officeDocument/2006/math\" xmlns:v=\"urn:schemas-microsoft-com:vml\" xmlns:wp14=\"http://schemas.microsoft.com/office/word/2008/9/16/wordprocessingDrawing\" xmlns:wp=\"http://schemas.openxmlformats.org/drawingml/2006/wordprocessingDrawing\" xmlns:w10=\"urn:schemas-microsoft-com:office:word\" xmlns:w=\"http://schemas.openxmlformats.org/wordprocessingml/2006/main\" xmlns:w14=\"http://schemas.microsoft.com/office/word/2009/2/wordml\" xmlns:wpg=\"http://schemas.microsoft.com/office/word/2008/6/28/wordprocessingGroup\" xmlns:wpi=\"http://schemas.microsoft.com/office/word/2008/6/28/wordprocessingInk\" xmlns:wne=\"http://schemas.microsoft.com/office/word/2006/wordml\" xmlns:wps=\"http://schemas.microsoft.com/office/word/2008/6/28/wordprocessingShape\" >");
	sResult += m_sFootnotes;
	sResult += _T("</w:footnotes>");
	return sResult;
}

OOXEndnoteWriter::OOXEndnoteWriter( OOXWriter& oWriter,RtfDocument& oDocument ):m_oWriter(oWriter),m_oDocument(oDocument)
{
	m_oRelsWriter = OOXRelsWriterPtr( new OOXRelsWriter( _T("endnotes.xml"), oDocument ) );
	oWriter.m_oCustomRelsWriter.push_back( m_oRelsWriter );
}
void OOXEndnoteWriter::AddEndnoteBegin( std::wstring sType, int nID, std::wstring sText )
{
	std::wstring sEndnote;
	sEndnote += _T("<w:endnote");
	if( false == sType.empty() )
	{
		sEndnote += _T(" w:type=\"");
		sEndnote += sType;
		sEndnote += _T("\"");
	}
	if( -2 != nID )
		sEndnote += L" w:id=\"" + std::to_wstring(nID) + L"\"";
	sEndnote += _T(">");
	sEndnote += sText;
	sEndnote += _T("</w:endnote>");

	m_sEndnotes.insert( m_sEndnotes.begin() , sEndnote.begin(), sEndnote.end() );
}
void OOXEndnoteWriter::AddEndnote( std::wstring sType, int nID, std::wstring sText )
{
	m_sEndnotes += _T("<w:endnote");
	if( false == sType.empty() )
	{
		m_sEndnotes += _T(" w:type=\"");
		m_sEndnotes += sType;
		m_sEndnotes += _T("\"");

	}
	if( -2 != nID )
		m_sEndnotes += L" w:id=\"" + std::to_wstring(nID) + L"\"";
	m_sEndnotes += _T(">");
	m_sEndnotes += sText;
	m_sEndnotes += _T("</w:endnote>");
}
bool OOXEndnoteWriter::Save( std::wstring sFolder )
{
	if( m_sEndnotes.empty() ) return false;

	NSFile::CFileBinary file;
	if (false == file.CreateFile(sFolder + FILE_SEPARATOR_STR + _T("endnotes.xml"))) return false;

	m_oWriter.m_oDocRels.AddRelationship( _T("http://schemas.openxmlformats.org/officeDocument/2006/relationships/endnotes"), _T("endnotes.xml") );
	m_oWriter.m_oContentTypes.AddContent( _T("application/vnd.openxmlformats-officedocument.wordprocessingml.endnotes+xml"), _T("/word/endnotes.xml") );

	std::string sXml = CreateXml();

	file.WriteFile(sXml.c_str(), (DWORD)sXml.length());
	file.CloseFile();
	return true;
}
std::string OOXEndnoteWriter::CreateXml()
{
	std::wstring sResult;

	sResult += _T("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>");
	sResult += _T("<w:endnotes xmlns:wpc=\"http://schemas.microsoft.com/office/word/2008/6/28/wordprocessingCanvas\" xmlns:mc=\"http://schemas.openxmlformats.org/markup-compatibility/2006\" xmlns:o=\"urn:schemas-microsoft-com:office:office\" xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" xmlns:m=\"http://schemas.openxmlformats.org/officeDocument/2006/math\" xmlns:v=\"urn:schemas-microsoft-com:vml\" xmlns:wp14=\"http://schemas.microsoft.com/office/word/2008/9/16/wordprocessingDrawing\" xmlns:wp=\"http://schemas.openxmlformats.org/drawingml/2006/wordprocessingDrawing\" xmlns:w10=\"urn:schemas-microsoft-com:office:word\" xmlns:w=\"http://schemas.openxmlformats.org/wordprocessingml/2006/main\" xmlns:w14=\"http://schemas.microsoft.com/office/word/2009/2/wordml\" xmlns:wpg=\"http://schemas.microsoft.com/office/word/2008/6/28/wordprocessingGroup\" xmlns:wpi=\"http://schemas.microsoft.com/office/word/2008/6/28/wordprocessingInk\" xmlns:wne=\"http://schemas.microsoft.com/office/word/2006/wordml\" xmlns:wps=\"http://schemas.microsoft.com/office/word/2008/6/28/wordprocessingShape\" >");
	sResult +=  m_sEndnotes;
	sResult += _T("</w:endnotes>");

	return  NSFile::CUtf8Converter::GetUtf8StringFromUnicode( sResult);
}
