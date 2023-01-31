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

#include "FormattedDiskPageCHPX.h"

namespace DocFileFormat
{
	FormattedDiskPageCHPX::~FormattedDiskPageCHPX()
	{
		RELEASEARRAYOBJECTS( rgfc );
		RELEASEARRAYOBJECTS( rgb );

		if ( grpchpx != NULL )
		{
			for ( unsigned int i = 0; i < grpchpxSize; i++ )
			{
				RELEASEOBJECT( grpchpx[i] );
			}

			RELEASEARRAYOBJECTS( grpchpx );
		}
	}

	/*========================================================================================================*/

	FormattedDiskPageCHPX::FormattedDiskPageCHPX( POLE::Stream* wordStream, int offset, int nWordVersion ):
																FormattedDiskPage(), rgb(NULL), grpchpxSize(0), grpchpx(NULL) 
	{
		Type = Character;
		WordStream = wordStream;

		//read the 512 bytes (FKP)
		unsigned char* bytes = NULL;
		bytes = new unsigned char[512];

		WordStream->seek( offset);
		WordStream->read( bytes, 512 );

		//get the count first
		crun = bytes[511];

		//create and fill the array with the adresses
		rgfcSize = crun + 1;
		rgfc = new int[rgfcSize];

		int j = 0;

		for ( unsigned int i = 0; i < rgfcSize; i++ )
		{
			rgfc[i] = FormatUtils::BytesToInt32( bytes, j, 512 );
			j += 4;
		}

		grpchpxSize		= crun;
		rgb				= new unsigned char[crun];
		grpchpx			= new CharacterPropertyExceptions*[grpchpxSize];

		j = 4 * ( crun + 1 );

		unsigned char* chpx = NULL;

		for ( int i = 0; i < crun; i++ )
		{
			//fill the rgb array
			unsigned char wordOffset = bytes[j];
			rgb[i] = wordOffset;
			j++;

			if ( wordOffset != 0 )
			{
				//read first unsigned char of CHPX
				//it's the count of bytes
				unsigned char cb = bytes[wordOffset * 2];

				//read the bytes of chpx
				chpx = new unsigned char[cb];
				memcpy( chpx, ( bytes + (wordOffset * 2) + 1 ), cb );

				//parse CHPX and fill grpchpx
				grpchpx[i] = new CharacterPropertyExceptions( chpx, cb, nWordVersion);

				RELEASEARRAYOBJECTS( chpx );
			}
			else
			{
				//create a CHPX which doesn't modify anything
				grpchpx[i] = new CharacterPropertyExceptions();
			}
		}

		RELEASEARRAYOBJECTS( bytes );
	}

	/*========================================================================================================*/

	/// Parses the 0Table (or 1Table) for FKP _entries containing CHPX
	std::list<FormattedDiskPageCHPX*>* FormattedDiskPageCHPX::GetAllCHPXFKPs( FileInformationBlock* fib, POLE::Stream* wordStream, POLE::Stream* tableStream )
	{
		std::list<FormattedDiskPageCHPX*>* CHPXlist = new std::list<FormattedDiskPageCHPX*>();

		//get bintable for CHPX
		unsigned char* binTableChpx = new unsigned char[fib->m_FibWord97.lcbPlcfBteChpx];

		if (tableStream)
		{
			tableStream->seek( fib->m_FibWord97.fcPlcfBteChpx);
			tableStream->read( binTableChpx, fib->m_FibWord97.lcbPlcfBteChpx);
		}
		//there are n offsets and n-1 fkp's in the bin table

		if (fib->m_nWordVersion > 0)
		{
			int				n		= ( ( (int)fib->m_FibWord97.lcbPlcfBteChpx - 8 ) / 6 ) + 1;
	
			unsigned int	first	= FormatUtils::BytesToInt32(binTableChpx, 0, fib->m_FibWord97.lcbPlcfBteChpx );
			unsigned int	last	= FormatUtils::BytesToInt32(binTableChpx, 4, fib->m_FibWord97.lcbPlcfBteChpx );

			int start_chpx = 8;
			if (fib->m_FibWord97.lcbPlcfBteChpx - 8 >  (n - 1) * 4)
			{
				start_chpx += ((n-1) * 4); //дублирование crun 
			}

			//Get the indexed CHPX FKPs
			for ( unsigned int i = start_chpx; i < fib->m_FibWord97.lcbPlcfBteChpx; i += 2 )
			{
				//indexed FKP is the 6th 512byte page
				int fkpnr = FormatUtils::BytesToInt16( binTableChpx, i, fib->m_FibWord97.lcbPlcfBteChpx );

				//so starts at:
				int offset = fkpnr * 512;

				//parse the FKP and add it to the list
				CHPXlist->push_back( new FormattedDiskPageCHPX( wordStream, offset, fib->m_nWordVersion ) );
			}
		}
		else
		{
			int n = ( ( (int)fib->m_FibWord97.lcbPlcfBteChpx - 4 ) / 8 ) + 1;
			//Get the indexed CHPX FKPs
			for ( unsigned int i = (n * 4); i < fib->m_FibWord97.lcbPlcfBteChpx; i += 4 )
			{
				//indexed FKP is the 6th 512byte page
				int fkpnr = FormatUtils::BytesToInt32( binTableChpx, i, fib->m_FibWord97.lcbPlcfBteChpx );

				//so starts at:
				int offset = fkpnr * 512;

				//parse the FKP and add it to the list
				CHPXlist->push_back( new FormattedDiskPageCHPX( wordStream, offset, fib->m_nWordVersion ) );
			}
		}

		RELEASEARRAYOBJECTS( binTableChpx );

		return CHPXlist;
	}
}