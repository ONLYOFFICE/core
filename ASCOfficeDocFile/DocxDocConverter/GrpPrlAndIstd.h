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

namespace ASCDocFileFormat
{
	class GrpPrlAndIstd: public IStreamWriter
	{
	public:
		GrpPrlAndIstd() :  istd(0), grpprlSize(0)
		{
		}

		GrpPrlAndIstd( short _istd, const vector<Prl>& _grpprl ) : istd(_istd), grpprlSize(0)
		{
			for ( unsigned int i = 0; i < _grpprl.size(); i++ )
			{
				this->grpprlSize += (unsigned short)_grpprl[i].Size();
				this->grpprl.push_back( _grpprl[i] );
			}
		}

		GrpPrlAndIstd( const GrpPrlAndIstd& gpai ) : istd(0), grpprlSize(0)
		{
			this->istd = gpai.istd;
			this->grpprlSize = gpai.grpprlSize;

			for ( unsigned int i = 0; i < gpai.grpprl.size(); i++ )
			{
				this->grpprl.push_back( gpai.grpprl[i] );
			}
		}

		virtual ~GrpPrlAndIstd()
		{
		}

		inline bool operator == ( const GrpPrlAndIstd& gpai )
		{
			if ( ( this->istd != gpai.istd ) || ( this->grpprlSize != gpai.grpprlSize ) || ( this->grpprl.size() != gpai.grpprl.size() ) )
			{
				return false;
			}
			else
			{
				unsigned int minPrlCount = min( this->grpprl.size(), gpai.grpprl.size() );

				for ( unsigned int i = 0; i < minPrlCount; i++ )
				{
					if ( this->grpprl[i] != gpai.grpprl[i] )
					{
						return false;
					}
				}
			}

			return true;
		}

		inline bool operator != ( const GrpPrlAndIstd& gpai )
		{
			return !( *this == gpai );
		}

		inline GrpPrlAndIstd& operator = ( const GrpPrlAndIstd& gpai )
		{
			if ( *this != gpai )
			{
				this->istd = gpai.istd;
				this->grpprlSize = gpai.grpprlSize;

				this->grpprl.clear();

				for ( unsigned int i = 0; i < gpai.grpprl.size(); i++ )
				{
					this->grpprl.push_back( gpai.grpprl[i] );
				}
			}

			return *this;
		}

		inline unsigned long Size() const
		{
			return ( sizeof(this->istd) + this->grpprlSize );
		}

		inline Prl operator[] ( unsigned int index ) const
		{
			return this->grpprl[index];
		}

		inline unsigned int GetPrlCount() const
		{
			return (unsigned int)this->grpprl.size();
		}

		inline vector<Prl> GetPrls() const
		{
			return this->grpprl;
		}

		inline short GetIstd() const
		{
			return istd;
		}

		// IStreamWriter
		virtual unsigned char* GetBytes(unsigned long* size) const
		{
			unsigned char* bytes = NULL;

			if ( size != NULL )
			{
				*size = sizeof(this->istd) + this->grpprlSize;
				bytes = new unsigned char[*size];

				if ( bytes != NULL )
				{
					memset( bytes, 0, *size );
					FormatUtils::SetBytes( bytes, this->istd );

					unsigned char* prlBytes = NULL;
					unsigned long prlSize = 0;
					unsigned int prlPrevSize = 0; 

					for ( unsigned int i = 0; i < this->grpprl.size(); i++ )
					{
						prlBytes = this->grpprl[i].GetBytes( &prlSize );

						if ( prlBytes != NULL )
						{
							memcpy( ( bytes + sizeof(this->istd) + prlPrevSize ), prlBytes, prlSize );
							prlPrevSize += prlSize;

							RELEASEARRAYOBJECTS (prlBytes);
						}
					}
				}
			}

			return bytes;
		}

	private:

		short			istd; //The style applied to this paragraph, cell marker or table row marker.
		vector<Prl>		grpprl;
		unsigned short	grpprlSize;
	};
}