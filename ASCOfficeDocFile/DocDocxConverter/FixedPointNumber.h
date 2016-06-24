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

namespace DocFileFormat
{
	/// Specifies an approximation of a real number, where the approximation has a fixed number of digits after the radix point. 
	/// 
	/// This type is specified in [MS-OSHARED] section 2.2.1.6.
	/// 
	/// Value of the real number = Integral + ( Fractional / 65536.0 ) 
	/// 
	/// Integral (2 bytes): A signed integer that specifies the integral part of the real number. 
	/// Fractional (2 bytes): An unsigned integer that specifies the fractional part of the real number.
	class FixedPointNumber
	{ 
	public:
		unsigned short Integral;
		unsigned short Fractional;

		FixedPointNumber( unsigned short integral = 0, unsigned short fractional = 0 )
		{
			this->Integral = integral;
			this->Fractional = fractional;
		}

		FixedPointNumber( unsigned int value )
		{
			unsigned char* bytes = FormatUtils::GetBytes( value );

			this->Integral = FormatUtils::BytesToUInt16( bytes, 0, sizeof(value) );
			this->Fractional = FormatUtils::BytesToUInt16( bytes, 2, sizeof(value) );

			RELEASEARRAYOBJECTS( bytes );
		}

		FixedPointNumber( const unsigned char* bytes, unsigned int size )
		{
			if ( ( bytes != NULL ) && ( size >= 4 ) )
			{
				this->Integral = FormatUtils::BytesToUInt16( bytes, 0, size );
				this->Fractional = FormatUtils::BytesToUInt16( bytes, 2, size );
			}
		}

		double ToAngle() const
		{
			if ( this->Fractional != 0 )
			{
				// negative angle
				return ( this->Fractional - 65536.0 );
			}
			else if ( this->Integral != 0 )
			{
				//positive angle
				return ( 65536.0 - this->Integral );
			}
			else
			{
				return 0.0;
			}
		}

		double GetValue() const
		{
			return (double)( this->Integral + ( (double)this->Fractional / 65536.0 ) );
		}
	};
}