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

#include "HeaderAndFooterTable.h"

#include <algorithm>

namespace DocFileFormat
{
	HeaderAndFooterTable::HeaderAndFooterTable (FileInformationBlock* fib, POLE::Stream* pTableStream)
	{
		VirtualStreamReader tableReader (pTableStream, fib->m_FibWord97.fcPlcfHdd);

		unsigned int tableSize	=	fib->m_FibWord97.lcbPlcfHdd / 4;

		if ( ( tableSize > 0 ) && ( fib->m_RgLw97.ccpHdr > 0 ) )
		{
			int* table			=	new int[tableSize];

			for (unsigned int i = 0; i < tableSize; ++i)
			{
				table[i]		=	tableReader.ReadInt32();
			}

			int count			=	( tableSize - 8 ) / 6;

			int initialPos		=	fib->m_RgLw97.ccpText + fib->m_RgLw97.ccpFtn;

			//the first 6 _entries are about footnote and endnote formatting
			//so skip these _entries
			int pos = 6;

			for (int i = 0; i < count; ++i)
			{
				//Even Header
				if ( table[pos] == table[pos + 1] )
				{
					m_arEvenHeaders.push_back( NULL );
				}
				else
				{
					m_arEvenHeaders.push_back( new CharacterRange( ( initialPos + table[pos] ), ( table[pos + 1] - table[pos] ) ) );
				}

				pos++;

				//Odd Header
				if ( table[pos] == table[pos + 1] )
				{
					m_arOddHeaders.push_back( NULL );
				}
				else
				{
					m_arOddHeaders.push_back( new CharacterRange( ( initialPos + table[pos] ), ( table[pos + 1] - table[pos] ) ) );
				}

				pos++;

				//Even Footer
				if ( table[pos] == table[pos + 1] )
				{
					m_arEvenFooters.push_back( NULL );
				}
				else
				{
					m_arEvenFooters.push_back( new CharacterRange( ( initialPos + table[pos] ), ( table[pos + 1] - table[pos] ) ) );
				}

				pos++;

				//Odd Footer
				if ( table[pos] == table[pos + 1] )
				{
					m_arOddFooters.push_back( NULL );
				}
				else
				{
					m_arOddFooters.push_back( new CharacterRange( ( initialPos + table[pos] ), ( table[pos + 1] - table[pos] ) ) );
				}

				pos++;

				//First Page Header
				if ( table[pos] == table[pos + 1] )
				{
					m_arFirstHeaders.push_back (NULL);
				}
				else
				{
					m_arFirstHeaders.push_back (new CharacterRange( ( initialPos + table[pos] ), ( table[pos + 1] - table[pos] ) ));
				}

				pos++;

				//First Page Footers
				if ( table[pos] == table[pos + 1] )
				{
					m_arFirstFooters.push_back( NULL );
				}
				else
				{
					m_arFirstFooters.push_back( new CharacterRange( ( initialPos + table[pos] ), ( table[pos + 1] - table[pos] ) ) );
				}

				pos++;
			}

			RELEASEARRAYOBJECTS(table);
		}
	}

	HeaderAndFooterTable::~HeaderAndFooterTable()
	{
		for_each (m_arEvenFooters.begin(),  m_arEvenFooters.end(),  DeleteDynamicObject());
		for_each (m_arEvenHeaders.begin(),  m_arEvenHeaders.end(),  DeleteDynamicObject());
		for_each (m_arFirstFooters.begin(), m_arFirstFooters.end(), DeleteDynamicObject());
		for_each (m_arFirstHeaders.begin(), m_arFirstHeaders.end(), DeleteDynamicObject());
		for_each (m_arOddFooters.begin(),   m_arOddFooters.end(),   DeleteDynamicObject());
		for_each (m_arOddHeaders.begin(),   m_arOddHeaders.end(),   DeleteDynamicObject());
	}
}