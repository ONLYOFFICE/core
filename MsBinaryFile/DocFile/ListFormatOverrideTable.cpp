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

#include "ListFormatOverrideTable.h"

namespace DocFileFormat
{  
	ListFormatOverrideTable::ListFormatOverrideTable( FileInformationBlock* fib, POLE::Stream* tableStream )
	{
		if ( fib->m_FibWord97.lcbPlfLfo > 0 )
		{
			VirtualStreamReader reader( tableStream, fib->m_FibWord97.fcPlfLfo, fib->m_nWordVersion);

			if (fib->m_FibWord97.fcPlfLfo > reader.GetSize()) return;

			//read the count of LFOs
			int count = reader.ReadInt32();

			//read the LFOs
			for ( int i = 0; i < count; i++ )
			{
				this->push_back( new ListFormatOverride( &reader, LFO_LENGTH ) );
			}

			//read the LFOLVLs
			for ( int i = 0; i < count; i++ )
			{
				this->cps.push_back( reader.ReadUInt32() );

				for ( int j = 0; j < this->at( i )->clfolvl; j++ )
				{
					this->at( i )->rgLfoLvl[j] = new ListFormatOverrideLevel( &reader, LFOLVL_LENGTH );
				}
			}
		}
	}

	ListFormatOverrideTable::~ListFormatOverrideTable()
	{
		for ( vector<ListFormatOverride*>::iterator iter = this->begin(); iter != this->end(); iter++ )
		{
			RELEASEOBJECT( *iter );
		}
	}
}
