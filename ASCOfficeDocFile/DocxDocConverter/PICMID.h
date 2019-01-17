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

#include "Brc80MayBeNil.h"

namespace ASCDocFileFormat
{
	class PICMID: public IOperand
	{
	public:
		PICMID()
		{
			memset(bytes, 0, SIZE_IN_BYTES);
		}

		explicit PICMID(short _dxaGoal, short _dyaGoal, unsigned short _mx, unsigned short _my, Brc80 _brcTop80, Brc80 _brcLeft80, Brc80 _brcBottom80, Brc80 _brcRight80 )
		{
			memset(bytes, 0, SIZE_IN_BYTES);

			unsigned int offset = 0;

			FormatUtils::SetBytes( ( this->bytes + offset ), _dxaGoal );
			offset += sizeof(_dxaGoal);

			FormatUtils::SetBytes( ( this->bytes + offset ), _dyaGoal );
			offset += sizeof(_dyaGoal);

			FormatUtils::SetBytes( ( this->bytes + offset ), _mx );
			offset += sizeof(_mx);

			FormatUtils::SetBytes( ( this->bytes + offset ), _my );
			offset += ( sizeof(_my) + 10 );

			unsigned int brc80Value = _brcTop80;

			FormatUtils::SetBytes( ( this->bytes + offset ), brc80Value );
			offset += sizeof(brc80Value);

			brc80Value = _brcLeft80;

			FormatUtils::SetBytes( ( this->bytes + offset ), brc80Value );
			offset += sizeof(brc80Value);

			brc80Value = _brcBottom80;

			FormatUtils::SetBytes( ( this->bytes + offset ), brc80Value );
			offset += sizeof(brc80Value);

			brc80Value = _brcRight80;

			FormatUtils::SetBytes( ( this->bytes + offset ), brc80Value );
			offset += sizeof(brc80Value);
		}

		PICMID (const PICMID& oICMID)
		{
			memset(bytes, 0, SIZE_IN_BYTES );
			memcpy(bytes, oICMID.bytes, SIZE_IN_BYTES );
		}

		virtual ~PICMID()
		{
		}

		virtual operator unsigned char*() const
		{
			return (unsigned char*)(this->bytes);
		}

		virtual operator const unsigned char*() const
		{
			return (const unsigned char*)this->bytes;
		}

		virtual unsigned int Size() const
		{
			return sizeof(this->bytes);
		}

	private:
		static const unsigned char SIZE_IN_BYTES = 38;
		unsigned char bytes[SIZE_IN_BYTES];
	};
}