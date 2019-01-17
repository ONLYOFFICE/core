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

#include "Spa.h"

namespace ASCDocFileFormat
{
	class PlcfSpa : public IOperand
	{
	public:

		PlcfSpa(): bytes(NULL), sizeInBytes(0)
		{
			bytes = new unsigned char [sizeInBytes];

			if (bytes)
				memset(bytes, 0, sizeInBytes);
		}

		PlcfSpa (const vector<unsigned int>& aCP, const vector<Spa>& oSpa) : bytes(NULL), sizeInBytes(0)
		{
			sizeInBytes	=	( (aCP.size() * sizeof(unsigned int) ) + ( oSpa.size() * Spa::SIZE_IN_BYTES ) );

			bytes		=	new unsigned char[sizeInBytes];
			if (bytes)
			{
				memset (bytes, 0,sizeInBytes);

				unsigned int offset = 0;
				for ( vector<unsigned int>::const_iterator iter = aCP.begin(); iter != aCP.end(); ++iter)
				{
					FormatUtils::SetBytes( ( bytes + offset ), (int)(*iter) );
					offset += sizeof(unsigned int);
				}

				if (bytes)
				{
					for (vector<Spa>::const_iterator iter = oSpa.begin(); iter != oSpa.end(); ++iter)
					{
						memcpy ((bytes + offset), (unsigned char*)(*iter), iter->Size());
						offset += iter->Size();
					}
				}
			}
		}

		PlcfSpa(const PlcfSpa& oSpa) : bytes(NULL), sizeInBytes(oSpa.sizeInBytes)
		{
			bytes	=	new unsigned char[sizeInBytes];

			if (bytes)
			{
				memset (bytes, 0, sizeInBytes);
				memcpy (bytes, oSpa.bytes, sizeInBytes);
			}
		}

		inline bool operator == (const PlcfSpa& oSpa)
		{
			return ((sizeInBytes == oSpa.sizeInBytes) && (memcmp(bytes, oSpa.bytes, sizeInBytes ) == 0));
		}

		inline bool operator != (const PlcfSpa& oSpa)
		{
			return !( this->operator == (oSpa) );
		}

		inline PlcfSpa& operator = (const PlcfSpa& oSpa)
		{
			if (*this != oSpa)
			{
				RELEASEARRAYOBJECTS(bytes);

				sizeInBytes	=	oSpa.sizeInBytes;
				bytes		=	new unsigned char[sizeInBytes];

				if (bytes)
				{
					memcpy(bytes, oSpa.bytes, sizeInBytes);
				}  
			}

			return *this;
		}


		// IOperand

		virtual ~PlcfSpa()
		{
			RELEASEARRAYOBJECTS(bytes);
		}

		virtual operator unsigned char*() const
		{
			return this->bytes;
		}

		virtual operator const unsigned char*() const
		{
			return (const unsigned char*)this->bytes;
		}

		virtual unsigned int Size() const
		{
			return this->sizeInBytes;
		}

	private:

		unsigned char* bytes;
		unsigned int sizeInBytes;
	};
}