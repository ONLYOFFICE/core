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
	class Stshif : public IOperand
	{
	public:

		Stshif ()
		{
			memset ( bytes, 0, SIZE_IN_BYTES );
		}

		Stshif (unsigned short cstd, bool StdfPost2000Exists, unsigned short stiMaxWhenSaved, short ftcAsci = 0, short ftcFE = 0, short ftcOther = 0 )
		{
			memset ( bytes, 0, SIZE_IN_BYTES);

			if ( cstd < 0x000F )
			{
				FormatUtils::SetBytes ( bytes, (unsigned short)0x000F );
			}
			else if ( cstd >= 0x0FFE )
			{
				FormatUtils::SetBytes ( bytes, (unsigned short)0x0FFE );
			}
			else
			{
				FormatUtils::SetBytes ( bytes, cstd );
			}

			if ( StdfPost2000Exists )
			{
				FormatUtils::SetBytes( ( bytes + sizeof(cstd) ), (unsigned short)0x0012 );
			}
			else
			{
				FormatUtils::SetBytes( ( bytes + sizeof(cstd) ), (unsigned short)0x000A );
			}

			FormatUtils::SetBytes( ( bytes + 4 ), (unsigned short)0x0001 );
			FormatUtils::SetBytes( ( bytes + 6 ), stiMaxWhenSaved );
			FormatUtils::SetBytes( ( bytes + 8 ), (unsigned short)0x000F );
			FormatUtils::SetBytes( ( bytes + 10 ), (unsigned short)0x0000 );
			FormatUtils::SetBytes( ( bytes + 12 ), ftcAsci );
			FormatUtils::SetBytes( ( bytes + 14 ), ftcFE );
			FormatUtils::SetBytes( ( bytes + 16 ), ftcOther );
		}

		Stshif (const Stshif& stshif)
		{
			memset ( bytes, 0, SIZE_IN_BYTES );
			memcpy ( bytes, stshif.bytes, SIZE_IN_BYTES );
		}

		virtual ~Stshif()
		{
		}

		virtual operator unsigned char*() const
		{
			return (unsigned char*)(bytes);
		}

		virtual operator const unsigned char*() const
		{
			return (const unsigned char*)bytes;
		}

		virtual unsigned int Size() const
		{
			return SIZE_IN_BYTES;
		}

	private:

		static const unsigned char	SIZE_IN_BYTES = 18;
		unsigned char				bytes[SIZE_IN_BYTES];
	};
}