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

#include "Prl.h"

#include <vector>

namespace ASCDocFileFormat
{
	class Chpx : public IStreamWriter
	{

	public:
		Chpx() : cb(0)
		{
		}

		Chpx(const std::vector<Prl>& _grpprl) : cb(0)
		{
			for (unsigned int i = 0; i < _grpprl.size(); ++i)
			{
				unsigned char grpprlSize = (unsigned char)_grpprl[i].Size();
				if( this->cb + grpprlSize >= 256 )
					break;
				this->cb += grpprlSize;
				this->grpprl.push_back( _grpprl[i] );
			}
		}

		Chpx(const Chpx& chpx) : cb(0)
		{
			this->cb = chpx.cb;

			for ( unsigned int i = 0; i < chpx.grpprl.size(); i++ )
			{
				this->grpprl.push_back( chpx.grpprl[i] );
			}
		}

		virtual ~Chpx()
		{
		}

		bool operator == (const Chpx& chpx)
		{
			if ( ( this->cb != chpx.cb ) || ( this->grpprl.size() != chpx.grpprl.size() ) )
			{
				return false;
			}
			else
			{
				unsigned int minPrlCount = min( this->grpprl.size(), chpx.grpprl.size() );

				for ( unsigned int i = 0; i < minPrlCount; i++ )
				{
					if ( this->grpprl[i] != chpx.grpprl[i] )
					{
						return false;
					}
				}
			}

			return true;
		}

		bool operator != (const Chpx& chpx)
		{
			return !( *this == chpx );
		}

		Chpx& operator = (const Chpx& chpx)
		{
			if ( *this != chpx )
			{
				this->cb = chpx.cb;

				this->grpprl.clear();

				for ( unsigned int i = 0; i < chpx.grpprl.size(); i++ )
				{
					this->grpprl.push_back( chpx.grpprl[i] );
				}
			}

			return *this;
		}

		virtual unsigned char* GetBytes(unsigned long* size) const
		{
			unsigned char* bytes = NULL;

			if ( size != NULL )
			{
				*size = sizeof(this->cb) + this->cb;
				bytes = new unsigned char[*size];

				if ( bytes != NULL )
				{
					memset( bytes, 0, *size );
					bytes[0] = this->cb;

					unsigned char* prlBytes = NULL;
					unsigned long prlSize = 0;
					unsigned int prlPrevSize = 0; 

					for ( unsigned int i = 0; i < this->grpprl.size(); i++ )
					{
						prlBytes = this->grpprl[i].GetBytes( &prlSize );

						if ( prlBytes != NULL )
						{
							memcpy( ( bytes + sizeof(this->cb) + prlPrevSize ), prlBytes, prlSize );
							prlPrevSize += prlSize;

							RELEASEARRAYOBJECTS (prlBytes);
						}
					}
				}
			}

			return bytes;
		}

		unsigned long Size() const
		{
			return ( sizeof(this->cb) + this->cb );
		}

		inline size_t GetPrlSize ()
		{
			return grpprl.size();
		}

	private:

		unsigned char cb; //Size in bytes of the grpprl
		std::vector<Prl> grpprl;
	};
}