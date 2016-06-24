/*
 * (c) Copyright Ascensio System SIA 2010-2016
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
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
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
#pragma once

#include "OOXWriter.h"

class OOXFootnoteWriter
{
public:
	OOXRelsWriterPtr m_oRelsWriter;
	OOXFootnoteWriter( OOXWriter& oWriter,RtfDocument& oDocument ):m_oWriter(oWriter),m_oDocument(oDocument)
	{
		m_oRelsWriter = OOXRelsWriterPtr( new OOXRelsWriter( _T("footnotes.xml"), oDocument ) );
		oWriter.m_oCustomRelsWriter.push_back( m_oRelsWriter );
	}
	void AddFootnoteBegin( CString sType, int nID, CString sText )
	{
		CString sFootnote;
		sFootnote.Append( _T("<w:footnote") );
		if( false == sType.IsEmpty() )
		{
			sFootnote.Append(_T(" w:type=\"") );
			sFootnote += sType;
			sFootnote.Append(_T("\""));

		}
		if( PROP_DEF != nID )
			sFootnote.AppendFormat( _T(" w:id=\"%d\""), nID );
		sFootnote.Append( _T(">") );
		sFootnote.Append( sText );
		sFootnote.Append(_T("</w:footnote>"));

		m_sFootnotes.Insert( 0 , sFootnote );
	}
	void AddFootnote( CString sType, int nID, CString sText )
	{
		m_sFootnotes.Append( _T("<w:footnote") );
		if( false == sType.IsEmpty() )
		{
			m_sFootnotes.Append(_T(" w:type=\"") );
			m_sFootnotes += sType;
			m_sFootnotes.Append(_T("\""));
		}
		if( PROP_DEF != nID )
		{
			m_sFootnotes.AppendFormat( _T(" w:id=\"%d\""), nID );
		}
		m_sFootnotes.Append( _T(">") );
		m_sFootnotes.Append( sText );
		m_sFootnotes.Append(_T("</w:footnote>"));
	}
	bool Save( CString sFolder )
	{
		if(  m_sFootnotes.IsEmpty() ) return false;

		CFile file;
        if (file.CreateFile(sFolder + FILE_SEPARATOR_STR + _T("footnotes.xml"))) return false;
		
		m_oWriter.m_oDocRels.AddRelationship( _T("http://schemas.openxmlformats.org/officeDocument/2006/relationships/footnotes"), _T("footnotes.xml") );
		m_oWriter.m_oContentTypes.AddContent( _T("application/vnd.openxmlformats-officedocument.wordprocessingml.footnotes+xml"), _T("/word/footnotes.xml") );

		 CString sXml = CreateXml();
		
         std::string sXmlUTF = NSFile::CUtf8Converter::GetUtf8StringFromUnicode(sXml.GetBuffer());

         file.WriteFile((void*)sXmlUTF.c_str(), sXmlUTF.length());

		 file.CloseFile();
		 return true;
	}
private: 
	RtfDocument& m_oDocument;
	OOXWriter& m_oWriter;

    CString m_sFootnotes;

    CString CreateXml()
	{
		CString sResult;
		sResult.Append( _T("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>") );
		sResult.AppendChar('\n');
		sResult.Append( _T("<w:footnotes xmlns:wpc=\"http://schemas.microsoft.com/office/word/2008/6/28/wordprocessingCanvas\" xmlns:mc=\"http://schemas.openxmlformats.org/markup-compatibility/2006\" xmlns:o=\"urn:schemas-microsoft-com:office:office\" xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" xmlns:m=\"http://schemas.openxmlformats.org/officeDocument/2006/math\" xmlns:v=\"urn:schemas-microsoft-com:vml\" xmlns:wp14=\"http://schemas.microsoft.com/office/word/2008/9/16/wordprocessingDrawing\" xmlns:wp=\"http://schemas.openxmlformats.org/drawingml/2006/wordprocessingDrawing\" xmlns:w10=\"urn:schemas-microsoft-com:office:word\" xmlns:w=\"http://schemas.openxmlformats.org/wordprocessingml/2006/main\" xmlns:w14=\"http://schemas.microsoft.com/office/word/2009/2/wordml\" xmlns:wpg=\"http://schemas.microsoft.com/office/word/2008/6/28/wordprocessingGroup\" xmlns:wpi=\"http://schemas.microsoft.com/office/word/2008/6/28/wordprocessingInk\" xmlns:wne=\"http://schemas.microsoft.com/office/word/2006/wordml\" xmlns:wps=\"http://schemas.microsoft.com/office/word/2008/6/28/wordprocessingShape\" >") );
		sResult.Append( m_sFootnotes );
		sResult.Append( _T("</w:footnotes>") );
		return sResult;
	}

};
class OOXEndnoteWriter
{
public: 
	OOXRelsWriterPtr m_oRelsWriter;
	OOXEndnoteWriter( OOXWriter& oWriter,RtfDocument& oDocument ):m_oWriter(oWriter),m_oDocument(oDocument)
	{
		m_oRelsWriter = OOXRelsWriterPtr( new OOXRelsWriter( _T("endnotes.xml"), oDocument ) );
		oWriter.m_oCustomRelsWriter.push_back( m_oRelsWriter );
	}
	void AddEndnoteBegin( CString sType, int nID, CString sText )
	{
		CString sEndnote;
		sEndnote.Append( _T("<w:endnote") );
		if( false == sType.IsEmpty() )
		{
			sEndnote.Append(_T(" w:type=\"") );
			sEndnote += sType;
			sEndnote.Append(_T("\""));
		}
		if( -2 != nID )
			sEndnote.AppendFormat( _T(" w:id=\"%d\""), nID );
		sEndnote.Append( _T(">") );
		sEndnote.Append( sText );
		sEndnote.Append(_T("</w:endnote>") );

		m_sEndnotes.Insert( 0 , sEndnote );
	}
	void AddEndnote( CString sType, int nID, CString sText )
	{
		m_sEndnotes.Append( _T("<w:endnote") );
		if( false == sType.IsEmpty() )
		{
			m_sEndnotes.Append(_T(" w:type=\"") );
			m_sEndnotes += sType;
			m_sEndnotes.Append(_T("\""));

		}
		if( -2 != nID )
			m_sEndnotes.AppendFormat( _T(" w:id=\"%d\""), nID );
		m_sEndnotes.Append( _T(">") );
		m_sEndnotes.Append( sText );
		m_sEndnotes.Append(_T("</w:endnote>") );
	}
	bool Save( CString sFolder )
	{
		if( m_sEndnotes.IsEmpty() ) return false;

		CFile file;
        if (file.CreateFile(sFolder + FILE_SEPARATOR_STR + _T("endnotes.xml"))) return false;

		m_oWriter.m_oDocRels.AddRelationship( _T("http://schemas.openxmlformats.org/officeDocument/2006/relationships/endnotes"), _T("endnotes.xml") );
		m_oWriter.m_oContentTypes.AddContent( _T("application/vnd.openxmlformats-officedocument.wordprocessingml.endnotes+xml"), _T("/word/endnotes.xml") );

        std::string sXml = CreateXml();

        file.WriteFile((void*)sXml.c_str(), sXml.length());
		file.CloseFile();
		return true;
	}
private: 
	RtfDocument& m_oDocument;
	OOXWriter& m_oWriter;
	CString m_sEndnotes;

    std::string CreateXml()
	{
        std::wstring sResult;
        sResult.append( _T("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>") );
        sResult.append( _T("<w:endnotes xmlns:wpc=\"http://schemas.microsoft.com/office/word/2008/6/28/wordprocessingCanvas\" xmlns:mc=\"http://schemas.openxmlformats.org/markup-compatibility/2006\" xmlns:o=\"urn:schemas-microsoft-com:office:office\" xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" xmlns:m=\"http://schemas.openxmlformats.org/officeDocument/2006/math\" xmlns:v=\"urn:schemas-microsoft-com:vml\" xmlns:wp14=\"http://schemas.microsoft.com/office/word/2008/9/16/wordprocessingDrawing\" xmlns:wp=\"http://schemas.openxmlformats.org/drawingml/2006/wordprocessingDrawing\" xmlns:w10=\"urn:schemas-microsoft-com:office:word\" xmlns:w=\"http://schemas.openxmlformats.org/wordprocessingml/2006/main\" xmlns:w14=\"http://schemas.microsoft.com/office/word/2009/2/wordml\" xmlns:wpg=\"http://schemas.microsoft.com/office/word/2008/6/28/wordprocessingGroup\" xmlns:wpi=\"http://schemas.microsoft.com/office/word/2008/6/28/wordprocessingInk\" xmlns:wne=\"http://schemas.microsoft.com/office/word/2006/wordml\" xmlns:wps=\"http://schemas.microsoft.com/office/word/2008/6/28/wordprocessingShape\" >") );
        sResult.append( m_sEndnotes.GetBuffer() );
        sResult.append( _T("</w:endnotes>") );

        return  NSFile::CUtf8Converter::GetUtf8StringFromUnicode( sResult);
	}
};
