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

#include "BorderCode.h"

namespace DocFileFormat
{	
	BorderCode::BorderCode(): cv(0), dptLineWidth(0), brcType(0), ico( Global::ColorNameIdentifier[0] ), dptSpace(0), fShadow(false), fFrame(false), fNil(false)
	{
	}

	/// Parses the unsigned char for a BRC
	BorderCode::BorderCode( unsigned char* bytes, int size ):
	cv(0), dptLineWidth(0), brcType(0), ico( Global::ColorNameIdentifier[0] ), dptSpace(0), fShadow(false), fFrame(false), fNil(false)
	{
		if ( FormatUtils::ArraySum( bytes, size ) ==  ( size * 255 ) )
		{
			fNil = true;
		}
		else if ( size == 8 )
		{
			//it's a border code of Word 2000/2003
			cv = FormatUtils::BytesToInt32( bytes, 0, size );
			ico = std::wstring( Global::ColorIdentifier[0] );

			dptLineWidth = bytes[4];
			brcType = bytes[5];

			short val = FormatUtils::BytesToInt16( bytes, 6, size );
			dptSpace = val & 0x001F;

			//not sure if this is correct, the values from the spec are definitly wrong:
			fShadow = FormatUtils::BitmaskToBool( val, 0x20 );
			fFrame = FormatUtils::BitmaskToBool( val, 0x40 );
		}
		else if ( size == 4 )
		{
			unsigned short val = FormatUtils::BytesToUInt16( bytes, 0, size );

			dptLineWidth = (unsigned char)( val & 0x00FF );
			brcType = (unsigned char)( ( val & 0xFF00 ) >> 8 );

			val = FormatUtils::BytesToUInt16( bytes, 2, size );

			ico = FormatUtils::MapValueToWideString( ( val & 0x00FF ), &Global::ColorNameIdentifier[0][0], 17, 12 );
			dptSpace = ( val & 0x1F00 ) >> 8;
		}
		else if (size == 2)
		{
			unsigned short val = FormatUtils::BytesToUInt16( bytes, 0, size );

			dptLineWidth	= GETBITS(val, 0, 2);
			brcType			= GETBITS(val, 3, 4);
			fShadow			= GETBIT(val, 5);
			ico				= FormatUtils::MapValueToWideString(GETBITS(val, 6, 10), &Global::ColorNameIdentifier[0][0], 17, 12 );
			dptSpace		= GETBITS(val, 11, 15);

		}
	}
	BorderCode::BorderCode( const BorderCode& bc )
	{
		if ( this != &bc )
		{
			cv				= bc.cv;
			dptLineWidth	= bc.dptLineWidth;
			brcType			= bc.brcType;
			ico				= bc.ico;
			dptSpace		= bc.dptSpace;
			fShadow			= bc.fShadow;
			fFrame			= bc.fFrame;
			fNil			= bc.fNil;
		}
	}

	bool BorderCode::operator == ( const BorderCode& bc )
	{
		if ( ( cv == bc.cv ) && ( dptLineWidth == bc.dptLineWidth ) && ( brcType == bc.brcType ) &&
			( ico == bc.ico ) && ( dptSpace == bc.dptSpace ) && ( fShadow == bc.fShadow ) &&
			( fFrame == bc.fFrame ) && ( fNil == bc.fNil ) )
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	bool BorderCode::operator != ( const BorderCode& bc )
	{
		return !( *this == bc );
	}
}
