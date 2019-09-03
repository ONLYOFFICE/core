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
#ifndef COMMENTS_WRITER
#define COMMENTS_WRITER

#include "../../XlsxSerializerCom/Common/Common.h"
#include "../../Common/DocxFormat/Source/DocxFormat/FileTypes.h"

namespace Writers
{
    static std::wstring g_string_comment_Start = _T("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?><w:comments xmlns:wpc=\"http://schemas.microsoft.com/office/word/2010/wordprocessingCanvas\" xmlns:mc=\"http://schemas.openxmlformats.org/markup-compatibility/2006\" xmlns:o=\"urn:schemas-microsoft-com:office:office\" xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" xmlns:m=\"http://schemas.openxmlformats.org/officeDocument/2006/math\" xmlns:v=\"urn:schemas-microsoft-com:vml\" xmlns:wp14=\"http://schemas.microsoft.com/office/word/2010/wordprocessingDrawing\" xmlns:wp=\"http://schemas.openxmlformats.org/drawingml/2006/wordprocessingDrawing\" xmlns:w10=\"urn:schemas-microsoft-com:office:word\" xmlns:w=\"http://schemas.openxmlformats.org/wordprocessingml/2006/main\" xmlns:w14=\"http://schemas.microsoft.com/office/word/2010/wordml\" xmlns:w15=\"http://schemas.microsoft.com/office/word/2012/wordml\" xmlns:wpg=\"http://schemas.microsoft.com/office/word/2010/wordprocessingGroup\" xmlns:wpi=\"http://schemas.microsoft.com/office/word/2010/wordprocessingInk\" xmlns:wne=\"http://schemas.microsoft.com/office/word/2006/wordml\" xmlns:wps=\"http://schemas.microsoft.com/office/word/2010/wordprocessingShape\" mc:Ignorable=\"w14 w15 wp14\">");
    static std::wstring g_string_comment_End = _T("</w:comments>");
    static std::wstring g_string_commentExt_Start = _T("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?><w15:commentsEx xmlns:wpc=\"http://schemas.microsoft.com/office/word/2010/wordprocessingCanvas\" xmlns:mc=\"http://schemas.openxmlformats.org/markup-compatibility/2006\" xmlns:o=\"urn:schemas-microsoft-com:office:office\" xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" xmlns:m=\"http://schemas.openxmlformats.org/officeDocument/2006/math\" xmlns:v=\"urn:schemas-microsoft-com:vml\" xmlns:wp14=\"http://schemas.microsoft.com/office/word/2010/wordprocessingDrawing\" xmlns:wp=\"http://schemas.openxmlformats.org/drawingml/2006/wordprocessingDrawing\" xmlns:w10=\"urn:schemas-microsoft-com:office:word\" xmlns:w=\"http://schemas.openxmlformats.org/wordprocessingml/2006/main\" xmlns:w14=\"http://schemas.microsoft.com/office/word/2010/wordml\" xmlns:w15=\"http://schemas.microsoft.com/office/word/2012/wordml\" xmlns:wpg=\"http://schemas.microsoft.com/office/word/2010/wordprocessingGroup\" xmlns:wpi=\"http://schemas.microsoft.com/office/word/2010/wordprocessingInk\" xmlns:wne=\"http://schemas.microsoft.com/office/word/2006/wordml\" xmlns:wps=\"http://schemas.microsoft.com/office/word/2010/wordprocessingShape\" mc:Ignorable=\"w14 w15 wp14\">");
    static std::wstring g_string_commentExt_End = _T("</w15:commentsEx>");
	static std::wstring g_string_commentsIds_Start = _T("<w16cid:commentsIds mc:Ignorable=\"w16cid\" xmlns:mc=\"http://schemas.openxmlformats.org/markup-compatibility/2006\" xmlns:w16cid=\"http://schemas.microsoft.com/office/word/2016/wordml/cid\">");
	static std::wstring g_string_commentsIds_End = _T("</w16cid:commentsIds>");
    static std::wstring g_string_people_Start = _T("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?><w15:people xmlns:wpc=\"http://schemas.microsoft.com/office/word/2010/wordprocessingCanvas\" xmlns:mc=\"http://schemas.openxmlformats.org/markup-compatibility/2006\" xmlns:o=\"urn:schemas-microsoft-com:office:office\" xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" xmlns:m=\"http://schemas.openxmlformats.org/officeDocument/2006/math\" xmlns:v=\"urn:schemas-microsoft-com:vml\" xmlns:wp14=\"http://schemas.microsoft.com/office/word/2010/wordprocessingDrawing\" xmlns:wp=\"http://schemas.openxmlformats.org/drawingml/2006/wordprocessingDrawing\" xmlns:w10=\"urn:schemas-microsoft-com:office:word\" xmlns:w=\"http://schemas.openxmlformats.org/wordprocessingml/2006/main\" xmlns:w14=\"http://schemas.microsoft.com/office/word/2010/wordml\" xmlns:w15=\"http://schemas.microsoft.com/office/word/2012/wordml\" xmlns:wpg=\"http://schemas.microsoft.com/office/word/2010/wordprocessingGroup\" xmlns:wpi=\"http://schemas.microsoft.com/office/word/2010/wordprocessingInk\" xmlns:wne=\"http://schemas.microsoft.com/office/word/2006/wordml\" xmlns:wps=\"http://schemas.microsoft.com/office/word/2010/wordprocessingShape\" mc:Ignorable=\"w14 w15 wp14\">");
    static std::wstring g_string_people_End = _T("</w15:people>");

	class CommentsWriter
	{
        std::wstring	m_sDir;
	public:
        std::wstring	m_sComment;
        std::wstring	m_sCommentExt;
		std::wstring	m_sCommentsIds;
        std::wstring	m_sPeople;
		std::wstring	m_sDocumentComment;
		std::wstring	m_sDocumentCommentExt;
		std::wstring	m_sDocumentCommentsIds;
		std::wstring	m_sDocumentPeople;

		CommentsWriter(std::wstring sDir) : m_sDir(sDir)
		{
		}
		void setElements(std::wstring& sComment, std::wstring& sCommentExt, std::wstring& sCommentsIds, std::wstring& sPeople, std::wstring& sDocumentComment, std::wstring& sDocumentCommentExt, std::wstring& sDocumentCommentsIds, std::wstring& sDocumentPeople)
		{
			m_sComment		= sComment;
			m_sCommentExt	= sCommentExt;
			m_sCommentsIds	= sCommentsIds;
			m_sPeople		= sPeople;
			m_sDocumentComment		= sDocumentComment;
			m_sDocumentCommentExt	= sDocumentCommentExt;
			m_sDocumentCommentsIds	= sDocumentCommentsIds;
			m_sDocumentPeople		= sDocumentPeople;
		}
		void Write()
		{
			std::wstring sDir = m_sDir + FILE_SEPARATOR_STR + _T("word") + FILE_SEPARATOR_STR;
            if(false == m_sComment.empty())
			{
				OOX::CPath filePath = sDir + OOX::FileTypes::Comments.DefaultFileName().GetPath();

				NSFile::CFileBinary oFile;
				oFile.CreateFileW(filePath.GetPath());
					oFile.WriteStringUTF8(g_string_comment_Start);
					oFile.WriteStringUTF8(m_sComment);
					oFile.WriteStringUTF8(g_string_comment_End);
				oFile.CloseFile();
			}
            if(false == m_sCommentExt.empty())
			{
				NSFile::CFileBinary oFile;
				oFile.CreateFileW(sDir + OOX::FileTypes::CommentsExt.DefaultFileName().GetPath());
				oFile.WriteStringUTF8(g_string_commentExt_Start);
				oFile.WriteStringUTF8(m_sCommentExt);
				oFile.WriteStringUTF8(g_string_commentExt_End);
				oFile.CloseFile();
			}
			if(false == m_sCommentsIds.empty())
			{
				NSFile::CFileBinary oFile;
				oFile.CreateFileW(sDir + OOX::FileTypes::CommentsIds.DefaultFileName().GetPath());
				oFile.WriteStringUTF8(g_string_commentsIds_Start);
				oFile.WriteStringUTF8(m_sCommentsIds);
				oFile.WriteStringUTF8(g_string_commentsIds_End);
				oFile.CloseFile();
			}
            if(false == m_sPeople.empty())
			{
				NSFile::CFileBinary oFile;
				oFile.CreateFileW(sDir + OOX::FileTypes::People.DefaultFileName().GetPath());
				oFile.WriteStringUTF8(g_string_people_Start);
				oFile.WriteStringUTF8(m_sPeople);
				oFile.WriteStringUTF8(g_string_people_End);
				oFile.CloseFile();
			}

			if(false == m_sDocumentComment.empty())
			{
				OOX::CPath filePath = sDir + OOX::FileTypes::DocumentComments.DefaultFileName().GetPath();

				NSFile::CFileBinary oFile;
				oFile.CreateFileW(filePath.GetPath());
					oFile.WriteStringUTF8(g_string_comment_Start);
					oFile.WriteStringUTF8(m_sDocumentComment);
					oFile.WriteStringUTF8(g_string_comment_End);
				oFile.CloseFile();
			}
			if(false == m_sDocumentCommentExt.empty())
			{
				NSFile::CFileBinary oFile;
				oFile.CreateFileW(sDir + OOX::FileTypes::DocumentCommentsExt.DefaultFileName().GetPath());
				oFile.WriteStringUTF8(g_string_commentExt_Start);
				oFile.WriteStringUTF8(m_sDocumentCommentExt);
				oFile.WriteStringUTF8(g_string_commentExt_End);
				oFile.CloseFile();
			}
			if(false == m_sDocumentCommentsIds.empty())
			{
				NSFile::CFileBinary oFile;
				oFile.CreateFileW(sDir + OOX::FileTypes::DocumentCommentsIds.DefaultFileName().GetPath());
				oFile.WriteStringUTF8(g_string_commentsIds_Start);
				oFile.WriteStringUTF8(m_sDocumentCommentsIds);
				oFile.WriteStringUTF8(g_string_commentsIds_End);
				oFile.CloseFile();
			}
			if(false == m_sDocumentPeople.empty())
			{
				NSFile::CFileBinary oFile;
				oFile.CreateFileW(sDir + OOX::FileTypes::DocumentPeople.DefaultFileName().GetPath());
				oFile.WriteStringUTF8(g_string_people_Start);
				oFile.WriteStringUTF8(m_sDocumentPeople);
				oFile.WriteStringUTF8(g_string_people_End);
				oFile.CloseFile();
			}

		}
	};
}
#endif	// #ifndef COMMENTS_WRITER
