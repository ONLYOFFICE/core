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
		PieceTable(FileInformationBlock* fib, POLE::Stream* tableStream);
		std::vector<wchar_t>* GetAllEncodingText(POLE::Stream* wordStream);
		std::vector<wchar_t>* GetEncodingChars(int fcStart, int fcEnd, POLE::Stream* wordStream);
		std::vector<wchar_t>* GetChars (int fcStart, int fcEnd, int cp, POLE::Stream* word);

	private:
		bool ReadSymbolsBuffer(int pos, int size, ASCDocFormatUtils::Encoding encoding, POLE::Stream* word, std::vector<wchar_t>* encodingChars);

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
