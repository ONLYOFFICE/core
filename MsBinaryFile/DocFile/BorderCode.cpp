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

#include "BorderCode.h"

namespace DocFileFormat
{	
	BorderCode::BorderCode(): dptLineWidth(0), brcType(0), ico( Global::ColorIdentifier[0] ), dptSpace(0), fShadow(false), fFrame(false), fNil(false)
	{
	}

	/// Parses the unsigned char for a BRC
	BorderCode::BorderCode( unsigned char* bytes, int size ):
	dptLineWidth(0), brcType(0), ico( Global::ColorIdentifier[0] ), dptSpace(0), fShadow(false), fFrame(false), fNil(false)
	{
		if ( FormatUtils::ArraySum( bytes, size ) ==  ( size * 255 ) )
		{
			fNil = true;
		}
		else if ( size == 8 )
		{ //it's a border code of Word 2000/2003			
			cv = FormatUtils::BytesToInt32( bytes, 0, size );
			BYTE auto_ = GETBITS(*cv, 24, 31);

			if (auto_ == 0)
			{
				BYTE B = GETBITS(*cv, 0, 7);
				BYTE G = GETBITS(*cv, 8, 15);
				BYTE R = GETBITS(*cv, 16, 23);
				cv = (B << 16) + (G << 8) + (R);
			}
			else cv = boost::none;

			ico = std::wstring( Global::ColorIdentifier[0] );

			dptLineWidth = bytes[4];
			brcType = bytes[5];

			short val = FormatUtils::BytesToInt16( bytes, 6, size );
            if (val == -1)
                dptSpace = 0;
            else
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

			ico = FormatUtils::MapValueToWideString( ( val & 0x00FF ), &Global::ColorIdentifier[0][0], 17, 12 );
			dptSpace = ( val & 0x1F00 ) >> 8;
		}
		else if (size == 2)
		{
			unsigned short val = FormatUtils::BytesToUInt16( bytes, 0, size );

			dptLineWidth	= GETBITS(val, 0, 2);
			brcType			= GETBITS(val, 3, 4);
			fShadow			= GETBIT(val, 5);
			ico				= FormatUtils::MapValueToWideString(GETBITS(val, 6, 10), &Global::ColorIdentifier[0][0], 17, 12 );
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
	std::wstring BorderCode::getColor()
	{
		if (cv)
		{
			return FormatUtils::IntToFormattedWideString(*cv, L"#%06x");
		}
		else if (false == ico.empty())
		{
			return ico;
		}
		else return L"auto";
	}
	bool BorderCode::operator == ( const BorderCode& bc )
	{
		if ( ( cv == bc.cv ) && ( dptLineWidth == bc.dptLineWidth ) && ( brcType == bc.brcType ) &&
			(ico == bc.ico ) && ( dptSpace == bc.dptSpace ) && ( fShadow == bc.fShadow ) &&
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
