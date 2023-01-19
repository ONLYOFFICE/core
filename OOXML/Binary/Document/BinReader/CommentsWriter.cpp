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

#include "CommentsWriter.h"

namespace Writers
{
	CommentsWriter::CommentsWriter(std::wstring sDir) : m_sDir(sDir)
	{
	}
	void CommentsWriter::setElements(std::wstring& sComment, std::wstring& sCommentExt, std::wstring& sContentExtensible, std::wstring& sCommentsIds, std::wstring& sPeople, std::wstring& sDocumentComment, std::wstring& sDocumentCommentExt, std::wstring& sDocumentContentExtensible, std::wstring& sDocumentCommentsIds, std::wstring& sDocumentPeople,std::wstring&  sContentUserData)
	{
		m_sComment		= sComment;
		m_sCommentExt	= sCommentExt;
		m_sContentExtensible	= sContentExtensible;
		m_sCommentsIds	= sCommentsIds;
		m_sPeople		= sPeople;
		m_sDocumentComment		= sDocumentComment;
		m_sDocumentCommentExt	= sDocumentCommentExt;
		m_sDocumentContentExtensible	= sDocumentContentExtensible;
		m_sDocumentCommentsIds	= sDocumentCommentsIds;
		m_sDocumentPeople		= sDocumentPeople;
		m_sContentUserData		= sContentUserData;
	}
	std::wstring CommentsWriter::getFilename()
	{
		return OOX::FileTypes::Comments.DefaultFileName().GetPath();
	}
	void CommentsWriter::Write(bool bGlossary)
	{
		std::wstring sDir = m_sDir + FILE_SEPARATOR_STR + L"word" + (bGlossary ? (FILE_SEPARATOR_STR + std::wstring(L"glossary")) : L"" ) + FILE_SEPARATOR_STR;
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
		if(false == m_sContentExtensible.empty())
		{
			NSFile::CFileBinary oFile;
			oFile.CreateFileW(sDir + OOX::FileTypes::CommentsExtensible.DefaultFileName().GetPath());
			oFile.WriteStringUTF8(g_string_commentExtensible_Start);
			oFile.WriteStringUTF8(m_sContentExtensible);
			oFile.WriteStringUTF8(g_string_commentExtensible_End);
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
		if(false == m_sDocumentContentExtensible.empty())
		{
			NSFile::CFileBinary oFile;
			oFile.CreateFileW(sDir + OOX::FileTypes::DocumentCommentsExtensible.DefaultFileName().GetPath());
			oFile.WriteStringUTF8(g_string_commentExtensible_Start);
			oFile.WriteStringUTF8(m_sDocumentContentExtensible);
			oFile.WriteStringUTF8(g_string_commentExtensible_End);
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
		if(false == m_sContentUserData.empty())
		{
			NSFile::CFileBinary oFile;
			oFile.CreateFileW(sDir + OOX::FileTypes::CommentsUserData.DefaultFileName().GetPath());
			oFile.WriteStringUTF8(g_string_commentExtensible_Start);
			oFile.WriteStringUTF8(m_sContentUserData);
			oFile.WriteStringUTF8(g_string_commentExtensible_End);
			oFile.CloseFile();
		}
	}
}
