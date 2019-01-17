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

#include "GrpPrlAndIstd.h"

namespace ASCDocFileFormat
{
	class PapxInFkp : public IStreamWriter
	{
	public:
		PapxInFkp() : grpprlInPapx()
		{

		}

		PapxInFkp(const GrpPrlAndIstd& _grpprlInPapx) : grpprlInPapx(_grpprlInPapx)
		{

		}

		PapxInFkp(const PapxInFkp& pifkp) : grpprlInPapx(pifkp.grpprlInPapx)
		{

		}

		virtual ~PapxInFkp()
		{
		}

		//
		inline bool operator == (const PapxInFkp& pifkp)
		{
			return ( grpprlInPapx == pifkp.grpprlInPapx );
		}

		inline bool operator != (const PapxInFkp& pifkp)
		{
			return !( *this == pifkp );
		}

		inline PapxInFkp& operator = (const PapxInFkp& pifkp)
		{
			if ( *this != pifkp )
			{
				grpprlInPapx = pifkp.grpprlInPapx;
			}

			return *this;
		}

		inline unsigned long Size() const
		{
			unsigned long sizeInBytes = 0;
			unsigned char cb		= 0;
			unsigned char cb_	= 0;

			sizeInBytes = this->grpprlInPapx.Size();

			if ( sizeInBytes % 2 != 0 )
			{
				cb = (unsigned char)( ( sizeInBytes + 1 ) / 2 );
				sizeInBytes += sizeof(cb); 
			}
			else
			{
				cb = 0;
				cb_ = (unsigned char)( sizeInBytes / 2 );
				sizeInBytes += ( sizeof(cb) + sizeof(cb_) );
			}

			return sizeInBytes;
		}

		inline Prl operator[] (unsigned int index) const
		{
			return grpprlInPapx[index];
		}

		inline unsigned int GetPrlCount() const
		{
			return this->grpprlInPapx.GetPrlCount();
		}

		inline vector<Prl> GetPrls() const
		{
			return this->grpprlInPapx.GetPrls();
		}

		inline short GetIstd() const
		{
			return grpprlInPapx.GetIstd();
		}

		// IStreamWriter
		virtual unsigned char* GetBytes(unsigned long* size) const
		{
			unsigned char* bytes = NULL;

			if ( size != NULL )
			{
				unsigned char cb = 0;
				unsigned char cb_ = 0;

				*size = this->grpprlInPapx.Size();

				if ( *size % 2 != 0 )
				{
					cb = (unsigned char)( ( *size + 1 ) / 2 );
					*size += sizeof(cb); 
				}
				else
				{
					cb = 0;
					cb_ = (unsigned char)( *size / 2 );
					*size += ( sizeof(cb) + sizeof(cb_) );
				}

				bytes = new unsigned char[*size];

				if ( bytes != NULL )
				{
					memset( bytes, 0, *size );
					bytes[0] = cb;

					unsigned char cb_offset = 0;

					if ( cb_ != 0 )
					{
						bytes[1] = cb_;
						cb_offset = sizeof(cb_);
					}

					unsigned char* grpPrlAndIstdBytes = NULL;
					unsigned long grpPrlAndIstdSize = 0;

					grpPrlAndIstdBytes = this->grpprlInPapx.GetBytes( &grpPrlAndIstdSize );

					if ( grpPrlAndIstdBytes != NULL )
					{
						memcpy( ( bytes + sizeof(cb) + cb_offset ), grpPrlAndIstdBytes, grpPrlAndIstdSize );

						RELEASEARRAYOBJECTS (grpPrlAndIstdBytes);
					}
				}
			}

			return bytes;
		}

	private:

		GrpPrlAndIstd	grpprlInPapx;
	};
}