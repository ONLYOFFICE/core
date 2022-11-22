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