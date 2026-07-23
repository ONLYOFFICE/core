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

#include "PieceDescriptor.h"
#include "FileInformationBlock.h"

namespace DocFileFormat
{
	class PieceTable
	{
        friend class DocumentMapping;
		friend class MainDocumentMapping;
		friend class HeaderMapping;
		friend class FooterMapping;
		friend class FootnotesMapping;
		friend class EndnotesMapping;
        friend class CommentsMapping;
		friend class Table;
		friend class TextboxMapping;
		friend class NumberingMapping;

	public:
		~PieceTable();
		// Parses the pice table and creates a list of PieceDescriptors.
		PieceTable(FileInformationBlock* fib, POLE::Stream* tableStream, POLE::Stream* wordStream);
		
		std::vector<wchar_t>* GetAllEncodingText(POLE::Stream* wordStream);
		std::vector<wchar_t>* GetEncodingChars(int fcStart, int fcEnd, POLE::Stream* wordStream);
		std::vector<wchar_t>* GetChars (int fcStart, int fcEnd, int cp, POLE::Stream* word);

	private:
		bool ReadSymbolsBuffer(int pos, int size, int coding, POLE::Stream* word, std::vector<wchar_t>* encodingChars);

	public:

		// A list of PieceDescriptor standing for each piece of text.
		std::list<PieceDescriptor> Pieces;
		// A dictionary with character positions as keys and the matching FCs as values
		std::map<int, int>* FileCharacterPositions;
		// A dictionary with file character positions as keys and the matching CPs as values
		std::map<int, int>* CharacterPositions;

		std::list<PieceDescriptor>::iterator	m_carriageIter;
	};
}
