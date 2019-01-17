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

#include "PapxInFkp.h"
#include "BxPap.h"
#include "Clx.h"
#include "BinaryStorage.h"

namespace ASCDocFileFormat
{
	class PapxFkp: public IStreamWriter
	{
	public:
		static const unsigned short PAPX_FKP_SIZE = 512;
		static const unsigned char PAPX_FKP_MAX_CPARA = 0x1D;

		PapxFkp (const vector<unsigned int>& _rgfc, const vector<PapxInFkp>& _papxInFkps) : cpara(0)
		{
			this->cpara = _papxInFkps.size();
			this->rgfc = _rgfc;

			unsigned long papxSizeInBytes = 0;

			this->papxInFkps.push_back( _papxInFkps[0] );
			papxSizeInBytes = this->papxInFkps[0].Size();
			this->rgbx.push_back( BxPap( (unsigned char)( ( PAPX_FKP_SIZE - 1 - papxSizeInBytes ) / 2 ) ) );

			for ( int i = 1; i < this->cpara; i++ )
			{
				this->papxInFkps.push_back( _papxInFkps[i] );
				papxSizeInBytes = this->papxInFkps[i].Size();
				this->rgbx.push_back( BxPap( (unsigned char)( this->rgbx[i-1].GetOffset() - 1 - ( papxSizeInBytes / 2 ) ) ) );
			}
		}

		virtual ~PapxFkp()
		{
		}

		virtual unsigned char* GetBytes( unsigned long* size ) const
		{
			unsigned char* bytes = NULL;

			if ( size != NULL )
			{
				*size = 512;
				bytes = new unsigned char[*size];

				if ( bytes != NULL )
				{
					memset( bytes, 0, *size );

					int i = 0;

					for ( ; i < ( this->cpara + 1); i++ )
					{
						FormatUtils::SetBytes( ( bytes + ( i * sizeof(this->rgfc[i]) ) ), (int)this->rgfc[i] );
					}

					i = ( this->cpara + 1) * sizeof(this->rgfc[i]);

					unsigned char *papxsbytes = NULL;
					unsigned long papxssize = 0;

					for ( int j = 0; j < this->cpara; j++, i += ( BxPap::RESERVED_SIZE + 1 ) )
					{
						bytes[i] = this->rgbx[j].GetOffset();
						unsigned char *papxsbytes = this->papxInFkps[j].GetBytes( &papxssize );

						if ( papxsbytes != NULL )
						{
							memcpy( ( bytes + ( bytes[i] * 2 ) ), papxsbytes, papxssize );
							RELEASEARRAYOBJECTS (papxsbytes);
						}
					}

					bytes[511] = this->cpara;
				}
			}

			return bytes;
		}

		unsigned int GetEndOffset() const
		{
			return ( *(this->rgfc.end() - 1 ) );
		}

		static vector<PapxFkp> GetAllPapxFkps( const vector<unsigned int>& _rgfc, const vector<PapxInFkp>& _papxInFkps )
		{
			vector<PapxFkp> allPapxFkps;
			vector<unsigned int> rgfc;
			vector<PapxInFkp> papxInFkps;

			rgfc.push_back( _rgfc[0] );
			unsigned int allPapxInFkpsSize = 0;
			unsigned int rgfcCount = 2;

			for ( unsigned int i = 0; i < _papxInFkps.size(); i++ )
			{
				unsigned int papxInFkpSize = _papxInFkps[i].Size();
				allPapxInFkpsSize += papxInFkpSize;

				//Check if all rgfcs and rgbxs + PapxInFkps less then 512 bytes
				if ( ( ( rgfcCount * sizeof(unsigned int) ) + ( ( rgfcCount - 1 ) * ( BxPap::RESERVED_SIZE + 1 ) ) ) >= ( PAPX_FKP_SIZE / 2 ) ||
					( allPapxInFkpsSize ) >= ( PAPX_FKP_SIZE / 2 ) )
				{
					PapxFkp papxFkp( rgfc, papxInFkps );
					allPapxFkps.push_back( papxFkp );
					rgfc.clear();
					papxInFkps.clear();
					rgfc.push_back( _rgfc[i] );
					allPapxInFkpsSize = papxInFkpSize;
					rgfcCount = 2;
				}

				rgfc.push_back( _rgfc[i+1] );

				if ( ( ( rgfcCount * sizeof(unsigned int) ) + ( ( rgfcCount - 1 ) * ( BxPap::RESERVED_SIZE + 1 ) ) ) >= ( PAPX_FKP_SIZE / 2 ) ||
					( papxInFkpSize ) >= ( PAPX_FKP_SIZE / 2 ) )
				{
					PrcData prcData( _papxInFkps[i].GetPrls() );

					BinaryStorageSingleton* binaryStorage = BinaryStorageSingleton::Instance();

					if ( binaryStorage != NULL )
					{
						unsigned int prcDataOffset = binaryStorage->PushData( (const unsigned char*)prcData, prcData.Size() );

						vector<Prl> prls;
						prls.push_back( Prl( (short)0x6646, (unsigned char*)(&prcDataOffset) ) );
						PapxInFkp papxInFkp( GrpPrlAndIstd( 0, prls ) );

						papxInFkps.push_back( papxInFkp );
					}
				}
				else
				{
					papxInFkps.push_back( _papxInFkps[i] );
				}

				rgfcCount++;
			}

			if ( !rgfc.empty() && !papxInFkps.empty() )
			{
				PapxFkp papxFkp( rgfc, papxInFkps );
				allPapxFkps.push_back( papxFkp );
				rgfc.clear();
				papxInFkps.clear();
			}

			return allPapxFkps;
		}

	private:

		vector<unsigned int> rgfc;
		vector<BxPap> rgbx;
		vector<PapxInFkp> papxInFkps;
		unsigned char cpara;
	};
}