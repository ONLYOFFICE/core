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

#include "OOXCommentsWriter.h"
	
	OOXCommentsWriter::OOXCommentsWriter( OOXWriter& oWriter, RtfDocument& oDocument ) : m_oWriter(oWriter), m_oDocument(oDocument)
	{
		m_oRelsWriter = OOXRelsWriterPtr( new OOXRelsWriter( _T("comments.xml"), oDocument ) );
		oWriter.m_oCustomRelsWriter.push_back( m_oRelsWriter );
	}
	void OOXCommentsWriter::SetCommentEnd(const std::wstring & ref) //for author
	{
		m_sCurrent_ref = ref;
	}
	void OOXCommentsWriter::AddComment(  const std::wstring & ref, int nID)
	{
		_comment comment(nID);
		m_mapComments.insert(std::make_pair(ref, comment));
	}
	void OOXCommentsWriter::AddCommentID( const std::wstring & id)
	{
		std::map<std::wstring, _comment>::iterator pFind = m_mapComments.find(m_sCurrent_ref);

		if (pFind != m_mapComments.end())
		{
			pFind->second.authorId = id;
		}
	}
	void OOXCommentsWriter::AddCommentAuthor( const std::wstring & author)
	{
		std::map<std::wstring, _comment>::iterator pFind = m_mapComments.find(m_sCurrent_ref);

		if (pFind != m_mapComments.end())
		{
			pFind->second.author = author;
		}
	}
	void OOXCommentsWriter::AddCommentContent( const std::wstring & ref, const std::wstring & paraId, const std::wstring & content)
	{
		std::map<std::wstring, _comment>::iterator pFind = m_mapComments.find(ref);

		if (pFind != m_mapComments.end())
		{
			pFind->second.content	= content;
			pFind->second.paraId	= paraId;

			m_mapCommentsParent.insert(std::make_pair(pFind->second.nID, paraId));
		}
	}
	void OOXCommentsWriter::AddCommentParent( const std::wstring & ref, const std::wstring & parent)
	{
		std::map<std::wstring, _comment>::iterator pFind = m_mapComments.find(ref);

		if (pFind != m_mapComments.end())
		{
			pFind->second.nParentID = boost::lexical_cast<int>(parent);
		}
	}
	void OOXCommentsWriter::AddCommentDate( const std::wstring & ref, const std::wstring & date)
	{
		std::map<std::wstring, _comment>::iterator pFind = m_mapComments.find(ref);

		if (pFind != m_mapComments.end())
		{
			pFind->second.date = date;
		}
	}
	bool OOXCommentsWriter::Save( std::wstring sFolder )
	{
		if(  m_mapComments.empty() ) return false;

		NSFile::CFileBinary file;
        if (false == file.CreateFile(sFolder + FILE_SEPARATOR_STR + _T("comments.xml"))) return false;
		
		m_oWriter.m_oDocRels.AddRelationship( _T("http://schemas.openxmlformats.org/officeDocument/2006/relationships/comments"), _T("comments.xml") );
		m_oWriter.m_oContentTypes.AddContent( _T("application/vnd.openxmlformats-officedocument.wordprocessingml.comments+xml"), _T("/word/comments.xml") );

         std::wstring sXml = CreateXml();
		
         std::string sXmlUTF = NSFile::CUtf8Converter::GetUtf8StringFromUnicode(sXml);

         file.WriteFile((void*)sXmlUTF.c_str(), (DWORD)sXmlUTF.length());

		 file.CloseFile();
		//-------------------------------------------------------------------------------------------------------------------------
        if (false == file.CreateFile(sFolder + FILE_SEPARATOR_STR + L"commentsExtended.xml")) return false;
		
		m_oWriter.m_oDocRels.AddRelationship( L"http://schemas.microsoft.com/office/2011/relationships/commentsExtended", L"commentsExtended.xml" );
		m_oWriter.m_oContentTypes.AddContent( L"application/vnd.openxmlformats-officedocument.wordprocessingml.commentsExtended+xml", L"/word/commentsExtended.xml" );

         sXml = CreateXmlExtended();
		
		sXmlUTF = NSFile::CUtf8Converter::GetUtf8StringFromUnicode(sXml);

         file.WriteFile((void*)sXmlUTF.c_str(), (DWORD)sXmlUTF.length());

		 file.CloseFile();
		 return true;
	}	

	std::wstring OOXCommentsWriter::CreateXml()
	{
        std::wstring sResult = L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n";

		sResult += L"<w:comments \
xmlns:wpc=\"http://schemas.microsoft.com/office/word/2010/wordprocessingCanvas\" \
xmlns:mc=\"http://schemas.openxmlformats.org/markup-compatibility/2006\" \
xmlns:o=\"urn:schemas-microsoft-com:office:office\" \
xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" \
xmlns:m=\"http://schemas.openxmlformats.org/officeDocument/2006/math\" \
xmlns:v=\"urn:schemas-microsoft-com:vml\" \
xmlns:wp14=\"http://schemas.microsoft.com/office/word/2010/wordprocessingDrawing\" \
xmlns:wp=\"http://schemas.openxmlformats.org/drawingml/2006/wordprocessingDrawing\" \
xmlns:w10=\"urn:schemas-microsoft-com:office:word\" \
xmlns:w=\"http://schemas.openxmlformats.org/wordprocessingml/2006/main\" \
xmlns:w14=\"http://schemas.microsoft.com/office/word/2010/wordml\" \
xmlns:w15=\"http://schemas.microsoft.com/office/word/2012/wordml\" \
xmlns:wpg=\"http://schemas.microsoft.com/office/word/2010/wordprocessingGroup\" \
xmlns:wpi=\"http://schemas.microsoft.com/office/word/2010/wordprocessingInk\" \
xmlns:wne=\"http://schemas.microsoft.com/office/word/2006/wordml\" \
xmlns:wps=\"http://schemas.microsoft.com/office/word/2010/wordprocessingShape\" \
mc:Ignorable=\"w14 w15 wp14\">";

		for (std::map<std::wstring, _comment>::iterator it = m_mapComments.begin(); it != m_mapComments.end(); ++it)
		{
			sResult += L"<w:comment w:id=\"" + std::to_wstring(it->second.nID) + L"\" w:author=\"" + 
				it->second.author + L"\" w:date=\"" + it->second.date + L"\" w:initials=\"" + it->second.authorId + L"\">";
			sResult += it->second.content;

			sResult += L"</w:comment>";
//--------------------------------------------------------
			m_sCommentsExtended += L"<w15:commentEx w15:paraId=\"" + it->second.paraId + L"\"";
			if (it->second.nParentID != 0)
			{
				it->second.nParentID = it->second.nID + it->second.nParentID;

				std::map<int, std::wstring>::iterator pFind = m_mapCommentsParent.find(it->second.nParentID);

				if (pFind != m_mapCommentsParent.end())
				{
					 m_sCommentsExtended += L" w15:paraIdParent=\"" + pFind->second + L"\"";
				}
			}
			m_sCommentsExtended += L" w15:done=\"0\"/>";

		}
		sResult += L"</w:comments>";
		return sResult;
	}
	std::wstring OOXCommentsWriter::CreateXmlExtended()
	{
        std::wstring sResult;
        sResult += L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n";

		sResult += L"<w15:commentsEx \
xmlns:wpc=\"http://schemas.microsoft.com/office/word/2010/wordprocessingCanvas\" \
xmlns:cx=\"http://schemas.microsoft.com/office/drawing/2014/chartex\" \
xmlns:cx1=\"http://schemas.microsoft.com/office/drawing/2015/9/8/chartex\" \
xmlns:mc=\"http://schemas.openxmlformats.org/markup-compatibility/2006\" \
xmlns:o=\"urn:schemas-microsoft-com:office:office\" \
xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" \
xmlns:m=\"http://schemas.openxmlformats.org/officeDocument/2006/math\" \
xmlns:v=\"urn:schemas-microsoft-com:vml\" \
xmlns:wp14=\"http://schemas.microsoft.com/office/word/2010/wordprocessingDrawing\" \
xmlns:wp=\"http://schemas.openxmlformats.org/drawingml/2006/wordprocessingDrawing\" \
xmlns:w10=\"urn:schemas-microsoft-com:office:word\" \
xmlns:w=\"http://schemas.openxmlformats.org/wordprocessingml/2006/main\" \
xmlns:w14=\"http://schemas.microsoft.com/office/word/2010/wordml\" \
xmlns:w15=\"http://schemas.microsoft.com/office/word/2012/wordml\" \
xmlns:w16se=\"http://schemas.microsoft.com/office/word/2015/wordml/symex\" \
xmlns:wpg=\"http://schemas.microsoft.com/office/word/2010/wordprocessingGroup\" \
xmlns:wpi=\"http://schemas.microsoft.com/office/word/2010/wordprocessingInk\" \
xmlns:wne=\"http://schemas.microsoft.com/office/word/2006/wordml\" \
xmlns:wps=\"http://schemas.microsoft.com/office/word/2010/wordprocessingShape\" \
mc:Ignorable=\"w14 w15 w16se wp14\">";
		sResult += m_sCommentsExtended;
		sResult += L"</w15:commentsEx>";
		return sResult;
	}
