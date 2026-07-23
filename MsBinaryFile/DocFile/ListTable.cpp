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

#include "ListTable.h"

#include <algorithm>

namespace DocFileFormat
{
	ListTable::~ListTable()
	{
	}

	ListTable::ListTable( FileInformationBlock* fib, POLE::Stream* tableStream )
	{
		if ( fib->m_FibWord97.lcbPlfLst > 0 )
		{
			VirtualStreamReader reader( tableStream, fib->m_FibWord97.fcPlfLst, fib->m_nWordVersion);
			
			if (fib->m_FibWord97.fcPlfLst > reader.GetSize()) return;

			//the ListTable is not a real plex:
			//it starts with a count, followed by the array of LSTF structs,
			//followed by the array of LVLF structs

			//read count
			short count = reader.ReadInt16();

			//read the LSTF structs
			for ( int i = 0; i < count; i++ )
			{
				listData.push_back( ListData::create( &reader, ListData::VARIABLE_LENGTH ) );
			}

			//read the LVLF structs
			for ( size_t i = 0; i < listData.size(); ++i)
			{
				for ( size_t j = 0; j < listData[i]->rglvl->size(); j++ )
				{
					listData[i]->rglvl->operator []( j ) = new ListLevel( &reader, ListData::VARIABLE_LENGTH );
				}
			}
		}
	}

	size_t ListTable::appendNumbering( NumberingDescriptorPtr &desc, const std::wstring & rPr )
	{
		for (size_t i = 0; i < listNumbering.size(); ++i)
		{
			if (listNumbering[i]->operator == (desc))
			{
				return listNumbering[i]->id;
			}
		}
		desc->rPr = rPr;
		desc->id = listData.size() + listNumbering.size() + 1; // 23.doc
		listNumbering.push_back(desc);
		return desc->id;
	}

}