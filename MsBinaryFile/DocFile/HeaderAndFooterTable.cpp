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

#include "HeaderAndFooterTable.h"

#include <algorithm>

namespace DocFileFormat
{
	HeaderAndFooterTable::HeaderAndFooterTable (FileInformationBlock* fib, POLE::Stream* pTableStream)
	{
		m_nCurrentIndex = 0;

		VirtualStreamReader tableReader (pTableStream, fib->m_FibWord97.fcPlcfHdd, fib->m_nWordVersion);

		if (fib->m_FibWord97.fcPlcfHdd > tableReader.GetSize()) return;

		unsigned int tableSize	=	fib->m_FibWord97.lcbPlcfHdd / 4;//in bytes

		if ( tableSize < 1  || fib->m_RgLw97.ccpHdr <1 )
			return;

		int* table = new int[tableSize];

		for (unsigned int i = 0; i < tableSize; ++i)
		{
			table[i] = tableReader.ReadInt32();
		}

		int initialPos = fib->m_RgLw97.ccpText + fib->m_RgLw97.ccpFtn;

		int count	=	0;
		int pos		= (fib->m_FibBase.fComplex || fib->m_nWordVersion == 0) ? 6 : 0;
				//the first 6 _entries are about footnote and endnote formatting -Word97 so skip these		

		std::vector<std::vector<CharacterRange*>*> arHeadersFooters;
		
		if (fib->m_nWordVersion == 2)
		{
			count =	( tableSize - 1);
		}
		else
		{
			count =	( tableSize - pos - 1) / 6;

			arHeadersFooters.push_back(&m_arEvenHeaders);
			arHeadersFooters.push_back(&m_arOddHeaders);
			arHeadersFooters.push_back(&m_arEvenFooters);
			arHeadersFooters.push_back(&m_arOddFooters);
			arHeadersFooters.push_back(&m_arFirstHeaders);
			arHeadersFooters.push_back(&m_arFirstFooters);
		}
		
		for (int i = 0; i < count; ++i)
		{
	//Even Header
			if (fib->m_nWordVersion == 2)
			{
				if ( table[pos] == table[pos + 1] )
				{
					m_arCommonHeadersFooters.push_back( NULL );
				}
				else
				{
					m_arCommonHeadersFooters.push_back( new CharacterRange( initialPos + table[pos], table[pos + 1] - table[pos]) );
				}
				pos++;
			}
			else
			{
				for (size_t j = 0; j < 6; j++)
				{
					if ( table[pos] == table[pos + 1] )
					{
						arHeadersFooters[j]->push_back( NULL );
					}
					else
					{
						arHeadersFooters[j]->push_back( new CharacterRange( initialPos + table[pos], table[pos + 1] - table[pos]) );
					}
					pos++;

					if (pos >= tableSize) break;
				}
			}
			if (pos >= tableSize) break;
		}
		RELEASEARRAYOBJECTS(table);
	}

	HeaderAndFooterTable::~HeaderAndFooterTable()
	{
		for_each (m_arEvenFooters.begin(),  m_arEvenFooters.end(),  DeleteDynamicObject());
		for_each (m_arEvenHeaders.begin(),  m_arEvenHeaders.end(),  DeleteDynamicObject());
		for_each (m_arFirstFooters.begin(), m_arFirstFooters.end(), DeleteDynamicObject());
		for_each (m_arFirstHeaders.begin(), m_arFirstHeaders.end(), DeleteDynamicObject());
		for_each (m_arOddFooters.begin(),   m_arOddFooters.end(),   DeleteDynamicObject());
		for_each (m_arOddHeaders.begin(),   m_arOddHeaders.end(),   DeleteDynamicObject());

		for_each (m_arCommonHeadersFooters.begin(), m_arCommonHeadersFooters.end(), DeleteDynamicObject());		
	}
}