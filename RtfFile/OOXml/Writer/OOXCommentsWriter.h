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
#pragma once

#include "OOXWriter.h"

class OOXCommentsWriter
{
public:
	OOXRelsWriterPtr m_oRelsWriter;
	
	OOXCommentsWriter( OOXWriter& oWriter, RtfDocument& oDocument );

	void SetCommentEnd(const std::wstring & ref); //for author

	void AddComment(  const std::wstring & ref, int nID);
	void AddCommentID( const std::wstring & id);
	void AddCommentAuthor( const std::wstring & author);
	void AddCommentContent( const std::wstring & ref, const std::wstring & paraId, const std::wstring & content);
	void AddCommentParent( const std::wstring & ref, const std::wstring & parent);
	void AddCommentDate( const std::wstring & ref, const std::wstring & date);

	bool Save( std::wstring sFolder );

	struct _comment
	{
		_comment(int id) : nID(id) {}
		int nID = 0;
		int nParentID = 0;
		std::wstring author;
		std::wstring date;
		std::wstring content;
		std::wstring authorId;
		std::wstring paraId;

	};
	std::map<std::wstring, _comment> m_mapComments;

private: 
	RtfDocument& m_oDocument;
	OOXWriter& m_oWriter;

	std::wstring						m_sCurrent_ref;
	std::wstring						m_sCommentsExtended;
	std::map<int, std::wstring>			m_mapCommentsParent;

	std::wstring CreateXml();
	std::wstring CreateXmlExtended();
};
