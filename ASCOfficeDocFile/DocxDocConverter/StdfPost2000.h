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
#pragma once

namespace ASCDocFileFormat
{
	union StdfPost2000
	{
	private:
		static const unsigned char SIZE_IN_BYTES = 8;

		struct
		{
			unsigned short istdLink				:	12;
			unsigned short fHasOriginalStyle	:	1;
			unsigned short fSpare				:	3;
			unsigned char rsid[4];
			unsigned short iftcHtml				:	3;
			unsigned short unused				:	1;
			unsigned short iPriority			:	12;
		} StdfPost2000Struct;

		unsigned char StdfPost2000Bytes [ SIZE_IN_BYTES ];

	public:

		StdfPost2000()
		{
			memset( this->StdfPost2000Bytes, 0, SIZE_IN_BYTES );
		}

		StdfPost2000( unsigned short _istdLink, bool _fHasOriginalStyle, unsigned int _rsid, unsigned short _iPriority )
		{
			memset ( StdfPost2000Bytes, 0, SIZE_IN_BYTES );

			if ( _istdLink > 0x0FFF )
			{
				StdfPost2000Struct.istdLink		=	0x0FFF;
			}
			else
			{
				StdfPost2000Struct.istdLink		=	_istdLink;
			}

			( _fHasOriginalStyle ) ? ( StdfPost2000Struct.fHasOriginalStyle = 1 ) : ( StdfPost2000Struct.fHasOriginalStyle = 0 );
			
			StdfPost2000Struct.fSpare			=	0;
			
			FormatUtils::SetBytes (StdfPost2000Struct.rsid, _rsid);
			
			StdfPost2000Struct.iftcHtml			=	0;
			StdfPost2000Struct.unused			=	0;

			if ( _iPriority > 0x0063 )
			{
				StdfPost2000Struct.iPriority	=	0x0063;
			}
			else
			{
				StdfPost2000Struct.iPriority	=	_iPriority;
			}
		}

		StdfPost2000( const StdfPost2000& _stdfPost2000 )
		{
			memset( StdfPost2000Bytes, 0, SIZE_IN_BYTES );
			memcpy( StdfPost2000Bytes, _stdfPost2000.StdfPost2000Bytes, SIZE_IN_BYTES );
		}

		bool operator == ( const StdfPost2000& _stdfPost2000 )
		{
			return ( memcmp ( StdfPost2000Bytes, _stdfPost2000.StdfPost2000Bytes, SIZE_IN_BYTES ) == 0 );
		}

		bool operator != ( const StdfPost2000& _stdfPost2000 )
		{
			return !( this->operator == ( _stdfPost2000 ) );
		}

		StdfPost2000& operator = ( const StdfPost2000& _stdfPost2000 )
		{
			if ( *this != _stdfPost2000 )
			{
				memset( StdfPost2000Bytes, 0, SIZE_IN_BYTES );
				memcpy( StdfPost2000Bytes, _stdfPost2000.StdfPost2000Bytes, SIZE_IN_BYTES );
			}

			return *this;
		}

		operator unsigned char*() const
		{
			return (unsigned char*)(StdfPost2000Bytes);
		}
	};
}