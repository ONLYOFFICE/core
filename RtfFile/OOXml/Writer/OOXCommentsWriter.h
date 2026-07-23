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
