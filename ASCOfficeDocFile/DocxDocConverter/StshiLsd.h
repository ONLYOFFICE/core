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

#include "LSD.h"

namespace ASCDocFileFormat
{
	class StshiLsd: public IOperand
	{
	public:
		StshiLsd () : bytes(NULL), sizeInBytes(0)
		{

		}

		StshiLsd (const vector<LSD>& _mpstiilsd) : bytes(NULL), sizeInBytes(0), mpstiilsd(_mpstiilsd)
		{
			sizeInBytes	=	( sizeof(unsigned short) + ( sizeof(LSD) * mpstiilsd.size() ) );
			bytes		=	new unsigned char[sizeInBytes];

			if ( NULL != bytes )
			{
				memset ( bytes, 0, sizeInBytes );
				FormatUtils::SetBytes ( bytes, (unsigned short)sizeof(LSD) );

				unsigned int lsdOffset = sizeof(unsigned short);

				for (vector<LSD>::iterator iter = mpstiilsd.begin(); iter != mpstiilsd.end(); ++iter)
				{
					FormatUtils::SetBytes( ( bytes + lsdOffset ), (unsigned int)(*iter) );
					lsdOffset += sizeof(LSD);
				}
			}
		}

		StshiLsd (const StshiLsd& _stshiLsd) : bytes(NULL), sizeInBytes(_stshiLsd.sizeInBytes), mpstiilsd(_stshiLsd.mpstiilsd)
		{
			bytes = new unsigned char[sizeInBytes];

			if ( NULL != bytes )
			{
				memset ( bytes, 0, sizeInBytes );
				memcpy ( bytes, _stshiLsd.bytes, sizeInBytes );
			}
		}

		virtual ~StshiLsd()
		{
			RELEASEARRAYOBJECTS(bytes);
		}

		virtual operator unsigned char*() const
		{
			return bytes;
		}

		virtual operator const unsigned char*() const
		{
			return (const unsigned char*)bytes;
		}

		virtual unsigned int Size() const
		{
			return sizeInBytes;
		}
	private:

		std::vector<LSD>	mpstiilsd;

		unsigned char*				bytes;
		unsigned int		sizeInBytes;
	};
}